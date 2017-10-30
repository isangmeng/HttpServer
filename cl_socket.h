#ifndef _CL_SOCKET_
#define _CL_SOCKET_
#include "cl_event.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/fcntl.h>




typedef struct cl_socket{
    cl_event_node       event_node;     //事件监听需要
    char*               buf;            //缓存
    void*               task;           //需要执行的任务
    socklen_t           len;            //地址长度
    struct sockaddr_in  addr;           //地址
}cl_socket;


// cl_socket





















#endif
