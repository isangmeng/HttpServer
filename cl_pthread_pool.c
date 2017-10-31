#include "cl_pthread_pool.h"
#include "cl_link.h"
#include <signal.h>

/**
 * 创建线程池对象
 * @return 线程池
 */
cl_pthread_pool* cl_pthread_pool_create()
{
    cl_pthread_pool* pool = malloc(sizeof(cl_pthread_pool));
    if(pool == NULL){
        write(STDERR_FILENO, "malloc error", sizeof("malloc error"));
        return NULL;
    }
    pthread_mutex_init(&(pool->pool_mutex), NULL);
    pthread_mutex_lock(&(pool->pool_mutex));
    pool->active = 0;
    pool->waiting = 0;
    pool->max = MAX_PTHREAD;
    pool->pthread_pool = cl_link_create();
    pool->task_queue = cl_link_create();
    pool->finish_task = cl_link_create();
    pool->step = 10;
    pthread_cond_init(&(pool->has_task), NULL);
    pthread_mutex_unlock(&(pool->pool_mutex));
    cl_pthread_pool_add_pthread(pool);
    return pool;
}

/**
 * 添加任务
 * @param  pool 线程池
 * @param  task 新任务
 * @return      添加状态
 */
int cl_pthread_pool_add_task(cl_pthread_pool* pool, void* task)
{
    pthread_mutex_lock(&(pool->pool_mutex));
    cl_link_push(pool->task_queue, task);
    pool->waiting++;
    pthread_mutex_unlock(&(pool->pool_mutex));
    pthread_cond_signal(&(pool->has_task));
    return ADDTASKSUCCESS;
}

/**
 * 添加线程
 * @param  pool 线程池
 * @return      添加数量
 */
int cl_pthread_pool_add_pthread(cl_pthread_pool* pool)
{
    pthread_mutex_lock(&(pool->pool_mutex));
    cl_pthread* new_thread;
    int n = ((pool->pthread_pool->sum + pool->step) > pool->max) ? (pool->max - pool->pthread_pool->sum) : pool->step;
    for(int i=0; i<n; i++){
        new_thread = malloc(sizeof(cl_pthread));
        pthread_create(&(new_thread->pid), NULL, cl_pthread_pool_main, pool);
        pthread_detach(new_thread->pid);
        cl_link_add_back(pool->pthread_pool, cl_link_get_node(new_thread,cl_pthread,cl_pthread_node));
    }
    // printf("active:%d,sum%d\n", pool->active,pool->pthread_pool->sum);
    pthread_mutex_unlock(&(pool->pool_mutex));
    return n;
}

/**
 * 线程主函数
 * @param arg 参数
 */
static void* cl_pthread_pool_main(void* arg)
{
    cl_pthread_pool* pool = arg;
    sigset_t            set;
    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    pthread_sigmask(SIG_BLOCK, &set, NULL);

    for ( ;; ) {
        pthread_mutex_lock(&(pool->pool_mutex));
        pool->waiting--;
        while(pool->task_queue->sum == 0){
            pthread_cond_wait(&(pool->has_task), &(pool->pool_mutex));
        }
        // printf("get task\n");
        cl_base_task* task = cl_link_get_front(pool->task_queue);
        pthread_mutex_unlock(&(pool->pool_mutex));
        if(task != NULL){
            // printf("run handler\n");
            task->handler(task->self);
            // printf("run finish\n");
        }
        cl_link_add_back(pool->finish_task, task);
    }
    return NULL;
}
