#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define filename "test.txt"

void test1()
{
    close(0);
    int pd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0666);
    const char* input="The file is opened in append mode.  Before each write(2), the file offset is positioned at the end  of  the  file,  as  if  with  lseek(2).";
    printf("文件描述符为：%d\n",pd);
    //这里测试说明，文件描述符的设置是按照线性遍历的，
    //哪个描述符空闲就使用哪个
}

void test2()
{
    //自己来模拟一个重定向
    close(1);
    int pd=open(filename,O_WRONLY|O_CREAT|O_APPEND,0666);
    const char* input="The file is opened in append mode.  Before each write(2), the file offset is positioned at the end  of  the  file,  as  if  with  lseek(2).";
    write(pd,input,sizeof(char)*strlen(input));
    //这里可以看到原本要向stdout的文字写入了文件，这种改变了输入输出目的地貌似就叫做重定向

}

void test3()
{
    //来介绍一下重定向的系统调用把！dup2
    int pd=open(filename,O_WRONLY|O_CREAT|O_APPEND,0666);
    if(pd<0) perror("open failed");
    const char* input="The file is opened in append mode.  Before each write(2), the file offset is positioned at the end  of  the  file,  as  if  with  lseek(2).\n";
   //这里重定向可以使用dup2（原来的fd，要打开的fd）
   //本质上是将oldfd的file_struct的地址拷贝到了newfd的位置
   dup2(pd,1);
    write(1,input,sizeof(char)*strlen(input));
    //这是输出重定向
}

void test4()
{
    //这是输入重定向
    int fd=open(filename,O_RDONLY);
    dup2(fd,0);
    char arr[1024]={0};
    ssize_t s=read(0,arr,sizeof(arr)-1);
    if(s>0)
    {
        //seccuss
        arr[s]='\0';
        printf("%s\n",arr);
    }
}

void test5()
{
    //命令行：./a.out 1>normal.txt 2>err.txt 意思是将stdout重定向为nor，stderr重定向为err
    //./a.out 1>normal.txt 2>&1 意思是将stdout重定向为nor，stderr重定向为修改过的1（nor）
    const char* nor="this is a normal message\n";
    const char* err="this is a errno message\n";
    int k=5;
    while(k)
    {
        fprintf(stdout,nor);
        fprintf(stderr,err);
        k--;
    }
}
int main()
{
    umask(0);
    test5();
    return 0;
}
