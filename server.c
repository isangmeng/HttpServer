#include "global.h"

int main()
{
    //初初始化服务器套节奏
    server = InitSocket(8888);
    //穿创建线程池
    pool = CreatePthreadPool();
    //初始化事树
    eventTree = InitEventTree(500);
    //创建接受请求任务
    AcceptConnectTask* acceptTask = CreatAcceptConnectTask(server);
    //创建新事件
    EventNode* node = CreateEventNode(acceptTask->ServerFd, EPOLLIN|EPOLLET, acceptTask);
    //事件管理任务
    EventManageTask* eventTask = CreateEventManageTask(eventTree);
    //添加到线程池进行执行
    AddTask(pool, eventTask);
    //添加监听事件到监听树
    AddEvent(eventTree, node);
    //监听事件发生
    WaitEvent(eventTree);
    return 0;
}
