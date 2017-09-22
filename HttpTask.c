#include "HttpTask.h"
#include "EventTree.h"
#include <malloc.h>
#include <unistd.h>

HttpTask* CreateHttpTask(int fd)
{
    HttpTask* task = malloc(sizeof(HttpTask));
    task->arg = task;
    task->clientFd = fd;
    task->CallHandel = Handle;
    task->DestroyTask = DestroyHandle;
    return task;
}

void* Handle(void* arg)
{
    HttpTask* task = (HttpTask*)arg;
    char buf[1024];
    int n = read(task->clientFd, buf, sizeof(buf));
    if(n >= 0)
    {
        buf[n] = '\0';
        printf("%s\n", buf);
    }else{
        task->DestroyTask(task);
    }
        // DeleteEvent(eventTree, )

    // write(STDOUT_FILENO, buf, strlen(buf));
    return NULL;
}

void* DestroyHandle(void* arg)
{
    HttpTask* task = (HttpTask*)arg;
    close(task->clientFd);
    
    free(task);
    return NULL;
}
