# pthread_pool
A simple implementation of threads pool using pthread.h with C++ 

In my implementation:
1. All the task will be finished before the pool is destroyed
2. Each thread_pool has its own tasks_queue if you create multiple pools.
