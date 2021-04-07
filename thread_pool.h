#ifndef PTHPOOL
#define PTHPOOL
#include<pthread.h>
#include<task_queue.h>
struct node{
    Task_Queue &task_queue;
    bool &shutdown;
    node(Task_Queue &task_queue,bool &shutdown):task_queue(task_queue),shutdown(shutdown){
    }
};
class Pthpool
{
public:
    Pthpool(int thread_cnt);
    ~Pthpool();
    void add_task(void *f(void *),void *arg);
private:
    int thread_cnt;
    Task_Queue task_queue;
    pthread_t *thread_pool;
    bool shutdown;
    node para;
    static void* run_thread(void *arg);
};
#endif