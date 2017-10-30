#include "../cl_link.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct test{

    int n;
    cl_link_node node;
}test;

void* handler(void *arg)
{
    test* t = cl_link_get_data(arg, test, node);
    printf("%p\n",t );
    free(t);

    int* r = malloc(sizeof(int));
    *r = rand()%10;
    // printf("%d\n", cl_link_get_data(arg, test, node)->n);
    return r;
}
int condition(void* node, void* key){
    test* t = cl_link_get_data(node, test, node);
    if(t->n == (int)key)
        return CANFIND;
    else
        return NOTFIND;
}
int main(){
    void* res[10];
    cl_link* link = cl_link_create();
    for(int i=0; i<10; i++){
        test* a = malloc(sizeof(test));
        printf("%p\n",a );
        a->n = i;
        cl_link_add_back(link, cl_link_get_node(a,test,node));

        // if( i == 9){
        //     cl_link_get_front(link);
        // }
    }

    test* t = cl_link_find(link, (void *)5, condition);
    if(t != NULL)
    printf("%d\n", (cl_link_get_data(t,test,node))->n);
    cl_link_each(link, res, handler);
    return 0;
}
