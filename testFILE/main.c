#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define ONE (1<<0)
#define TWO (1<<1)
#define THR (1<<2)
//补充一下open中运用到的单个int运用bit传递多个标志位
void show(int ret)
{
    if(ret&ONE) printf("1\n");
    if(ret&TWO) printf("2\n");
    if(ret&THR) printf("3\n");
}
void bitset()
{
    show(ONE);
    show(ONE|TWO);
    show(ONE|TWO|THR);
}
void test1()
{
    //打开文件的路径和文件名，默认在当前路径下新建一个文件
    //这个路径是如何获取的？当前路径就是进程的路径，保存在/proc/cwd中
    //如果我更改了当前路径，就可以改变文件的存储地点
    FILE* fp=fopen("log.txt","wb");
    //w打开文件，没有就创建，从头开始写入，清空之前所有内容，类似>
    //a打开文件，追加写入，类似>>
    if(fp==NULL)
    {
        perror("fopen failed");
        exit(1);
    }

    const char* message="abcd";
    fwrite(message,sizeof(char)*(strlen(message)),1,fp);
    //这里注意不要strlen+1算上了\0，文件写入仅仅针对字节，\0写入变成乱码，没有意义
    fclose(fp);
}

void test2()
{
    //来看看open的系统调用
    //open的接口有两个，第一个接口不能指定权限，创建文件权限是随机值
    //第二个可以指定权限，但要注意权限掩码默认为0002
    //int fd=open("open.txt",O_WRONLY|O_CREAT|O_TRUNC);
    //注意，这样调用的open权限是随机值，如果要指定随机值，那么请调用第二种open函数
    umask(0);//在当前进程中临时改变权限掩码，可以解决下面的问题
    //int fd1=open("open.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);//但是这样不会完全遵循0666，因为还存在着权限掩码的问题
    //close(fd1);
    int fd2=open("open.txt",O_WRONLY|O_CREAT|O_APPEND,0666);//但是这样不会完全遵循0666，因为还存在着权限掩码的问题
    if(fd2<0) perror("open failed\n");
    const char* str="ABCDEFGHIJKLMN";
    write(fd2,str,sizeof(str));
    write(fd2,"6666",4);//如果没有选择O_TRUNC就会导致写入没有清空，如果要追加，就带上O_APPEND
}

void test3()
{
//    char ret[1024]={0};
//    write(1,"HELLO LINUX",11);
//    read(0,ret,sizeof(ret));
//    printf("%s\n",ret);
    close(1);
    printf("%d\n",stdin->_fileno);//这三个流文件对应的文件描述符为0，1，2
    printf("%d\n",stdout->_fileno);
    printf("%d\n",stderr->_fileno);
    //虽然关掉了stdout，但是三个流是互相独立的，向stderr中打印不会影响什么
    write(2,"I am ALIVE!",11);
}
int main()
{
    umask(0);
    //bitset();
    //tesnt2();
    test3();
    return 0;
}
