#include<thread_pool.h>
#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
void *Func(void* arg){
    // cout<<*(int *)arg<<"\n";
    for(int i=0;i<10000000;++i);
    return (void *) 0;
}
int arg2[100];
int main()
{
    clock_t st,ed;
    st=clock();
    {
        Pthpool thread_pool(30);
        for(int i=1;i<=30;++i){
            arg2[i]=i;
            thread_pool.add_task(Func,&arg2[i]);
        }
    }
    ed=clock();
    cout<<ed-st<<"ms\n";
}