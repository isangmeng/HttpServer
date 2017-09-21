
#include "SocketConnect.h"
#include "EventTree.h"
#include "Queue.h"
// #include "PthreadPool.h"
#include "CPthreadPool.h"
// #include "ManagePthread.h"

#define DEBUG 1
//服务器socke
SocketConnect* server;
//事件树
EventTree* eventTree;

//线程池
PthreadPool* pthreadPool;
//线程管理线程
pthread_t* manage;
