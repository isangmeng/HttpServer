#ifndef _CL_BASE_TASK_
#define _CL_BASE_TASK_

#include "cl_link.h"

#define NEEDDIE 1
#define NOTDIE  0

typedef struct cl_base_task{
    cl_link_node    cl_task_node;               //任务队列需要
    void*           self;                       //任务回调参数
    void*           (*handler)(void* self);     //任务回调函数
    void*           (*destroy)(void* self);     //任务销毁函数
    int             need_destroy;               //是否需要销毁
    // pthread_mutex_t task_mutex;                 //任务锁
}cl_base_task;


#define cl_base_task_get_base(aim, type, node)       \
    (cl_base_task *) ((u_char *) aim + offsetof(type, node))


#define cl_base_task_get_task(aim, type, node)      \
    (type *) ((u_char *) aim - offsetof(type, node))
#endif
