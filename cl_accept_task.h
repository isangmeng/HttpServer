#ifndef _CL_ACCEPT_TASK_
#define _CL_ACCEPT_TASK_

#include "cl_base_task.h"
#include "cl_socket.h"
#include "cl_http_event.h"

typedef struct cl_accept_task{
    cl_base_task        accept_task;
    cl_socket*          server;
    cl_pthread_pool*    pool;
    cl_event*           event;
}cl_accept_task;



cl_accept_task* cl_accpet_task_create(const char* ip, const char* port, int listen_n, cl_pthread_pool* pool, cl_event* event);

void* cl_accept_task_main(void* arg);
#endif
