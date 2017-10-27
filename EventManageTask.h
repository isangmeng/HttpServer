/**
 *  管理事件树
 */
#ifndef _EVENT_TASK_H_
#define _EVENT_TASK_H_
#include "global.h"
#include <unistd.h>
typedef struct EventManageTask{
    void* next;
    void* pre;
    void* (*CallHandel)(void* arg);
    void* arg;
    int isLive;
    void* (*DestroyTask)(void* arg);
    pthread_mutex_t lockIsLive;

    EventTree* eventTree;
}EventManageTask;

EventManageTask* CreateEventManageTask(EventTree* eventTree);

static void* ManageEvent(void* arg);

static void* DealHandel(plinkTab linkTab, void* arg, void* otherArg);
#endif
