#ifndef _BASE_TASK_H_
#define _BASE_TASK_H_

#include <pthread.h>
typedef struct Task{
    void* next;
    void* pre;
    void* (*CallHandel)(void* arg);
    void* arg;
    int isLive;
    void* (*DestroyTask)(void* arg);
    pthread_mutex_t lockIsLive;
}Task;


#endif
