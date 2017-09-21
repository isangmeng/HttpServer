#include "../CPthreadPool.h"
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

typedef struct CTask{
    void* next;
    void* pre;
    // RunHandel
    void* (*CallHandel)(void* arg);
    void* arg;
    int n;
}CTask;

void* TaskHandle(void* arg)
{
    CTask* myself = (CTask*)arg;
    // sleep(5);
    while(1)
    ;
    printf("%d\n", myself->n);
    return NULL;
}

int main()
{
    printf("fuck\n");
    PthreadPool* pool = CreatePthreadPool();
    for(int i=0; i<1000; i++)
    {
        // printf("%d\n",i);
        CTask* p = (CTask*)malloc(sizeof(CTask));
        p->n = i;
        p->arg = p;
        p->CallHandel = TaskHandle;
        AddTask(pool, p);
    }
    while(1){
        printf("%d,%d\n", pool->ActiveNum,pool->Pool->sum);
        sleep(1);
    }

}
