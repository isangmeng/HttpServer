#ifndef _CL_HTTP_TASK_
#define _CL_HTTP_TASK_

#include "cl_socket.h"
#include "cl_event.h"
#include "cl_base_task.h"

typedef struct cl_http_event{
    cl_socket*      client;
    // cl_base_task    task;
    cl_event_node   event_node;
    cl_event*       event;
}cl_http_event;

// typedef struct cl_http_read_task{
//
// };

cl_http_event* cl_http_event_create(cl_socket* client, cl_event* event);

void* handler(void* arg);

void* response(void* arg);

void* destroy(void* arg);
#endif
