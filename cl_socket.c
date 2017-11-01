#include "cl_socket.h"

/**
 * 创建一个socket
 * 当ip和port都是NULL时返回一个初始化的socket
 * @param  ip   ip
 * @param  port 端口
 * @return      Server or client or empty
 */
cl_socket* cl_socket_create(const char* ip, const char* port)
{
    cl_socket* newsock = malloc(sizeof(cl_socket));
    if(newsock == NULL){
        return NULL;
    }
    bzero(newsock, sizeof(newsock));
    newsock->buffersize = 10240;
    if(NULL == ip && NULL == port){
        newsock->addr.sin_family = AF_INET;
        newsock->len = sizeof(newsock->addr);
    }else {
        struct hostent* host = gethostbyname(ip);
        if(host == NULL){
            printf("gethostbyname error\n");
            free(newsock);
            return NULL;
        }
        newsock->fd = socket(AF_INET, SOCK_STREAM, 0);
        if(newsock->fd < 0)
        {
            printf("server fd error\n");
            free(newsock);
            return NULL;
        }
        int mmapfd = open("/dev/zero",O_CREAT|O_RDWR,0644);
        newsock->recv_buf = mmap(0, newsock->buffersize,PROT_READ|PROT_WRITE, MAP_SHARED, mmapfd, 0);
        close(mmapfd);
        mmapfd = open("/dev/zero",O_CREAT|O_RDWR,0644);
        newsock->send_buf = mmap(0, newsock->buffersize,PROT_READ|PROT_WRITE, MAP_SHARED, mmapfd, 0);
        close(mmapfd);
        newsock->addr.sin_family = AF_INET;
        newsock->addr.sin_port = htons(atoi(port));
        newsock->addr.sin_addr =  *((struct in_addr*)host->h_addr);
        newsock->len = sizeof(newsock->addr);
    }
    return newsock;
}

/**
 * 监听sever
 * @param  cl_sockets sever
 * @param  n       监听状态
 * @return         <0 出错
 */
int cl_socket_listen(cl_socket* cl_sockets, int n)
{

    if(cl_sockets) {
        int res = listen(cl_sockets->fd, n);
        if(res < 0)
        {
            printf("%s\n", strerror(errno));
            exit(-1);
        }
        return res;
    }else {
        return -1;
    }
}

/**
 * 接受请求
 * @param  server server
 * @return        客户端
 */
cl_socket* cl_socket_accept(cl_socket* server)
{
    cl_socket* client = cl_socket_create(NULL, NULL);
    if(client == NULL)
    {
        printf("create cl_socket error\n");
        return NULL;
    }
    client->fd = accept(server->fd, (struct sockaddr *)&client->addr, &client->len);
    if(client->fd < 0){
        printf("%s\n", strerror(errno));
        free(client);
        return NULL;
    }
    int mmapfd = open("/dev/zero",O_CREAT|O_RDWR,0644);
    client->recv_buf = mmap(0, client->buffersize,PROT_READ|PROT_WRITE, MAP_SHARED, mmapfd, 0);
    close(mmapfd);
    mmapfd = open("/dev/zero",O_CREAT|O_RDWR,0644);
    client->send_buf = mmap(0, client->buffersize,PROT_READ|PROT_WRITE, MAP_SHARED, mmapfd, 0);
    close(mmapfd);
    return client;
}


/**
 * 连接服务端
 * @param  server 服务器
 * @return        连接情况
 */
int cl_socket_connect(cl_socket* server)
{
    int n = connect(server->fd, (const struct sockaddr *)&server->addr, server->len);
    if(n < 0)
    {
        printf("%s\n", strerror(errno));
        exit(-1);
    }
    return n;
}
/**
 * 绑定
 * @param  server 服务器
 * @return        bind返回值
 */
int cl_socket_bind(cl_socket* server)
{
    int opt = 1;
	setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    int n = bind(server->fd, (const struct sockaddr *)&server->addr, server->len);
    if(n < 0)
    {
        printf("bind error\n");
        exit(-1);
    }
    return n;
}

/**
 * 读取数据
 * @param  cl_sockets 要读取的socket
 * @return        read返回值
 */
int cl_socket_read(cl_socket* cl_sockets)
{
    int n;
    bzero(cl_sockets->recv_buf, cl_sockets->buffersize);
    do{
        n = read(cl_sockets->fd, cl_sockets->recv_buf, cl_sockets->buffersize);
        if(n < 0 && errno == EINTR)
        {
            continue;
        }else{
            cl_sockets->recv_buf[n] = '\0';
            break;
        }
    }while(1);
    return n;
}


/**
 * 发送数据
 * @param  cl_sockets 要发送的数据
 * @return        发情况
 */
int cl_socket_write(cl_socket* cl_sockets)
{
    int n = write(cl_sockets->fd, cl_sockets->send_buf, strlen(cl_sockets->send_buf));
    return n;
}


void cl_socket_destroy(cl_socket* cl_sockets)
{
    // if(cl_sockets->fd > 0)
    // {
    close(cl_sockets->fd);
    // }
    munmap(cl_sockets->recv_buf, cl_sockets->buffersize);
    munmap(cl_sockets->send_buf, cl_sockets->buffersize);
    printf("delete socket\n");
    free(cl_sockets);
}
