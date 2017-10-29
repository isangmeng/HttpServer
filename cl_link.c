#include "cl_link.h"

/**
 * 创建一个链表对象
 * @return 链表对象地址
 */
cl_link* cl_link_create()
{
    cl_link* link = malloc(sizeof(cl_link));
    if(link == NULL){
        write(STDERR_FILENO, "malloc error", sizeof("malloc error"));
        return NULL;
    }
    pthread_mutex_init(&(link->cl_link_mutex), NULL);

    pthread_mutex_lock(&(link->cl_link_mutex));

    link->cl_link_head.next = &(link->cl_link_tail);
    link->cl_link_head.prev = &(link->cl_link_tail);

    link->cl_link_tail.next = &(link->cl_link_head);
    link->cl_link_tail.prev = &(link->cl_link_head);

    link->sum = 0;

    pthread_mutex_unlock(&(link->cl_link_mutex));

    return link;
}

/**
 * 链表的压栈操作
 * @param  link 链表对象
 * @param  node 新节点
 * @return      压栈状态
 */
int cl_link_push(cl_link* link, void* node)
{
    cl_link_node* new_node = (cl_link_node*)node;
    pthread_mutex_lock(&(link->cl_link_mutex));
    if(link)
    {
        new_node->next = link->cl_link_head.next;
        link->cl_link_head.next->prev = new_node;

        link->cl_link_head.next = new_node;
        new_node->prev = &(link->cl_link_head);
        link->sum++;
        pthread_mutex_unlock(&(link->cl_link_mutex));
        return ADD_SUCCESS;
    }else{
        pthread_mutex_unlock(&(link->cl_link_mutex));
        return ADD_FAIL;
    }
}


/**
 * 链表的出栈操作
 * @param link 链表对象
 */
void* cl_link_pop(cl_link* link)
{
    pthread_mutex_lock(&(link->cl_link_mutex));
    if(link->sum){
        cl_link_node* aim = link->cl_link_head.next;
        link->cl_link_head.next = aim->next;
        aim->next->prev = &(link->cl_link_head);
        link->sum--;
        pthread_mutex_unlock(&(link->cl_link_mutex));
        return aim;
    }else{
        pthread_mutex_unlock(&(link->cl_link_mutex));
        return NULL;
    }
}

/**
 * 对每个节点进行处理
 * @param link    链表对象
 * @param handler 处理方法
 */
void cl_link_each(cl_link* link, void** res, void* (*handler)(void* node))
{
    pthread_mutex_lock(&(link->cl_link_mutex));
    int n = link->sum;
    void* r;
    cl_link_node* p = link->cl_link_head.next;
    while(n)
    {
        r = handler(p);
        if(res != NULL)
        {
            res[n] = r;
        }
        n--;
        p = p->next;
    }
    pthread_mutex_unlock(&(link->cl_link_mutex));
}
