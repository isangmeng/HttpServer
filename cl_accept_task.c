#include "cl_accept_task.h"


cl_accept_task* cl_accpet_task_create(const char* ip, const char* port, int listen_n, cl_pthread_pool* pool, cl_event* event)
{
    cl_accept_task* accept_task = malloc(sizeof(cl_accept_task));
    accept_task->server = cl_socket_create(ip, port);
    accept_task->accept_task.self = accept_task;
    accept_task->accept_task.need_destroy = 0;
    accept_task->pool = pool;
    accept_task->event = event;
    accept_task->accept_task.handler = cl_accept_task_main;
    cl_socket_bind(accept_task->server);
    cl_socket_listen(accept_task->server, listen_n);
    return accept_task;
}

void* cl_accept_task_main(void* arg)
{
    cl_accept_task* accept_task = arg;
    while(1)
    {
        // printf("client start\n");
        cl_socket* client = cl_socket_accept(accept_task->server);
        cl_http_event* task = cl_http_event_create(client, accept_task->event);
        cl_event_add_event(accept_task->event, &task->event_node);
    }


    return NULL;
}
