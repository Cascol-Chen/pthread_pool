#include<pthread.h>
#include<queue>
#include <utility>
#ifndef TASKQUEQUE
#define TASKQUEUE
struct Task{
    void* (*f)(void *);
    void *arg;
    Task(void* (*f)(void *),void *arg){
        this->f=f;
        this->arg=arg;
    }
    Task(){
        
    }
};
class Task_Queue
{
public:
    Task_Queue();
    ~Task_Queue();
    bool add_task(void *f(void *),void *arg);
    std::pair<Task,bool> extract_task();
    std::pair<Task,bool> try_extract_task();
    unsigned int size();
    void destroy();
private:
    std::queue<Task> tasks;
    pthread_mutex_t task_lock;
    pthread_cond_t task_cond;
    bool shutdown;
};
#endif