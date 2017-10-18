#include "../linkTab.h"
#include "../CPthreadPool.h"
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
plinkTab links;
// typedef struct CTask{
//     void* next;
//     void* pre;
//     // RunHandel
//     void* (*CallHandel)(void* arg);
//     void* arg;
//     int n;
// }CTask;

typedef struct Node{
    struct linknode* next;
	void (*showWay)(void* x);

    int n;
}Node;


// void* TaskHandle(void* arg)
// {
//     int n = 10;
//     while(n--)
//     {
//         Node* nx = malloc(sizeof(Node));
//         nx->n = n;
//         AddLinkTabNode(links, nx);
//         DeleteLinkTabNode(links, nx);
//     }
//     sleep(10);
//     return NULL;
// }
// plinkTab links;
//
void* show(plinkTab links, void* arg, void* other)
{
    Node* n = (Node*)arg;
    printf("%d\n", n->n);
    return NULL;
}
int main()
{
    links = CreatLinkTab();
    int n = 1;
    while (n--) {
        Node* p = (Node*)malloc(sizeof(Node));
        p->n = n;
        AddLinkTabNode(links, p);
    }
    Node* p = (Node*)links->pHead;

    DeleteLinkTabNode(links, p);
    p = (Node*)links->pHead;

    DeleteLinkTabNode(links, p);
    n=10;
    while (n--) {
        Node* p = (Node*)malloc(sizeof(Node));
        p->n = n;
        AddLinkTabNode(links, p);
    }
    while(1)
    {
        ForeachDeal(links, NULL, show);
        sleep(1);
    }
    // PthreadPool* pool = CreatePthreadPool();
    // for(int i=0; i<200; i++)
    // {
    //     // printf("%d\n",i);
    //     CTask* p = (CTask*)malloc(sizeof(CTask));
    //     p->n = i;
    //     p->arg = p;
    //     p->CallHandel = TaskHandle;
    //     AddTask(pool, p);
    // }

    // while(1)
    // {
    //     ForeachDeal(links, NULL, show);
    //     sleep(1);
    // }
    return 0;
}
