#pragma once
#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<cstring>
#include<string>
#include<cstdio>
#include<cassert>
#include<fcntl.h>
#include<cerrno>
#include<vector>
#include<sstream>
#include<ctime>
#include<functional>
#define PIPE_NAME "./myfifo"
#define NUM 1024
#define MODE 0664
#define processnum 5
using namespace std;

enum Status
{
    FIFO_CREATE_ERR,
    FIFO_OPEN_ERR,
    FIFO_READ_ERR,
    FIFO_CLOSE_ERR,
    FIFO_UNLINK_ERR,
    FORK_ERR
};

class Pipe
{
public:
    Pipe()
    {
        int n=mkfifo(PIPE_NAME, MODE);
        if(n)
        {
            perror("mkfifo failed");
            exit(FIFO_CREATE_ERR);
        }
    }

    static void open_pipes(int& fd)
    {
        fd=open(PIPE_NAME, O_RDONLY);
        if(fd<0)
        {
            perror("open failed");
            exit(FIFO_OPEN_ERR);
        }
    }

    static void open_pipec(int& fd)
    {
        fd=open(PIPE_NAME, O_WRONLY);
        if(fd<0)
        {
            perror("open failed");
            exit(FIFO_OPEN_ERR);
        }
    }

    static void write_pipe(int& fd)
    {
        string str;
        while(1)
        {
            cout<<"this is client: Your input: ";
            getline(cin,str);
            write(fd,str.c_str(), str.size());
            sleep(1);
        }
    }
    static void read_pipe(int& fd)
    {
        char PIPE_BUF[NUM];
        while(1)
        {
            memset(PIPE_BUF, 0, sizeof(PIPE_BUF));
            ssize_t n=read(fd, PIPE_BUF, sizeof(PIPE_BUF));
            if(n>0)
            {
                cout<<"server read data: "<<PIPE_BUF<<endl;
            }
            else if(n==0) break;
            else
            {
                perror("read failed");
                exit(FIFO_READ_ERR);
            }
        }
    }

    static void close_pipe(int& fd)
    {
        int n=close(fd);
        if(n<0)
        {
            perror("close failed");
            exit(FIFO_CLOSE_ERR);
        }
    }

    ~Pipe()
    {
        int n=unlink(PIPE_NAME);
        if(n<0)
        {
            perror("unlink failed");
            exit(FIFO_UNLINK_ERR);
        }
    }

};

class channel
{
public:
    channel(pid_t id, const string& name)
        :slaverid(id), fifoname(name) 
    {}
    pid_t slaverid;
    string fifoname;
};

void open_pipe1(int& fd, const string& fifoname)
{
    fd=open(fifoname.c_str(), O_WRONLY);
    if(fd<0)
    {
        perror("open failed");
        exit(FIFO_OPEN_ERR);
    }
}

void open_pipe2(int& fd, const string& fifoname)
{
    fd=open(fifoname.c_str(), O_RDONLY);
    if(fd<0)
    {
        std::cout << "open failed:" << fifoname << endl;
        perror("open failed");
        exit(FIFO_OPEN_ERR);
    }
}

void slaver(const vector<channel>& channels, const vector<function<void()>>& tasks)
{
    std::cout << channels.size() << " channels in slaver process." << std::endl;
    //cout<<"getpid: " << getpid()<< "channelsid: " << channels[0].slaverid << endl;
    for(const auto& e : channels)
    {
        cout << "slaver process: " << e.slaverid << " with fifo: " << e.fifoname << endl;
    }
    string name;
    for(const auto&e: channels)
    {
        std::cout << "compare :" << e.slaverid << " " << getpid() << std::endl;
        if(e.slaverid==getpid())
        {
            name=e.fifoname;
            break;
        }
    }
    while(1)
    {
        int cmdcode=0,fd=0;
        open_pipe2(fd, name.c_str()); // 打开管道的读端
        ssize_t n = read(fd, &cmdcode, sizeof(int)); // 每次读取四个字节！
        close(fd); // 读取完毕后关闭管道
        if(n==sizeof(int))
        {
            cout<<"child says: recieve the message "<<cmdcode<<" pid: "<<getpid()<<endl;
            tasks[cmdcode](); // 执行对应的任务
        }
        else if(n==0) break;
    }
}


void task1()
{
    //模拟任务1
    cout << "Executing task 0 in process pool." << endl;
}

void task2()
{
    //模拟任务2
    cout << "Executing task 1 in process pool." << endl;
}

void task3()
{
    //模拟任务3
    cout << "Executing task 2 in process pool." << endl;
}

void task4()
{
    //模拟任务4
    cout << "Executing task 3 in process pool." << endl;
}  


void initprocesspool(vector<channel>& channels, vector<function<void()>>& tasks)
{
    tasks= {task1, task2, task3, task4}; // 任务队列
    vector<string> str;
    for(int i=0;i<processnum;i++)
    {
        string tmp;
        tmp+="./myfifo-";
        tmp+=to_string(i);

        int n=mkfifo(tmp.c_str(), MODE);
        if(n)
        {
            perror("mkfifo failed");
            exit(FIFO_CREATE_ERR);
        }
        str.push_back(tmp);

        pid_t id=fork();
        if(id>0)
        {
            //父进程
            std::cout << "pushid: "<< id << std::endl;
            channels.push_back(channel(id,str[i]));
            std::cout << "channels size: " << channels.size() << std::endl;
        }
        else if(id==0)
        {
            sleep(2);
            //子进程
            slaver(channels, tasks);
            exit(0);
        }
        else
        {
            perror("fork failed");
            exit(FORK_ERR);
        }
        sleep(10);
    }

}


void ctrlslaver(const vector<channel>& channels, const vector<function<void()>>& tasks)
{
    int cmdcode=0;//第几个任务
    for(int i=0;i<10;i++)
    {
        //随机选取一个进程
        int process_index = rand() % channels.size();
        //将任务轮流随机分配给进程
        cout<<"parent says: send the message "<<cmdcode<<" to process: "
            <<channels[process_index].slaverid<<" "
            <<"第"<<i<<"次发送"<<endl;
        int fd=0;
        open_pipe1(fd,channels[process_index].fifoname);
        write(fd, &cmdcode, sizeof(int));
        close(fd); // 发送完毕后关闭管道
        cmdcode++;cmdcode%= tasks.size(); // 循环使用任务
    }
}

void destroyprocesspool(const vector<channel>& channels)
{
    for(auto&e:channels)
    {
        int n=unlink(e.fifoname.c_str());
        if(n<0)
        {
            perror("unlink failed");
            exit(FIFO_UNLINK_ERR);
        }
    }

    for(int i=0;i<processnum;i++)
    {
        waitpid(channels[i].slaverid, nullptr, 0); // 等待子进程结束
    }

    cout<<"Process pool destroyed successfully."<<endl;
}