#include "../CPthreadPool.h"
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include "../SocketConnect.h"
#include "../AcceptConnectTask.h"

int main()
{
    SocketConnect* server = InitSocket(8888);
    AcceptConnectTask* task = CreatAcceptConnectTask(server);
    PthreadPool* pool = CreatePthreadPool();
    AddTask(pool, task);

    while(1){
        printf("%d,%d\n", pool->ActiveNum,pool->Pool->sum);
        sleep(1);
    }
    return 0;
}
