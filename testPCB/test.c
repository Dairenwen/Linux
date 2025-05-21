#include<stdio.h>
#include<string.h>
#include<unistd.h>


//PCB
int main()
{
    char arr[20]="HELLO LINUX~!";
    while(1)
    {
        printf("%s\n",arr);
        sleep(1);
    }
    return 0;
}
