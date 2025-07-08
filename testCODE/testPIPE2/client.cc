#include"comm.hpp"

int main()
{
    // int fd=0;
    // Pipe::open_pipec(fd);
    // Pipe::write_pipe(fd);
    // Pipe::close_pipe(fd);
    //想要用命名管道实现的进程池失败了，push实在父进程进行的，但在push之前就进行了fork
    //所以会导致文件无法打开
    vector<channel> channels; // 管道通道
    vector<function<void()>> tasks; // 任务队列
    initprocesspool(channels, tasks); // 初始化进程池
    ctrlslaver(channels, tasks); // 控制子进程执行任务
    destroyprocesspool(channels); // 销毁进程池
    return 0;
}