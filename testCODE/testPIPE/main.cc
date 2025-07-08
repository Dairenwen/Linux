#include<iostream>
#include<unistd.h>
#include<cstdlib>//这样是c/c++混用的写法
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
#define NUM 1024
#define TASKNUM 10
#define processnum 10
using namespace std;


void Write(int wfd)
{
    string str("This message is from writing.");
    char PIPE_BUF[NUM];
    int count=0;

    while(1)
    {
        sleep(1);
        memset(PIPE_BUF,'\0',sizeof(PIPE_BUF)); // 清空缓冲区
        //建议来使用一下snprintf，也可以使用string拼接
        snprintf(PIPE_BUF,sizeof(PIPE_BUF),"%d %s",count++,str.c_str());
        write(wfd,PIPE_BUF,sizeof(char)*strlen(PIPE_BUF));
        cout<<str<<"pid:"<<getpid()<<endl;
        
    }

}
void Read(int rfd)
{
    string str("This message is from reading.");
    char PIPE_BUF[NUM];
    int count=5;
    while(count>0)
    {
        memset(PIPE_BUF,'\0',sizeof(PIPE_BUF)); // 清空缓冲区
        ssize_t n=read(rfd,PIPE_BUF,sizeof(PIPE_BUF));
        if(n>0)
        {
            PIPE_BUF[strlen(PIPE_BUF)]='\0'; // 确保字符串以'\0'结尾
            cout<<str<<"pid:"<<getpid()<<" read data:"<<PIPE_BUF<<endl;
            sleep(1);
            count--;
        }
    }
}

void testpipe1()
{
    // 首先来使用以下pipe这个接口
    int pipefd[2];
    int ret=pipe(pipefd);
    if(ret==-1)
    {
        perror("pipe error");
        exit(1);
    }
    else
    {
        // cout<<"读文件描述符:"<<pipefd[0]<<" "<<"写文件操作符:"<<pipefd[1]<<endl;
        // 让pipefd[0]作为写端，pipefd[1]作为读端
        pid_t pid=fork();
        if(pid>0)
        {
            // 父进程
            close(pipefd[1]); // 关闭写端
            Read(pipefd[0]);

            //关闭读端
            close(pipefd[0]);
            //那么写端进程会被信号杀掉
            int status=0;
            pid_t ret= waitpid(pid,&status,0);
            if(ret==pid)
            {
                cout<<"等待成功！"<<endl;
                cout<<"exit_code:"<<" "<<((status>>8)&0xFF)<<endl;
                cout<<"exit_signal:"<<" "<<((status)&0x7F)<<endl;
            }
        }
        else if(pid==0)
        {
            // 子进程
            close(pipefd[0]); // 关闭读端
            Write(pipefd[1]);
        }
        else
        {
            perror("fork error");
        }
    }
}

// void testpipe2()
// {

// }

//这里来设计一个简易的进程池
class channel
{
public:
    channel(int wfd, pid_t id, const string& name)
        : cwdfd(wfd)
        , slaverid(id)
        , processname(name) 
    {}

    int getCwdFd() const { return cwdfd; }
    pid_t getSlaverId() const { return slaverid; }
    string getProcessName() const { return processname; }
private:
    int cwdfd;//发送任务的文件描述符
    pid_t slaverid;//子进程的pid
    string processname;//子进程的名字，方便打印日志
};


