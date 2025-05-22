#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
//这是getpid和getppid所需要包含的头文件

void test1()
{
    while(1)                                                      
    {                                                             
      //这里使用PID，PPID可以进行系统调用
          printf("my PID:%d \t my PPID:%d \t\n",getpid(),getppid());
          usleep(5000);                   
    }
}

void test2()
{
    printf("大家好啊~，my PID:%d \t my PPID:%d \t\n",getpid(),getppid());
    //这个测试函数用于测试fork函数创建子进程的过程
    pid_t id=fork();
    //在fork函数中，创建了子进程：
    //1.创建子进程PCB
    //2.填充PCB对应的内容
    //3.让子进程和父进程指向同样的代码
    //4.父子进程都是有独立的task_struct，可以被CPU调度运行了
    //其中fork函数中有return语句，对于父子进程来说是共有的，因此有两个返回值，那么为什么id变量可以拥有两个值呢？当子进程要对变量进行修改时，Linux会发生数据层面的写时拷贝，重新在内存上开辟空间，这么做的目的是防止大面积无效拷贝复制占用操作系统的内存空间，从而降低运行效率
    if(id==0)
    {
        //说明这是子进程
        while(1)
				{
						printf("child: my PID:%d \t my PPID:%d \t\n",getpid(),getppid());
            usleep(1000000);
				}
				
    }
		else if(id>0)
    {
        //说明这是一个父进程
        while(1)
        {
            printf("parent: my PID:%d \t my PPID:%d \t\n",getpid(),getppid());
            usleep(1000000);
        }
    }
    else 
    {
        //error
        printf("errno\n");
    }
}

int main()
{
   // test1();
    test2();
    return 0;
}
