#ifndef __MYFILE_H__
#define __MYFILE_H__ 

#define SIZE 1024

#define FLUSH_NOW 1
#define FLUSH_LINE 2
#define FLUSH_ALL 4
#define FILE_MODE 0666

#include<stdio.h>

typedef struct IO_FILE
{
    int fileno;//文件描述符
    int flag;//缓冲方式
    char outbuffer[SIZE];//缓冲区
    int out_pos;//缓冲区指针
}_FILE;

_FILE* _fopen(const char* filename,const char* flag);
int _fwrite(const void *str, size_t size, size_t num,_FILE *fp);
void _fclose(_FILE* fp);
void _fflush(_FILE* fp);

#endif
