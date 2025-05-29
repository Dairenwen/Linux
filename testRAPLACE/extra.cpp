#include<iostream>

using namespace std;

int main(int argc,char* argv[],char* env[])
{
    cout<<"this is cpp file"<<endl;
    int i=0;
    for(;argv[i];i++)
    {
        cout<<argv[i]<<endl;
    }
    for(i=0;env[i];i++)
    {
        cout<<env[i]<<endl;
    }
    return 0;
}
