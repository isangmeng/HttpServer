#ifndef _CL_EVENT_
#define _CL_EVENT_

#include "cl_link.h"
#include "cl_pthread_pool.h"
#include <sys/epoll.h>


#define ADDEVENTSUCCESS 1
#define ADDEVENTFAIL 0

#define DELETESUCCESS 1
#define DELETEFAIL 0


typedef struct cl_event_node{
    int             fd;             //要监听的文件描述符
    int             events;         //监听的事件
    int             status;         //状态，是否处于监听状态
    // char*           buf;            //缓冲区
    long            last_active;    //最后活跃时间
    cl_link_node    cl_event;       //所有事件链表需要

    void*           task;           //任务指针
}cl_event_node;

typedef struct cl_event{
    cl_link*                cl_event_all;       //所有的事件
    struct epoll_event*     active_event;       //活动的事件
    int                     active_max;         //最大的活动事件
    int                     cl_event_root;      //事件树根
    pthread_mutex_t         cl_event_mutex;     //事件锁
    cl_pthread_pool*        pool;               //线程池
}cl_event;


#define cl_event_get_node(aim, type, node)       \
    (cl_event_node *) ((u_char *) aim + offsetof(type, node))


#define cl_event_get_event(aim, type, node)      \
    (type *) ((u_char *) aim - offsetof(type, node))


/**
 * 创建事件树对象
 * @param  n 事件树大小
 * @return   事件树对象
 */
cl_event* cl_event_create(int n, int m, cl_pthread_pool* pool);


/**
 * 添加事件
 * @param  event     事件对象
 * @param  new_event 新事件
 * @return           添加情况
 */
int cl_event_add_event(cl_event* event, cl_event_node* new_event);


/**
 * 从shi监听事件树上删除一个节点
 * @param  event      事件树
 * @param  delete_aim 删除对象
 * @return            删除状态
 */
int cl_event_delete_event(cl_event* event, cl_event_node* delete_aim);

/**
 * 等待事件发生
 * @param event 事件树
 */
void cl_event_wait_event(cl_event* event);


#endif
