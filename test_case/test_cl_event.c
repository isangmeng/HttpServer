#include "../cl_event.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

typedef struct event{
    cl_event_node event_node;
    char            buf[1024];
    cl_base_task    base_task;
}event;


cl_event* cl_event1;
void* handler(void* arg)
{
    event* e = arg;
    int n = read(e->event_node.fd, e->buf, 1024);
    printf("has data,%d\n",n);

    cl_event_add_event(cl_event1, cl_event_get_node(arg, event, event_node));
    printf("add ok\n");
    sleep(3);
    return  NULL;
}


int main()
{
    cl_pthread_pool* pool = cl_pthread_pool_create();
    cl_event1 = cl_event_create(100, 100, pool);
    event a;
    // a.event_node.fd = STDIN_FILENO;
    strcpy(a.buf, "aaa");
    a.event_node.fd = STDIN_FILENO;
    a.event_node.events = EPOLLIN;
    int flags = fcntl(a.event_node.fd,F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(a.event_node.fd,F_SETFL,flags);
    pthread_mutex_init(&(a.event_node.event_lock), NULL);
    a.base_task.self = &a;
    a.event_node.status = 0;
    a.event_node.task = &(a.base_task);
    a.base_task.self = &a;
    a.base_task.handler = handler;

    cl_event_add_event(cl_event1, cl_event_get_node(&a, event, event_node));
    cl_event_wait_event(cl_event1);

    return 0;
}
