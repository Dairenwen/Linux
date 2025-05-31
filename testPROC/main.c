#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

void test1()
{
    //test 6 child process
    int i=0;
    pid_t id=0;
    for(i=0;i<10;i++)
    {
        id=fork();
        if(id==0)
        {
            printf("child[%d],pid=%d,ppid=%d\n",i,getpid(),getppid());
            exit(0);
        }
        sleep(1);
    }
    sleep(100);
}

const char* error[]={
    "malloc failed",
    "out of range",
    "bad allocator"};

//you can define your own error masseage

int test2()
{
    int i=0;
    for(i=0;i<60;i++)
    {
        printf("%s\n",strerror(i));
    }
    printf("finish!!!\n");
    return 2;
}

void test3()
{
    int* tmp=(int*)malloc(sizeof(int)*4*1024*1024);
    if(tmp==NULL)
    {
        printf("%s\n",strerror(errno));
        printf("error\n");
        exit(0);
    }
    printf("error");
    _exit(0);
    //在这里exit和_exit的区别一个是库函数，而另一个是系统调用，前者会冲刷缓冲区，而后者不会
}

//以下是对进程等待的测试
int testwait()
{
    pid_t id=fork();
    if(id<0)
    {
        perror("fork");
        return 1;
    }
    else if(id==0)
    {
        //child
        int cnt=5;
        while(cnt)
        {
            printf("I am child,pid:%d,ppid:%d,cnt:%d\n",getpid(),getppid(),cnt);
            cnt--;
            sleep(1);
        }
        exit(0);
    }
    else 
    {
        //parent
        int cnt=10;
        while(cnt)
        {
            
            printf("I am father,pid:%d,ppid:%d,cnt:%d\n",getpid(),getppid(),cnt);
            cnt--;
            sleep(1);
        }
        pid_t ret=wait(NULL);//这里wait返回的就是子进程创建成功后返回的pid
        if(ret==id)
        {
            printf("success!\n");
         }
    }
   return 0; 
}

//上面测试的是一个子进程，如果是多个呢？
void Runchild()
{
        int cnt=5;
        while(cnt)
        {
            printf("I am child,pid:%d,ppid:%d,cnt:%d\n",getpid(),getppid(),cnt);
            cnt--;
            sleep(1);
        }
}
void testmultiple()
{
    int i=0;
    int n=10;
    for(i=0;i<n;i++)
    {
        pid_t id=fork();
        if(id==0)
        {
            //child
            Runchild();
            exit(0);
        }
        printf("create successful! %d\n",i);//只有父进程才会执行
    }
    sleep(10);
    //对于多个子进程，只需要将wait遍历即可
    for(i=0;i<n;i++)
    {
        pid_t ret=wait(NULL);
        if(ret>0)
        {
            printf("wait %d success\n",ret);
        }
    }
    sleep(10);
}
int testwaitplus()
{
    pid_t id=fork();
    if(id<0)
    {
        perror("fork");
        return 1;
    }
    else if(id==0)
    {
        //child
        int cnt=20;
        while(cnt)
        {
            printf("I am child,pid:%d,ppid:%d,cnt:%d\n",getpid(),getppid(),cnt);
            cnt--;
            sleep(1);
        }
        exit(0);
    }
    else 
    {
        //parent
        int cnt=10;
        while(cnt)
        {
            
            printf("I am father,pid:%d,ppid:%d,cnt:%d\n",getpid(),getppid(),cnt);
            cnt--;
            sleep(1);
        }
        //pid_t ret=wait(NULL);//这里wait返回的就是子进程创建成功后返回的pid
       while(1)
       {
         int status=0;
       // pid_t ret=waitpid(-1,&status,0);
        pid_t ret=waitpid(-1,&status,WNOHANG);
         if(ret>0)
         {
            printf("success!\n");
            printf("exit successfully,pid:%d\n,exit_code:%d,exit_signal:%d\n",ret,(status>>8)&0xFF,status&0x7F);
            //还可以用宏
            if(WIFEXITED(status))//判断是否异常退出
            {
                printf("进程正常退出，无异常！exit_code:%d\n",WEXITSTATUS(status));
            }
            exit(0);
         }
        else if(ret<0)
        {
            printf("wait failed\n");
        }
        else 
        {
            printf("子进程还未退出，waiting...\n");
        }
        sleep(1);
       }
    }
   return 0; 
}

int main()
{
   // test1();
   // test3();
    //testwaitplus();
    return test2();
}
