#include<task_queue.h>
Task_Queue::Task_Queue()
{
    pthread_mutex_init(&this->task_lock,NULL);
    pthread_cond_init(&this->task_cond,NULL);
    this->shutdown=false;
}
Task_Queue::~Task_Queue()
{
    pthread_mutex_destroy(&this->task_lock);
}
bool Task_Queue::add_task(void *f(void *),void *arg)
{
    Task current_task={f,arg};
    pthread_mutex_lock(&this->task_lock);
    this->tasks.push(current_task);
    if(tasks.size()==1)pthread_cond_signal(&this->task_cond);
    pthread_mutex_unlock(&this->task_lock);
    return true;
}
void Task_Queue::destroy()
{
    this->shutdown=true;
    pthread_cond_broadcast(&this->task_cond);
}
std::pair<Task,bool> Task_Queue::extract_task()
{
    pthread_mutex_lock(&this->task_lock);
    while(this->tasks.size()==0&&!shutdown) pthread_cond_wait(&this->task_cond,&this->task_lock);
    if(shutdown){
        pthread_mutex_unlock(&this->task_lock);
        return std::make_pair(Task(),false);
    }
    // task must be greater than one
    auto temp=this->tasks.front();
    this->tasks.pop();
    pthread_mutex_unlock(&this->task_lock);
    return {temp,true};
}
std::pair<Task,bool> Task_Queue::try_extract_task()
{
    pthread_mutex_lock(&this->task_lock);
    if(this->tasks.size()==0){
        pthread_mutex_unlock(&this->task_lock);
        return std::make_pair(Task(),false);
    }
    else{
        auto tmp=this->tasks.front();
        this->tasks.pop();
        pthread_mutex_unlock(&this->task_lock);
        return std::make_pair(tmp,true);
    }
}
unsigned int Task_Queue::size(){
    return this->tasks.size();
}