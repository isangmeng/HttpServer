#include "HttpTask.h"
#include "EventTree.h"
#include "global.h"
#include <malloc.h>
#include <unistd.h>
#include<sys/socket.h>

HttpTask* CreateHttpTask(int fd)
{

    HttpTask* task = malloc(sizeof(HttpTask));
    task->arg = task;
    task->isLive = 1;
    task->clientFd = fd;
    task->CallHandel = Handle;
    task->DestroyTask = DestroyHandle;
    pthread_mutex_init(&task->lockIsLive, NULL);
    return task;
}

void* Handle(void* arg)
{
    HttpTask* task = (HttpTask*)arg;
    char* buf = (char*)malloc(sizeof(char)*1024);

    while(1)
    {
        int n = read(task->clientFd, buf, 1024);
        if(n > 0)
        {
            buf[n] = '\0';
            printf("%s\n", buf);
            write(task->clientFd, buf, sizeof(buf));
        }else{
            pthread_mutex_lock(&task->lockIsLive);
            task->isLive = 0;
            pthread_mutex_unlock(&task->lockIsLive);
            break;
        }
        // task->isLive = 0;


        // struct epoll_event* new = (struct epoll_event*)malloc(sizeof(struct epoll_event));
        // new->events = EPOLLIN|EPOLLET;
        // new->data.ptr = task;
        // pthread_mutex_lock(&(eventTree->TreeLock));
        // epoll_ctl(eventTree->Root, EPOLL_CTL_MOD, task->clientFd, new);
        // eventTree->HasNum++;
        // pthread_mutex_unlock(&eventTree->TreeLock);
        // free(new);
        // AddEvent(eventTree,  CreateEventNode(task->clientFd, EPOLLIN|EPOLLET, task));
    }

    free(buf);
    epoll_ctl(eventTree->Root, EPOLL_CTL_DEL, task->clientFd, NULL);
    // close(task->clientFd);
    // task->DestroyTask(task->arg);

    // write(STDOUT_FILENO, buf, strlen(buf));
    return NULL;
}

void* DestroyHandle(void* arg)
{
    HttpTask* task = (HttpTask*)arg;
    close(task->clientFd);
    // shutdown(task->clientFd,SHUT_RDWR);
    free(task);
    return NULL;
}
