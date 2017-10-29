#include "../cl_link.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct test{
    cl_link_node node;
    int n;
}test;

void* handler(void *arg)
{
    int* r = malloc(sizeof(int));
    *r = rand()%10;
    printf("%d\n", ((test*)arg)->n);
    return r;
}
int main(){
    void* res[10];
    cl_link* link = cl_link_create();
    for(int i=0; i<10; i++){
        test* a = malloc(sizeof(test));
        a->n = i;
        cl_link_push(link, a);

        // if(i == 9 || i == 6){
        //     cl_link_pop(link);
        // }
    }
    cl_link_each(link, res, handler);
    for(int i=0; i<10; i++){
        printf("%d\n", *(int*)(res[i]));
    }
    return 0;
}
