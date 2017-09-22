#ifndef _HTTP_TASK_H_
#define _HTTP_TASK_H_

#include <pthread.h>
typedef struct HttpTask{
    void* next;
    void* pre;
    void* (*CallHandel)(void* arg);
    void* arg;
    int isLive;
    void* (*DestroyTask)(void* arg);
    pthread_mutex_t lockIsLive;

    int clientFd;
}HttpTask;

HttpTask* CreateHttpTask(int fd);

void* Handle(void* arg);
void* DestroyHandle(void* arg);

#endif
