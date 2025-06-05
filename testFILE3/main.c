#include<unistd.h>
#include<stdio.h>
#include<string.h>

void test1()
{
    const char* input1="hello fwrite";
    const char* input2="hello write";
    printf("hello printf\n");
    fprintf(stdout,"hello fprintf\n");
    fwrite(input1,strlen(input1),1,stdout);
    //以上是C语言提供的库函数，数据会与用户缓冲区接触！
    write(1,input2,strlen(input2));//没有必要+1哦！

    //这里来实验不同的缓冲方式，以及重定向和子进程创建对缓冲区的影响
    close(1);
    //这里需要了解的是，\n是行缓冲，通过write写入到内存缓冲区，
    //而重定向到文件之后，就会变成全缓冲，缓冲区满才会write写入内存。
    //write直接写入内核，close只会将内核数据刷新到磁盘，不管user-space
}

void test2()
{
    //注意最后的return 0会刷新user-space，将数据通过内核写入到磁盘
    const char* input1="hello fwrite";
    const char* input2="hello write";
    printf("hello printf\n");
    fprintf(stdout,"hello fprintf\n");
    fwrite(input1,strlen(input1),1,stdout);
    write(1,input2,strlen(input2));

    //这里来实验不同的缓冲方式，以及重定向和子进程创建对缓冲区的影响
    fork();
    //这里你需要知道的是，fork创建子进程后对代码数据写时拷贝，user-space中的数据也一并复制，父子进程重定向到文件就会输出两段重复数据
}

void test3()
{
    //这里可以来验证一下行缓冲和全缓冲
    const char* input1="hello fwrite";
    const char* input2="hello write";
    printf("hello printf\n");
    sleep(1);
    fprintf(stdout,"hello fprintf\n");
    sleep(1);
    fwrite(input1,strlen(input1),1,stdout);
    sleep(1);
    write(1,input2,strlen(input2));
    sleep(5);
}

void test4()
{
    
}
int main()
{
    test3();
    return 0;
}
