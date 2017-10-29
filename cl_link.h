#ifndef _CL_LINK_H
#define _CL_LINK_H

#include <pthread.h>

#define ADD_SUCCESS 0
#define ADD_FAIL -1
#define DELETE_SUCCESS 0
#define SELETE_FAIL -1

typedef struct cl_link_node cl_link_node;
typedef struct cl_link cl_link;



/**
 * 链表节点，其他结构体需要使用链表数据结构，只需包含此节点
 * @return
 */
typedef struct cl_link_node{
    cl_link_node* next;
    cl_link_node* prev;
}cl_link_node;

/**
 * 通用链表对象
 * @return
 */
typedef struct cl_link{
    pthread_mutex_t cl_link_mutext;     //链表锁
    cl_link_node cl_link_head;          //链表头
    cl_link_node cl_link_tail;          //链表尾
    int sum;                            //节点数
}cl_link;

/**
 * 创建一个链表对象
 * @return 链表对象地址
 */
cl_link* cl_link_create();

/**
 * 链表的压栈操作
 * @param  link 链表对象
 * @param  node 新节点
 * @return      压栈状态
 */
int cl_link_push(cl_link* link, void* node);

/**
 * 链表的出栈操作
 * @param link 链表对象
 */
void* cl_link_pop(cl_link* link);




#endif
