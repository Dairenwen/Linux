#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

void testPATH1()
{
    //这里演示了系统调用env的方式
    char aName[20]={0};
    strcpy(aName,getenv("USER"));
    if(strcmp(aName,"root"))
    {
        printf("drw1\n");
    }
    else{
        printf("root\n");
    }

}

void testPATH2(int argc,char* argv[],char* env[])
{
    int i=0;
    //这里根据命令行参数表以及环境变量参数表最后一位为NULL可以直接打印
    for(;argv[i];i++)
    {
        printf("%s\n",argv[i]);
    }
    sleep(1);
    for(i=0;env[i];i++)
    {
        printf("%s\n",env[i]);
    }
    //这里打印出的环境变量与父进程bash相同，说明环境变量具有继承性以及全局性

}

void testPATH3(int argc,char* argv[],char* env[])
{
    if(argc==2)
    {
        if(strcmp(argv[0],"testPATH")==0)
        {
            sleep(40);
            chdir(argv[1]);
            printf("oi!\n" );
            sleep(5);
            //这里可以调用系统调用，建立内建命令，切换目录
        }
    }
    else{
        return;
    }
}

void testPATH4()
{
    extern char** environ;
    int i=0;
    for(;environ[i];i++)
    {
        printf("%s\n",environ[i]);
    }
}

int main(int argc,char* argv[],char* env[])
{
    testPATH3(argc,argv,env);
   // testPATH4();
   return 0;
}
