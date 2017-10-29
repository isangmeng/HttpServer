#ifndef _CL_BASE_TASK_
#define _CL_BASE_TASK_

#include "cl_link.h"

typedef struct cl_task{
    cl_link_node    cl_task_node;               //任务队列需要
    void*           self;                       //任务回调参数
    void*           (*handler)(void* self);     //任务回调函数
    void*           (*destroy)(void* self);     //任务销毁函数
    int             need_destroy;               //是否需要销毁
}cl_task;


#endif
