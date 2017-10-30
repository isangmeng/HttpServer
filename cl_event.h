#ifndef _CL_EVENT_
#define _CL_EVENT_

#include "cl_link.h"

typedef struct cl_event_node{
    int             fd;             //要监听的文件描述符
    int             events;         //监听的事件
    int             status;         //状态，是否处于监听状态
    char*           buf;            //缓冲区
    long            last_active;    //最后活跃时间
    cl_link_node    cl_event;       //所有事件链表需要
}cl_event_node;

typedef struct cl_event{
    cl_link*        cl_event_all;       //所有的事件
    int             cl_event_root;      //事件树根
}cl_event;


#endif
