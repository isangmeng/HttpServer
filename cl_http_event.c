#include "cl_http_event.h"



cl_http_event* cl_http_event_create(cl_socket* client, cl_event* event)
{
    cl_http_event* http_task = malloc(sizeof(cl_http_event));
    http_task->client = client;
    http_task->task.self = http_task;
    http_task->task.handler = handler;
    int flags = fcntl(client->fd,F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(client->fd,F_SETFL,flags);
    http_task->event_node.fd = client->fd;
    pthread_mutex_init(&(http_task->event_node.event_lock), NULL);
    http_task->event_node.task = &(http_task->task);
    http_task->event_node.events = EPOLLIN;
    http_task->event = event;
    return http_task;
}

void* handler(void* arg)
{
    cl_http_event* http_task = arg;
    http_task->event_node.events = EPOLLOUT;
    http_task->task.handler = response;

    // while(1)
    // {
        int n = cl_socket_read(http_task->client);
        // if(n <= 0)
        //     break;
        printf("recv data,%s\n",http_task->client->recv_buf);
    // }
    if(n > 0)
        cl_event_add_event(http_task->event, &(http_task->event_node));
    else
        shutdown(http_task->client->fd, SHUT_RDWR);
    return NULL;
}
void* response(void* arg)
{
    cl_http_event* http_task = arg;
    http_task->event_node.events = EPOLLIN;
    http_task->task.handler = handler;
    sprintf(http_task->client->send_buf, "HTTP/1.0 200 OK\r\nServer:Codelover\r\nConnection: Close\r\nDate:Mon,6Oct2003 13:23:42 GMT\r\nContent-Length:30\r\n\r\n<h1>codelover http server</h1>");
    // strcpy(http_task->client->send_buf, "fuck you");
    int n = cl_socket_write(http_task->client);
    if(n > 0)
        cl_event_add_event(http_task->event, &(http_task->event_node));
    else
        shutdown(http_task->client->fd, SHUT_RDWR);
    return NULL;
}
