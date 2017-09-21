#include "SocketConnect.h"
#include "global.h"
#include <unistd.h>
#include <sys/fcntl.h>
/**
 * 初始化服务器端socket
 * @param  port 端口
 * @return      server socket
 */
SocketConnect* InitSocket(uint16_t port)
{
    SocketConnect* server = (SocketConnect*)malloc(sizeof(SocketConnect));
    if(server == NULL)
    {
        printf("malloc error:%s\n", __func__);
        exit(1);
    }
    server->ServerFd = socket(AF_INET, SOCK_STREAM, 0);
    if(server->ServerFd < -1)
    {
        printf("socket error%s\n", strerror(server->ServerFd));
        exit(1);
    }
    server->ServerAddr.sin_family = AF_INET;
    server->ServerAddr.sin_port = htons(port);
    server->ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int n = bind(server->ServerFd, (struct sockaddr *)&(server->ServerAddr), sizeof(server->ServerAddr));
    if(n < 0)
    {
        printf("bind error%s\n" ,strerror(n));
        exit(1);
    }
    listen(server->ServerFd, 10);
    return server;
}

/**
 * 关闭服务器端socket
 * @param server server socket
 */
void CloseSocket(SocketConnect* server)
{
    shutdown(server->ServerFd, SHUT_WR);
    free(server);
}
