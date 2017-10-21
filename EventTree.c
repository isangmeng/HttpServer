#include "EventTree.h"
#include "global.h"
#include <unistd.h>

/**
 * 初始化事件树
 * @param  ActiveEventNum 活动事件大小
 * @return                树根
 */
EventTree* InitEventTree(unsigned int ActiveEventNum)
{
    EventTree* eventTree = (EventTree*)malloc(sizeof(EventTree));
    if(eventTree == NULL)
    {
        printf("malloc eventTree error:\n");
        exit(1);
    }
    pthread_mutex_init(&eventTree->ActiveEventLock, NULL);
    eventTree->Root = epoll_create(ActiveEventNum);
    eventTree->ActiveEventNum = ActiveEventNum-1;
    eventTree->HasNum = 0;
    eventTree->ActiveEvent = (struct epoll_event*)malloc(sizeof(struct epoll_event) * eventTree->ActiveEventNum);
    eventTree->AllEvent = CreatLinkTab();
    if(eventTree->ActiveEvent == NULL)
    {
        printf("malloc eventTree error:\n");
        exit(1);
    }
    return eventTree;
}

/**
 * 创建新事件
 * @param  fd     文件描述符
 * @param  event  事件
 * @param  task   回调任务
 * @return        新节点地址
 */
EventNode* CreateEventNode(int fd, int event, void* task)
{
    EventNode* newNode = malloc(sizeof(EventNode));
    if(newNode == NULL)
    {
        printf("malloc error\n");
        exit(1);
    }
    newNode->task = task;
    newNode->task->arg = task;

    newNode->nodie = 0;
    newNode->event = event;
    newNode->fd = fd;
    newNode->time = time(NULL);
    return newNode;
}

/**
 * 添加新节点
 * @param eventTree 事件树根
 * @param newNode   新节点
 */
void AddEvent(EventTree* eventTree, EventNode* newNode)
{
    struct epoll_event* new = (struct epoll_event*)malloc(sizeof(struct epoll_event));
    new->events = newNode->event;
    new->data.ptr = newNode;
    pthread_mutex_lock(&(eventTree->TreeLock));
    epoll_ctl(eventTree->Root, EPOLL_CTL_ADD, newNode->fd, new);
    eventTree->HasNum++;
    pthread_mutex_unlock(&eventTree->TreeLock);
    AddLinkTabNode(eventTree->AllEvent, newNode);
    // free(new);
}

/**
 * 删除事件
 * @param eventTree 事件根节点
 * @param newNode   要删除的节点
 */
void DeleteEvent(EventTree* eventTree, EventNode* deleteNode)
{
    printf("删除事件");
    pthread_mutex_lock(&(eventTree->TreeLock));
    epoll_ctl(eventTree->Root, EPOLL_CTL_DEL, deleteNode->fd, NULL);
    // close(deleteNode->fd);
    eventTree->HasNum--;
    deleteNode->task->DestroyTask(deleteNode->task);
    free(deleteNode->task);
    pthread_mutex_unlock(&eventTree->TreeLock);
    DeleteLinkTabNode(eventTree->AllEvent, deleteNode);
    // free(deleteNode);
}

/**
 * 循环监听事件
 * @param eventTree 事件树
 */
void WaitEvent(EventTree* eventTree)
{
    EventNode* activeNode;
    while(1)
    {
        pthread_mutex_lock(&eventTree->ActiveEventLock);
        int nready = epoll_wait(eventTree->Root, eventTree->ActiveEvent, eventTree->ActiveEventNum, -1);
        if(DEBUG)
            printf("所有事件:%d,活动事件%d\n", eventTree->HasNum,nready);
        for(int i=0; i<nready; i++)
        {
            printf("处理事件\n");
            activeNode = (EventNode*)(eventTree->ActiveEvent[i].data.ptr);
            // activeNode->CallHandel(activeNode->arg);
            activeNode->time = time(NULL);
            AddTask(pool, activeNode->task);
            // activeNode->task->CallHandel(activeNode->task->arg);

            pthread_mutex_lock(&activeNode->task->lockIsLive);
            printf("活动事件活%d\n", activeNode->task->isLive);
            // if(activeNode->task->isLive == 0)
            // {
            //     printf("删除任务，删除事件\n");
            //     // activeNode->task->DestroyTask(activeNode->task->arg);
            //     DeleteEvent(eventTree,activeNode);
            // }
            pthread_mutex_unlock(&activeNode->task->lockIsLive);

                // DeleteEvent(eventTree, activeNode);
        }
        // for(int i=0; i<nready; i++)
        // {
        //     activeNode = (EventNode*)(eventTree->ActiveEvent[i].data.ptr);
        //     if(activeNode->fd != 3)
        //         DeleteEvent(eventTree, activeNode);
        // }

        nready = 0;
        pthread_mutex_unlock(&eventTree->ActiveEventLock);

    }
}
