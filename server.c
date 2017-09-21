#include "global.h"

int main()
{
    //初初始化服务器套节奏
    server = InitSocket(8888);
    //穿创建线程池
    pool = CreatePthreadPool();
    //初始化事树
    eventTree = InitEventTree(100);
    //创建接受请求任务
    AcceptConnectTask* acceptTask = CreatAcceptConnectTask(server);
    //创建新事件
    EventNode* node = CreateEventNode(acceptTask->ServerFd, EPOLLIN, acceptTask);
    //添加监听事件到监听树
    AddEvent(eventTree, node);

    //监听事件发生
    WaitEvent(eventTree);
    return 0;
}