#include"processBar.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
const char* status="|/-\\";

//颜色宏定义
#define NONE         "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"


//第一个版本，v1
void processbar()
{
   // char* choose[15]={RED,LIGHT_RED,GREEN,LIGHT_GREEN,BLUE,LIGHT_BLUE,DARY_GRAY,CYAN,LIGHT_CYAN,PURPLE,LIGHT_PURPLE,BROWN,YELLOW,LIGHT_GRAY,WHITE};
    srand((unsigned)time(NULL));
    char nums[LEN]={0};
    int count=0;
    int size=strlen(status);
    while(count<=100)
    {
        printf(LIGHT_BLUE"[%-100s][%lf%%][%c]\r"NONE,nums,(double)(100*count)/NUM,status[count%size]);
        nums[count++]=FILL;
        if(count+1<=100)
        {
            nums[count]='>';
        }
        fflush(stdout);
        usleep((rand()%10)*10000);
    }
    printf("\n");
}

//第二个版本，v2








