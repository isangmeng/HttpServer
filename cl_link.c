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
 * 对每个节点进行操作
 * @param link    链表对象
 * @param res     返回值
 * @param handler 处理函数
 */
void cl_link_each(cl_link* link, void* res[], void* (*handler)(void* node))
{
    pthread_mutex_lock(&(link->cl_link_mutex));
    int n = link->sum;
    void* r;

    cl_link_node* p = link->cl_link_head.next;
    cl_link_node* todo = p;
    while(n)
    {
        todo = p;
        p = p->next;
        r = handler(todo);
        if(res != NULL)
        {
            res[link->sum-n] = r;
        }
        n--;
    }
    pthread_mutex_unlock(&(link->cl_link_mutex));
}


/**
 * 队尾添加元素
 * @param  link 队列对象
 * @param  node 新节点
 * @return      添加状态
 */
int cl_link_add_back(cl_link* link, void* node)
{
    cl_link_node* new_node = (cl_link_node*)node;
    pthread_mutex_lock(&(link->cl_link_mutex));
    if(link)
    {
        new_node->next = &(link->cl_link_tail);
        new_node->prev = link->cl_link_tail.prev;

        link->cl_link_tail.prev->next = new_node;
        link->cl_link_tail.prev = new_node;
        link->sum++;
        pthread_mutex_unlock(&(link->cl_link_mutex));
        return ADD_SUCCESS;
    }else{
        pthread_mutex_unlock(&(link->cl_link_mutex));
        return ADD_FAIL;
    }
}

/**
 * 队头获取元素
 * @param  link 队列对象
 * @return      取得的元素
 */
void* cl_link_get_front(cl_link* link)
{
    return cl_link_pop(link);
}

/**
 * 根据key查找节点
 * @param link      链表对象
 * @param key       关键字
 * @param condition 条件
 */
void* cl_link_find(cl_link* link, void* key, int (*condition)(void* node, void* key))
{
    pthread_mutex_lock(&(link->cl_link_mutex));
    int n = link->sum;
    cl_link_node* p = link->cl_link_head.next;
    while(n)
    {
        if(condition(p, key) == CANFIND)
        {
            pthread_mutex_unlock(&(link->cl_link_mutex));
            return p;
        }

        p = p->next;
        n--;
    }
    pthread_mutex_unlock(&(link->cl_link_mutex));
    return NULL;
}
