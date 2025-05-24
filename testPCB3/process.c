#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <stdlib.h>

void test1()
{
   //S Sleepint可以对应到阻塞态，当printf时CPU会等待输入到外设中，两者速度相差太大，为S状态，其他情景中当等待数据就绪时也会是阻塞状态（S态）
    while(1)
    {
        printf("this is a process,PID:%d\n",getpid());
        sleep(1);
    }
}

void test2()
{
    //R Running可以对应到运行态，后面+代表前台运行，没有+代表在后台运行，运行时加上&可以放于后台运行
    while(1)
    {
    }
}

void test3()
{
    //T/t stop 暂停状态，表示当前进程暂停运行，暂停和继续一个进程可以使用kill命令：9) SIGKILL	18) SIGCONT 19) SIGSTOP，T和S其实是有一定的区别的，S可以代表等待资源就绪，而T不一定是因为资源的问题而暂停的，t可以是调试时遇到断点而暂停的
    int arr[10]={10,9,8,7,6,5,4,3,2,1};
    int i=0;int j=0;
    for(i=0;i<10;i++)
    {
        for(j=i+1;j<10;j++)
        {
            if(arr[i]>arr[j])
            {
                int tmp=arr[j];
                arr[j]=arr[i];
                arr[i]=tmp;
            }
        }
    }
    for(i=0;i<10;i++)
    {
        printf("->%d ",arr[i]);
        fflush(stdout);
    }
}

void test4()
{
    //Z 僵尸状态，当一个子进程执行完毕（正常退出或异常终止）后，其PCB不会立即被销毁，而是会保留一段时间，等待父进程通过 wait() 或 waitpid() 系统调用来获取其终止状态（如退出码、信号信息等），等待的这个过程中，子进程的状态为僵尸状态。之后会变为X死亡状态，如果父进程没有对子进程的资源进行释放，处于僵尸进程的子进程没有被处理，就会造成内存泄露！
    pid_t id=fork();
    if(id==0)
    {
        //说明是子进程
        int count=10;
        while(count)
        {
            printf("CHILD : PID: %d,PPID: %d\n",getpid(),getppid());
						count--;
            sleep(1);
        }
				exit(0);
    }
    else 
    {
        //说明是父进程
        while(1)
        {
            printf("PARENT : PID: %d,PPID: %d\n",getpid(),getppid()); 
            sleep(1);
        }
        //这里需要对子进程的资源进行释放，否则就会造成内存泄露！
    }
}



int main()
{
    test4();
    return 0;
}
