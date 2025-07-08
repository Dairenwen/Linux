#include"comm.hpp"

int main()
{
    Pipe pipe;
    int fd=0;
    //这是服务端代码，创建管道，打开管道，写入数据，关闭管道，删除管道
    pipe.open_pipes(fd);
    pipe.read_pipe(fd);
    pipe.close_pipe(fd);
    return 0;
}