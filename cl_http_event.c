#include "cl_http_event.h"



cl_http_event* cl_http_event_create(cl_socket* client, cl_event* event)
{
    // printf("malloc base task index\n");
    cl_http_event* http_task = malloc(sizeof(cl_http_event));
    http_task->client = client;
    // http_task->task.self = http_task;
    // http_task->task.handler = handler;
    int flags = fcntl(client->fd,F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(client->fd,F_SETFL,flags);
    http_task->event_node.fd = client->fd;
    pthread_mutex_init(&(http_task->event_node.event_lock), NULL);
    http_task->event_node.task = malloc(sizeof(cl_base_task));
    ((cl_base_task*)(http_task->event_node.task))->self = http_task;
    ((cl_base_task*)(http_task->event_node.task))->handler = handler;
    ((cl_base_task*)(http_task->event_node.task))->destroy = destroy;
    http_task->event_node.events = EPOLLIN;
    http_task->event = event;
    return http_task;
}

void* handler(void* arg)
{
    // printf("malloc base task handler\n");
    cl_http_event* http_task = arg;
    http_task->event_node.events = EPOLLOUT;
    // http_task->task.handler = response;
    ((cl_base_task*)(http_task->event_node.task))->need_destroy = NEEDDIE;
    http_task->event_node.task = malloc(sizeof(cl_base_task));
    ((cl_base_task*)(http_task->event_node.task))->self = http_task;
    ((cl_base_task*)(http_task->event_node.task))->handler = response;
    char buf[1024];
    // while(1)
    // {

        int n = cl_socket_read(http_task->client, buf, sizeof(buf));
        // if(n <= 0)
        //     break;
        // printf("recv data,%s\n",http_task->client->recv_buf);
    // }
    // sleep(4);
    if(n > 0)
        cl_event_add_event(http_task->event, &(http_task->event_node));
    else
        close(http_task->event_node.fd);
    // close(http_task->event_node.fd);
    return NULL;
}
void* response(void* arg)
{
    // printf("malloc base task response\n");
    cl_http_event* http_task = arg;
    http_task->event_node.events = EPOLLIN;
    ((cl_base_task*)(http_task->event_node.task))->need_destroy = NEEDDIE;
    // http_task->task.handler = handler;
    http_task->event_node.task = malloc(sizeof(cl_base_task));
    ((cl_base_task*)(http_task->event_node.task))->self = http_task;
    ((cl_base_task*)(http_task->event_node.task))->handler = handler;
    char buf[1024];
    sprintf(buf, "HTTP/1.0 200 OK\r\nServer: Codelover\r\nConnection: Close\r\nDate:Mon,6Oct2003 13:23:42 GMT\r\nContent-Length:30\r\n\r\n<h1>codelover http server</h1>");
    // strcpy(http_task->client->send_buf, "fuck you");
    cl_socket_write(http_task->client, buf, strlen(buf));
    // sleep(5);
    // if(n > 0)
    //     cl_event_add_event(http_task->event, &(http_task->event_node));
    // else
    //     close(http_task->event_node.fd);
    // close(http_task->event_node.fd);
    // munmap(http_task->client->recv_buf, http_task->client->buffersize);
    // munmap(http_task->client->send_buf, http_task->client->buffersize);

    cl_socket_destroy(http_task->client);
    // http_task->client = NULL;
    return NULL;
}

void* destroy(void* arg)
{
    cl_http_event* http_task = arg;
    // cl_base_task* base_task = ((cl_base_task*)(http_task->event_node.task));
    // if(base_task->need_destroy == NEEDDIE && http_task)
    // {
    if(http_task->client)
        cl_socket_destroy(http_task->client);
    http_task->client = NULL;
    //     http_task->event_node.fd = -1;
    // }
    // http_task->event->
    return NULL;










}
