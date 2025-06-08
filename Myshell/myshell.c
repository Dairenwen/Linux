#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define LEFT "["
#define RIGHT "]"
#define LABLE "$"
#define DELIM " \t"
#define LINE_SIZE 1024
#define ARGC_SIZE 32
#define EXIT_CODE 404

#define NONE -1
#define OUT 0
#define APPEND 1
#define IN 2

int exitcode=0;
int rdir=0;
char* filename=NULL;

extern char** environ;

const char* getuser()
{
    //这里都用环境变量实现，不用系统调用接口
    return getenv("USER");
}

const char* gethost()
{
    return getenv("HOSTNAME");
}

const char* getpwd()
{
    return getenv("PWD");
}

void check(char* arr)
{
    char* tmp=arr;
    while(*tmp)
    {
        if(*tmp=='>')
        {
            if(*(tmp+1)=='>')
            {
                *tmp='\0';tmp++;
                *tmp='\0';tmp++;
                while(*tmp==' ') tmp++;
                filename=tmp;
                rdir=APPEND;
            }
            else 
            {
                *tmp='\0'; tmp++;
                while(*tmp==' ') tmp++;
                filename=tmp;
                rdir=OUT;
            }
        }
        else if(*tmp=='<')
        {
            *tmp='\0';
            tmp++;
            while(*tmp==' ') tmp++;
            filename=tmp;
            rdir=IN;
        }
        else 
        {
            //do nothing
        }
        tmp++;
    }
}
void interact(char* commandline,int size)
{
    printf(LEFT"%s@%s %s""]""$ ",getuser(),gethost(),getpwd());
    char* ret=fgets(commandline,size,stdin);
    if(ret==NULL)
    {
        //fgets读取错误会返回NULL，正常则返回指针
        perror("fgets failed");
    }
    else 
    {
        //处理一下读取到的\n,如输入abcd\n,则会读取abcd\n\0
        commandline[strlen(commandline)-1]='\0';
        check(commandline);
    }
}

int splitstring(char* commandline,char* argv[])
{
    int i=0;
    argv[i++]=strtok(commandline,DELIM);
    if(argv[0]==NULL) return 0;
    while(argv[i++]=strtok(NULL,DELIM));//循环处理，直到最后分割完返回NULL，作为命令行参数列表的最后一个NULL
    return i-1;
}

void NormalExecute(char* argv[])
{
        pid_t id=fork();
        if(id<0)
        {
            perror("fork failed");
        }
        else if(id==0)
        {
            //child
            int fd=0;
            if(rdir==OUT)
            {
                fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0666);
                dup2(fd,1);
            }
            else if(rdir==APPEND)
            {
                fd=open(filename,O_WRONLY|O_CREAT|O_APPEND,0666);
                dup2(fd,1);
            }
            else if(rdir==IN)
            {
                fd=open(filename,O_RDONLY);
                dup2(fd,0);
            }
            execvpe(argv[0],argv,environ);
            exit(EXIT_CODE);
        }
        else 
        {
            int status=0;
            pid_t ret=waitpid(id,&status,0);
            if(ret==id)
            {
                exitcode=WEXITSTATUS(status);
            }

        }
}

int buildCommand(char* argv[],int* argc)
{
    if(*argc==2&&strcmp(argv[0],"cd")==0)
    {
        chdir(argv[1]);
        char tmp[LINE_SIZE];
        getcwd(tmp,sizeof(tmp));//获取路径再去更新环境变量
        sprintf(getenv("PWD"),"%s",tmp);//sprintf可以改变字符串信息
        return 1;
    }
    else if(*argc==2&&strcmp(argv[0],"export")==0)
    {
        //注意这里环境变量并不存放在env中，env中保存的是地址
        //如果是临时变量或者栈区，或被其他数据覆盖，建议单独开辟一块区域保存环境变量
        char* tmp=(char*)malloc(sizeof(char)*LINE_SIZE);
        if(tmp==NULL) perror("malloc failed");
        strcpy(tmp,argv[1]);
        putenv(tmp);
        return 1;
    }
    else if(*argc==2&&strcmp(argv[0],"echo")==0)
    {
        if(strcmp(argv[1],"$?")==0)
        {
            printf("%d\n",exitcode);
            exitcode=0;
        }
        else if(*argv[1]=='$')
        {
            char* tmp=getenv(argv[1]+1);
            if(tmp) printf("%s\n",tmp);
        }
        else 
        {
            printf("%s\n",argv[1]);
        }
        return 1;
    }
    //...还有很多很多，可以自己添加
    
    if(strcmp(argv[0],"ls")==0)
    {
        argv[(*argc)++]="--color";
        argv[*argc]=NULL;
        return 0;
    }
    return 0;

}
int main()
{
    //在这里实现自己的命令行解释器
    char commandline[LINE_SIZE];
    char* argv[ARGC_SIZE];

    while(1)
    {
        //1.
        filename=NULL;
        rdir=NONE;
        //2.交互问题，获取命令行
        interact(commandline,sizeof(commandline));

        //3.字串使用strtok分割，解析命令行
        int argc=splitstring(commandline,argv);
        //4.指令的判断
        if(argc==0) continue;

        //这里来处理一下内建命令
        int ret=buildCommand(argv,&argc);
        if(!ret) NormalExecute(argv);

    }
    return 0;
}
