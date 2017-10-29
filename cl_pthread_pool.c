#include "cl_pthread_pool.h"
#include "cl_link.h"
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
    pool->step = 10;
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
int cl_pthread_pool_add_task(cl_pthread_pool* pool, void* task);

/**
 * 添加线程
 * @param  pool 线程池
 * @return      添加数量
 */
int cl_pthread_pool_add_pthread(cl_pthread_pool* pool)
{
    pthread_mutex_lock(&(pool->pool_mutex));
    int n = ((pool->pthread_pool->sum + pool->step) > pool->max) ? (pool->max - pool->pthread_pool->sum) : pool->step;
    for(int i=0; i<n; i++){
        cl_pthread* thread = malloc(sizeof(cl_pthread));
        pthread_create(&(thread->pid), NULL, cl_pthread_pool_main, pool);
        // cl_link_add_back(pool->pthread_pool, thread);
        cl_link_add_back(pool->pthread_pool, (cl_link_get_node(thread, cl_pthread, cl_pthread_node)));
        // cl_link_add_back(link, cl_link_get_node(a,test,node));
    }
    pthread_mutex_unlock(&(pool->pool_mutex));
    return n;
}

/**
 * 线程主函数
 * @param arg 参数
 */
void* cl_pthread_pool_main(void* arg)
{
    return NULL;
}
