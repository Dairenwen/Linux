#include"log.hpp"

void introduce(int n,...)
{
    //这里来介绍可变参数
    //可变参数至少有一个固定参数，其他的按照从右到左，依次创建栈帧，最后轮到n
    va_list s;//其实就是char*类型
    va_start(s, n); //初始化s，n是最后一个固定参数,代表初始指针s跳转到第一个可变参数
    for(int i=0;i<n;i++)
    {
        int x=va_arg(s,int); //获取下一个参数，int还可以是别的类型，设置为要转换为的类型
        cout<<x<<endl;
    }
    va_end(s); //结束可变参数的使用
}



int main()
{
    //这里来测试日志
    LOG log(PRINT_TO_SCREEN);
    log(INFO, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log(WARNING, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log(FATAL, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log(DEBUG, "server open file done, error string: %s, error code: %d", strerror(errno), errno);

    LOG log1(PRINT_TO_ONEFILE);
    log1(INFO, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log1(WARNING, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log1(FATAL, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log1(DEBUG, "server open file done, error string: %s, error code: %d", strerror(errno), errno);

    LOG log2(PRINT_TO_ClASSFILE);
    log2(INFO, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log2(WARNING, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log2(FATAL, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log2(DEBUG, "server open file done, error string: %s, error code: %d", strerror(errno), errno);


    return 0;
}