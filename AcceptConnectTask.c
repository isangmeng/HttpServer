#include "AcceptConnectTask.h"
AcceptConnectTask* CreatAcceptConnectTask(SocketConnect* server)
{
    AcceptConnectTask* AcceptTask = (AcceptConnectTask*)malloc(sizeof(AcceptConnectTask));
    AcceptTask->CallHandel = AcceptHandle;
    AcceptTask->arg = AcceptTask;
    AcceptTask->ServerFd = server->ServerFd;
    return AcceptTask;
}

void* AcceptHandle(void* arg)
{
    AcceptConnectTask* AcceptTask = (AcceptConnectTask*)arg;
    while(1)
    {
        struct sockaddr_in client;

        socklen_t len = sizeof(client);
        int con = accept(AcceptTask->ServerFd, (struct sockaddr*)&client, &len);

        if(DEBUG)
            printf("客户端连接%d\n",con);

    }
    return NULL;
}
