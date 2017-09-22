#ifndef _HTTP_TASK_H_
#define _HTTP_TASK_H_

typedef struct HttpTask{
    void* next;
    void* pre;
    void* (*CallHandel)(void* arg);
    void* arg;

    void* (*DestroyTask)(void* arg);
    int clientFd;
}HttpTask;

HttpTask* CreateHttpTask(int fd);

void* Handle(void* arg);
void* DestroyHandle(void* arg);

#endif
