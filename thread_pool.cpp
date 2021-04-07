#include<thread_pool.h>
#include<iostream>
Pthpool::Pthpool(int thread_cnt):para(this->task_queue,this->shutdown)
{
    this->thread_cnt=thread_cnt;
    this->thread_pool=new pthread_t[thread_cnt];
    this->task_queue=Task_Queue();
    this->shutdown=false;
    for(int i=0;i<thread_cnt;++i){
        pthread_create(&this->thread_pool[i],NULL,this->run_thread,&this->para);
    }
}
Pthpool::~Pthpool()
{
    // 等待所有线程执行完
    this->task_queue.destroy();
    this->shutdown=true;
    for(int i=0;i<this->thread_cnt;++i){
        pthread_join(this->thread_pool[i],NULL);
    }
    // 执行某种操作
    // std::cout<<"remain task queue size: "<<this->task_queue.size()<<"\n";
    // std::cout<<"end"<<std::endl;
    delete []thread_pool;
}
void Pthpool::add_task(void *f(void *),void *arg)
{
    this->task_queue.add_task(f,arg);
    // std::cout<<"add_task\n";
}
static int id=0;
static pthread_mutex_t lock=PTHREAD_MUTEX_DEFAULT;
void* Pthpool::run_thread(void *arg)
{
    node* node_arg=(node *)arg;
    Task_Queue& task_queue=node_arg->task_queue;
    bool& shutdown=node_arg->shutdown;
    // int thread_id=++id;
    // pthread_mutex_lock(&lock);
    // std::cout<<"thread_id "<<thread_id<<"\n"; //这个thread_id实现不太正确 只可以调试使用
    // pthread_mutex_unlock(&lock);
    while(true){
        if(shutdown){
            auto temp=task_queue.try_extract_task(); //把剩下的任务跑玩
            while(temp.second){
                // pthread_mutex_lock(&lock);
                // std::cout<<"run by"<<thread_id<<":";
                temp.first.f(temp.first.arg);
                // pthread_mutex_unlock(&lock);
                temp=task_queue.try_extract_task();
            }
            return (void*)0;
        }
        auto temp=task_queue.extract_task();
        if(!temp.second) continue;
        // pthread_mutex_lock(&lock);
        // std::cout<<"run by"<<thread_id<<":";
        temp.first.f(temp.first.arg);
        // pthread_mutex_unlock(&lock);
    }
    return (void *)0;
}
