#ifndef _CL_LINK_PTGREAD_POOL_
#define _CL_LINK_PTGREAD_POOL_

#include <pthread.h>
#include "cl_link.h"
#include "cl_base_task.h"

#define ADDTASKSUCCESS (1)
#define ADDTASKFAIL (0)
#define MAX_PTHREAD (1024)

// typedef struct cl_pthread cl_pthread;

typedef struct cl_pthread{

    cl_link_node    cl_pthread_node;        //线程池需要
    pthread_t       pid;                    //线程id
}cl_pthread;

typedef struct cl_pthread_pool{

    pthread_cond_t      has_task;           //任务到来通知
    pthread_mutex_t     pool_mutex;         //线程池锁
    cl_link*            task_queue;         //任务队列
    cl_link*            pthread_pool;       //线程池
    cl_link*            finish_task;        //已经完成的任务
    int                 waiting;            //等待的任务数量
    int                 active;             //正在执行的数量
    int                 max;                //最大线程数
    int                 step;               //添加线程和删除线程的步进值
}cl_pthread_pool;

/**
 * 创建线程池对象
 * @return 线程池
 */
cl_pthread_pool* cl_pthread_pool_create();

/**
 * 添加任务
 * @param  pool 线程池
 * @param  task 新任务
 * @return      添加状态
 */
int cl_pthread_pool_add_task(cl_pthread_pool* pool, void* task);

/**
 * 添加线程
 * @param  pool 线程池
 * @return      添加数量
 */
int cl_pthread_pool_add_pthread(cl_pthread_pool* pool);

/**
 * 线程主函数
 * @param arg 参数
 */
static void* cl_pthread_pool_main(void* arg);

#endif
