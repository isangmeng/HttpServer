/**
 * 接受请求的任务
 * 接受客户端的连接请求
 */
#ifndef _ACCEPT_CONNECT_TASK_H_
#define _ACCEPT_CONNECT_TASK_H_


#include "SocketConnect.h"
#include "global.h"
typedef struct AcceptConnectTask{
    void* next;
    void* pre;
    void* (*CallHandel)(void* arg);
    void* arg;
    int isLive;
    void* (*DestroyTask)(void* arg);
    pthread_mutex_t lockIsLive;

    int ServerFd;
}AcceptConnectTask;


/**
 * 创建一个接受请求任务
 * @param  server 服务器
 * @return        新任务
 */
AcceptConnectTask* CreatAcceptConnectTask(SocketConnect* server);

/**
 * r任务回调函数
 * @param arg 参数
 */
void* AcceptHandle(void* arg);

#endif