void slaver(const vector<function<void()>>& tasks)
{
    while(1)
    {
        int cmdcode=0;
        ssize_t n=read(0, &cmdcode, sizeof(int)); // 每次读取四个字节！
        if(n==sizeof(int))
        {
            cout<<"child says: recieve the message "<<cmdcode<<" pid: "<<getpid()<<endl;
            tasks[cmdcode](); // 执行对应的任务
        }
        else if(n==0) break;//说明写端关闭了，读到文件末尾，直接跳出
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
    vector<int> rubbish;//子进程用于处理
    //先来进行初始化
    for(int i=0;i<processnum;i++)
    {
        int pipefd[2];
        int ret=pipe(pipefd);
        if(ret==-1) perror("pipe error");

        pid_t pid=fork();
        if(pid>0)
        {
            //父进程
            close(pipefd[0]); // 关闭读端
            rubbish.push_back(pipefd[1]);
            channels.push_back(channel(pipefd[1], pid, "Process-" + to_string(i)));
            //第一次创建3，4，关闭3，传递4，之后父进程关闭3，下一次创建3，5，父进程又一次关闭3
            //这样子进程是45678，父进程永远是3
        }
        else if(pid==0)
        {
            cout<<"rubbish process: ";
            for(const auto&e:rubbish)
            {
                cout<< e << " ";
                close(e); // 关闭之前的管道
            }
            cout<<endl;
            //子进程
            close(pipefd[1]); // 关闭写端
            dup2(pipefd[0],0);//将读端复制到标准输入,方便一些，也可以不这样
            close(pipefd[0]); // 关闭原来的读端
            slaver(tasks);//做任务
            exit(0);//子进程一定要退出！不然会wait堵塞
        }
        else
        {
            perror("fork error");
        }
    }

    //这样是错的// function<void(ProcessPool* const)> task1_func = task1;
    // function<void(ProcessPool* const)> task2_func = task2;
    // function<void(ProcessPool* const)> task3_func = task3;
    // function<void(ProcessPool* const)> task4_func = task4;
    // tasks = { task1_func , task2_func , task3_func , task4_func };


}


void ctrlslaver(const vector<channel>& channels, const vector<function<void()>>& tasks)
{
    int cmdcode=0;//第几个任务
    for(int i=0;i<TASKNUM;i++)
    {
        //随机选取一个进程
        int process_index = rand() % channels.size();
        //将任务轮流随机分配给进程
        cout<<"parent says: send the message "<<cmdcode<<" to process: "
            <<channels[process_index].getSlaverId()<<" "
            <<"第"<<i<<"次发送"<<endl;
        sleep(2);
        write(channels[process_index].getCwdFd(), &cmdcode, sizeof(int));
        cmdcode++;cmdcode%= tasks.size(); // 循环使用任务
    }
}

void destroy(const vector<channel>& channels)
{
    for(const auto& e:channels)
    {
        close(e.getCwdFd()); // 关闭写端
    }
    //注意还要等待所有子进程结束，否则内存泄漏！
    sleep(5);
    for(int i=0;i<processnum;i++)
    {
        wait(nullptr);
        //waitpid(channels[i].getSlaverId(), nullptr, 0); // 子进程结束
    }
    sleep(5);
}

void Method1(const vector<channel>& channels)
{
    auto rit = channels.rbegin();
    while(rit!= channels.rend())
    {
        cout<<"Closing process: " << rit->getSlaverId() << endl;
        close(rit->getCwdFd()); // 关闭写端
        waitpid(rit->getSlaverId(), nullptr, 0);
        rit++;
    }
}


void destroyprocesspool(const vector<channel>& channels)
{
    //对进程池进行收尾
    //先关闭所有的管道通道
    // destroy(channels);
    Method1(channels);
}

//这里存在一个bug，在关闭所有的管道时，如果关闭一个管道的同时等待子进程退出会一直阻塞
//因为当父进程打开管道时，子进程也会继承这个管道的读端，
//如果正向遍历去关闭管道后等待子进程，那么会管道文件的引用计数不为0，一直阻塞
//有两种方法，1.反向回收，2.在创建的子进程中关闭父进程的残留管道

int main()
{
    //testpipe1();
    // testpipe3();
    //进程池测试
    srand((unsigned int)time(nullptr));
    vector<channel> channels; // 管道通道
    vector<function<void()>> tasks; // 任务队列

    initprocesspool(channels, tasks); // 初始化进程池
    // for(const auto&e: channels)
    // {
    //     cout << "Process ID: " << e.getSlaverId() 
    //          << ", Process Name: " << e.getProcessName() 
    //          << ", Write FD: " << e.getCwdFd() << endl;
    // }
    sleep(2);
    ctrlslaver(channels, tasks); // 控制子进程执行任务
    destroyprocesspool(channels); // 销毁进程池
    cout << "Process pool destroyed." << endl;
   
    return 0;
}
