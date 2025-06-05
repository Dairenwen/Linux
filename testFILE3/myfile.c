#include"myfile.h"
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<assert.h>
#include<sys/types.h>

_FILE* _fopen(const char* filename,const char* flag)
{
    assert(filename&&flag);
    int fd=-1;
    if(strcmp(flag,"w")==0)
    {
        fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,FILE_MODE);
    }
    else if(strcmp(flag,"a")==0)
    {
        fd=open(filename,O_WRONLY|O_CREAT|O_APPEND,FILE_MODE);
    }
    else if(strcmp(flag,"r")==0)
    {
        fd=open(filename,O_RDONLY);
    }
    else return NULL;

    if(fd==-1) return NULL;

    _FILE* ret=(_FILE*)malloc(sizeof(_FILE));
    if(ret==NULL) return NULL;

    ret->fileno=fd;
    ret->flag=FLUSH_ALL;
    ret->out_pos=0;

    return ret;
}

int _fwrite(const void *str, size_t size, size_t num,_FILE *fp)
{
    assert(size>=0&&num>=0);
    for(int i=0;i<num;i++)
    {
        memcpy(&fp->outbuffer[fp->out_pos],str,size);
        fp->out_pos+=size;
    }

    if(fp->flag==FLUSH_NOW)
    {
        write(fp->fileno,fp->outbuffer,fp->out_pos);
        fp->out_pos=0;
    }
    else if(fp->flag==FLUSH_LINE)
    {
        for(int i=0;i<fp->out_pos;i++)
        {
            if(fp->outbuffer[i]=='\n')
            {
                //寻找\n，然后把之前的写入
                write(fp->fileno,fp->outbuffer,i);
                fp->out_pos=0;
            }
        }
    }
    else if(fp->flag==FLUSH_ALL)
    {
        if(fp->out_pos>=SIZE)
        {
            write(fp->fileno,fp->outbuffer,SIZE);
            fp->out_pos=0;
        }
    }

    return num;
}

void _fclose(_FILE* fp)
{
    assert(fp);
    if(fp->out_pos>0)
    {
        write(fp->fileno,fp->outbuffer,fp->out_pos);
        fp->out_pos=0;
    }
    free(fp);
    fp=NULL;
}

void _fflush(_FILE* fp)
{
    if(fp->out_pos>0)
    {
        write(fp->fileno,fp->outbuffer,fp->out_pos);
        fp->out_pos=0;
    }
}
void testmyfile()
{
    _FILE* fd=_fopen("myfile.txt","a");
    if(fd==NULL) perror("fopen failed");
    const char* writein="this is a message which needed be writen in the file\n";
    _fwrite(writein,strlen(writein),1,fd);
    _fflush(fd);
    _fclose(fd);
}

int main()
{
    testmyfile();
    return 0;
}
