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
#include <pthread.h>




typedef struct cl_socket{
    int                 fd;             //套接字描述符
    // char*               send_buf;       //发送缓存
    // char*               recv_buf;       //接收缓存
    socklen_t           len;            //地址长度
    struct sockaddr_in  addr;           //地址
    // int                 buffersize;     //缓冲区大小
}cl_socket;

/**
 * 创建一个socket
 * 当ip和port都是NULL时返回一个初始化的socket
 * @param  ip   ip
 * @param  port 端口
 * @return      Server or client or empty
 */
cl_socket* cl_socket_create(const char* ip, const char* port);

/**
 * 监听sever
 * @param  cl_sockets sever
 * @param  n       监听状态
 * @return         <0 出错
 */
int cl_socket_listen(cl_socket* cl_sockets, int n);

/**
 * 接受请求
 * @param  server server
 * @return        客户端
 */
cl_socket* cl_socket_accept(cl_socket* server);


/**
 * 连接服务端
 * @param  server 服务器
 * @return        连接情况
 */
int cl_socket_connect(cl_socket* server);

/**
 * 绑定
 * @param  server 服务器
 * @return        bind返回值
 */
int cl_socket_bind(cl_socket* server);

/**
 * 读取数据
 * @param  cl_sockets 要读取的socket
 * @return        read返回值
 */
int cl_socket_read(cl_socket* cl_sockets, char* buf, size_t len);


/**
 * 发送数据
 * @param  cl_sockets 要发送的数据
 * @return        发情况
 */
int cl_socket_write(cl_socket* cl_sockets, const char* buf, size_t len);


void cl_socket_destroy(cl_socket* cl_sockets);

#endif
