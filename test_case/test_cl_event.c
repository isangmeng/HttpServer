#include "../cl_event.h"
#include <stdio.h>
#include <string.h>


typedef struct event{
    cl_event_node event_node;
    char            buf[1024];
    cl_base_task    base_task;
}event;

// typedef struct task{
//     cl_base_task    base_task;
//
// }task;
cl_event* cl_event1;
void* handler(void* arg)
{
    event* e = arg;
    printf("%s\n", e->buf);
    printf("has data\n");
    cl_event_delete_event(cl_event1, cl_event_get_node(arg, event, event_node));
    return  NULL;
}


int main()
{
    cl_pthread_pool* pool = cl_pthread_pool_create();
    cl_event1 = cl_event_create(100, 100, pool);
    event a;
    // a.event_node.fd = STDIN_FILENO;
    strcpy(a.buf, "aaa");
    a.event_node.events = EPOLLIN;
    a.base_task.self = &a;
    a.event_node.task = &a.base_task;
    a.base_task.self = &a;
    a.base_task.handler = handler;
    // a.event_node
    cl_event_add_event(cl_event1, cl_event_get_node(&a, event, event_node));
    cl_event_wait_event(cl_event1);
    while(1)
    {
        ;
    }
    return 0;
}
