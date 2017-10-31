#include "cl_accept_task.h"
#include "cl_pthread_pool.h"
#include "cl_event.h"
#include <stdio.h>



int main(int argc, char* argv[])
{
    cl_pthread_pool* pool = cl_pthread_pool_create();
    cl_event* event = cl_event_create(100, 100, pool);
    cl_accept_task* accept =  cl_accpet_task_create("127.0.0.1", "8888", 10, pool, event);
    cl_pthread_pool_add_task(pool, cl_base_task_get_base(accept, cl_accept_task, accept_task));
    cl_event_wait_event(event);
    while(1);
    return 0;
}
