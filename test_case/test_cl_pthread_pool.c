#include "../cl_pthread_pool.h"
#include <stdio.h>



typedef struct task{
    int n;
    cl_base_task base_task;
}task;

void* handler(void* arg)
{
    task* t = arg;
    // printf("%d\n", t->n);

    sleep(4);
    return NULL;
}
void* show(void* arg){
    cl_base_task* t = arg;
    task* tt = t->self;
    printf("%d\n", tt->n);
    return NULL;
}
int main()
{
    cl_pthread_pool* pool = cl_pthread_pool_create();
    int n = 30;
    cl_pthread_pool_add_pthread(pool);
    while(n--){
        task* t = malloc(sizeof(task));
        t->n = n;
        t->base_task.handler = handler;
        t->base_task.self = t;
        t->base_task.need_destroy = 0;
        cl_pthread_pool_add_task(pool, cl_base_task_get_base(t, task, base_task));
    }

    while(1)
    {
        cl_link_each(pool->finish_task, NULL, show);
        printf("\n");
        sleep(1);
    }
    return 0;
}
