
#include "SocketConnect.h"
#include "EventTree.h"
#include "Queue.h"
#include "CPthreadPool.h"
#include "AcceptConnectTask.h"
#include "EventManageTask.h"

#define DEBUG 1
//服务器socke
SocketConnect* server;
//事件树
EventTree* eventTree;

//线程池
PthreadPool* pool;
