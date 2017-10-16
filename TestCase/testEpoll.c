// #include "../CPthreadPool.h"
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include "../SocketConnect.h"
#include "../AcceptConnectTask.h"
#include "../EventTree.h"


typedef struct CTask{
    void* next;
    void* pre;
    // RunHandel
    void* (*CallHandel)(void* arg);
    void* arg;
    int fd;
}CTask;

void* HandelEpoll(void* arg){
    CTask* task = (CTask*)arg;
    char buf[1024];
    read(task->fd, buf, 1024);
    printf("%s\n",buf );
    return NULL;
}

int main()
{
    CTask* task = (CTask*)malloc(sizeof(CTask));

    task->CallHandel = HandelEpoll;
    task->fd = STDIN_FILENO;
    EventTree* tree = InitEventTree(100);
    EventNode* node = CreateEventNode(STDIN_FILENO, EPOLLIN, task);
    AddEvent(tree, node);

    WaitEvent(tree);
    return 0;
}
