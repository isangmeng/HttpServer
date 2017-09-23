#include "AcceptConnectTask.h"
#include "HttpTask.h"
#include <unistd.h>
/**
 * 创建一个接受请求任务
 * @param  server 服务器
 * @return        新任务
 */
AcceptConnectTask* CreatAcceptConnectTask(SocketConnect* server)
{
    AcceptConnectTask* AcceptTask = (AcceptConnectTask*)malloc(sizeof(AcceptConnectTask));
    AcceptTask->CallHandel = AcceptHandle;
    AcceptTask->arg = AcceptTask;
    AcceptTask->isLive = 1;
    AcceptTask->ServerFd = server->ServerFd;
    return AcceptTask;
}

/**
 * r任务回调函数
 * @param arg 参数
 */
void* AcceptHandle(void* arg)
{
    AcceptConnectTask* AcceptTask = (AcceptConnectTask*)arg;

    struct sockaddr_in client;

    socklen_t len = sizeof(client);
    int con = accept(AcceptTask->ServerFd, (struct sockaddr*)&client, &len);
    // close(con);

    if(con > 0)
    {
        HttpTask* task = CreateHttpTask(con);
        EventNode* node = CreateEventNode(con, EPOLLIN|EPOLLET, task);
        AddEvent(eventTree, node);
    }
    if(con < 0)
        printf("%s\n",strerror(con));
    if(DEBUG)
        printf("客户端连接%d\n",con);

    return NULL;
}
