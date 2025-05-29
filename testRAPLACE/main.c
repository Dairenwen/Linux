#include<stdio.h>
#include<unistd.h>

void test1()
{
    //单个进程进行进程替换
    printf("this is a begin:pid:%d,ppid:%d\n",getpid(),getppid());
    execl("/usr/bin/ls","ls","-a","-l",NULL); 
    printf("this is a end:pid:%d,ppid:%d\n",getpid(),getppid());
    //可以看到进程替换后代码和数据进行了替换，pid不变，execl后面的代码不再执行
}

void test2()
{
    //对多个进程进行进程切换
    pid_t id=fork();
    if(id==0)
    {
        //child
        printf("I am child,pid:%d,ppid:%d\n",getpid(),getppid());
        execl("/usr/bin/ls","ls","-l",NULL);
        printf("child end\n");
    }
    else 
    {
        //father
        printf("I am father,pid:%d,ppid:%d\n",getpid(),getppid());
        wait(NULL);
        printf("wait success\n");
        printf("father end\n");
    }
}

extern char** environ;

void test3()
{
    //对exec系统调用函数进行使用
    char* const arr[]={"ls","-a","-l",NULL };
    //execlp("testcpp","testcpp",NULL);
    //execv("/usr/bin/ls",arr);
    execle("./testcpp","testcpp",NULL,environ);
}
int main()
{
    //test1();
    //test2();
    test3();
    return 0;
}
