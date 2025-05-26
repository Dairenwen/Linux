#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int g_val_1;
int g_val_2=100;

void test1()
{
    printf("code addr: %p\n",test1);
    const char* str="hello bit";
    printf("read only string addr: %p\n",str);
    printf("init global value addr: %p\n",&g_val_2);
    printf("uninit global value addr: %p\n",&g_val_1);
    char* mem=(char*)malloc(100);
    printf("heap addr: %p\n",mem);
    printf("stack addr: %p\n",&str);
    printf("stack addr: %p\n",&mem);
    int a;
    int b;
    int c;
    printf("stack addr: %p\n",&a);
    printf("stack addr: %p\n",&b);
    printf("stack addr: %p\n",&c);
}

int g_val=0;

void test2()
{
    pid_t id=fork();
    if(id==0)
    {
        int cnt=5;
        while(1)
        {
            printf("i am child, pid : %d,ppid : %d, g_val : %d, &g_val : %p\n",getpid(),getppid(),g_val,&g_val);
            sleep(1);
            if(cnt) cnt--;
            else 
            {
                 g_val=200;
                 printf("changed!\n");
            }
        
        }
    }
    else 
    {
    
        while(1)
        {
            printf("i am parent, pid : %d,ppid : %d, g_val : %d, &g_val : %p\n",getpid(),getppid(),g_val,&g_val);
            sleep(1);
        }
    }
}


int main()
{
    test1();
    return 0;
}
