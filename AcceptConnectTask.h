#ifndef _ACCEPT_CONNECT_TASK_H_
#define _ACCEPT_CONNECT_TASK_H_


#include "SocketConnect.h"
#include "global.h"
typedef struct AcceptConnectTask{
    void* next;
    void* pre;
    void* (*CallHandel)(void* arg);
    void* arg;
    int ServerFd;
}AcceptConnectTask;


AcceptConnectTask* CreatAcceptConnectTask(SocketConnect* server);

void* AcceptHandle(void* arg);

#endif
