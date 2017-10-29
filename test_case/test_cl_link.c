#include "../cl_link.h"
#include <stdio.h>

typedef struct test{
    cl_link_node node;
    int n;
}test;

void* handler(void *arg)
{
    printf("%d\n", ((test*)arg)->n);
    return NULL;
}
int main(){
    cl_link* link = cl_link_create();
    for(int i=0; i<10; i++){
        test* a = malloc(sizeof(test));
        a->n = i;
        cl_link_push(link, a);

        if(i == 9 || i == 6){
            cl_link_pop(link);
        }
    }
    cl_link_each(link, NULL, handler);

    return 0;
}
