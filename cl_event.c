#include "cl_event.h"

/**
 * 创建事件对象
 * @return 事件对象
 */
cl_event* cl_event_create(int n, int m, cl_pthread_pool* pool)
{
    cl_event* event = malloc(sizeof(cl_event));
    if(event == NULL)
    {
        write(STDERR_FILENO, "malloc error", sizeof("malloc error"));
        return NULL;
    }
    event->cl_event_all = cl_link_create();
    event->cl_event_root = epoll_create(n>0 ? n : 1024);
    event->active_max = m>0 ? m : 100;
    event->active_event = malloc(sizeof(cl_event_node)*event->active_max);
    event->pool = pool;
    return event;
}



/**
 * 添加事件
 * @param  event     事件对象
 * @param  new_event 新事件
 * @return           添加情况
 */
int cl_event_add_event(cl_event* event, cl_event_node* new_event)
{
    // printf("try add event\n");
    pthread_mutex_lock(&(new_event->event_lock));
    if(new_event->status == 0)
    {
        struct epoll_event tmp;
        new_event->last_active = time(NULL);
        new_event->status = 1;
        tmp.data.ptr = new_event;
        tmp.events = new_event->events;
        epoll_ctl(event->cl_event_root, EPOLL_CTL_ADD, new_event->fd, &tmp);
        cl_link_add_back(event->cl_event_all, cl_link_get_node(new_event, cl_event_node, cl_event));
        // printf("add event ok\n");
    }
    pthread_mutex_unlock(&(new_event->event_lock));
    return ADDEVENTSUCCESS;
}


/**
 * 从监听事件树上删除一个节点
 * @param  event      事件树
 * @param  delete_aim 删除对象
 * @return            删除状态
 */
int cl_event_delete_event(cl_event* event, cl_event_node* delete_aim)
{
    // printf("try delete event\n");
    pthread_mutex_lock(&(delete_aim->event_lock));
    if(delete_aim->status == 1)
    {
        delete_aim->status = 0;
        epoll_ctl(event->cl_event_root, EPOLL_CTL_DEL, delete_aim->fd, NULL);
        // printf("delete event ok\n");
    }
    pthread_mutex_unlock(&(delete_aim->event_lock));
    return DELETESUCCESS;
}

/**
 * 等待事件发生
 * @param event 事件树
 */
void cl_event_wait_event(cl_event* event)
{
    int nready;
    while(1)
    {
        pthread_mutex_lock(&(event->cl_event_mutex));
        nready = epoll_wait(event->cl_event_root, event->active_event, event->active_max, -1);
        if(errno == EINTR)
        {
            pthread_mutex_unlock(&(event->cl_event_mutex));
            continue;
        }
        for(int j=0; j<nready; j++)
        {
            // printf("添加任务\n");
            // cl_base_task_get_base(event->active_event[j].data.ptr, cl_event_node, task)
            // event->active_event[j].data.ptr
            cl_event_delete_event(event, event->active_event[j].data.ptr);
            cl_pthread_pool_add_task(event->pool, ((cl_event_node*)(event->active_event[j].data.ptr))->task);

            // cl_link_add_back(pool->task_queue, event->active_event[j].data.ptr);
        }
        // for(int j=0; j<nready; j++)
        // {
        //     cl_base_task* task = cl_base_task_get_base(event->active_event[j].data.ptr, cl_event_node, task);
        //     pthread_mutex_lock(&(task->task_mutex));
        //     cl_event_delete_event(event, event->active_event[j].data.ptr);
        //     pthread_mutex_unlock(&(task->task_mutex));
        // }
        nready = 0;
        pthread_mutex_unlock(&(event->cl_event_mutex));
    }
}
