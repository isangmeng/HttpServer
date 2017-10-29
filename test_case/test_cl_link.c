#include "../cl_link.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct test{

    int n;
    cl_link_node node;
}test;

void* handler(void *arg)
{
    int* r = malloc(sizeof(int));
    *r = rand()%10;
    printf("%d\n", cl_link_get_data(arg, test, node)->n);
    return r;
}
int main(){
    void* res[10];
    cl_link* link = cl_link_create();
    for(int i=0; i<10; i++){
        test* a = malloc(sizeof(test));
        a->n = i;
        cl_link_add_back(link, cl_link_get_node(a,test,node));

        if( i == 9){
            cl_link_get_front(link);
        }
    }
    cl_link_each(link, res, handler);
    // for(int i=0; i<10; i++){
    //     printf("%d\n", *(int*)(res[i]));
    // }
    return 0;
}
