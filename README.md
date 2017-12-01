# HttpServer
<p>此版本为<a href="https://github.com/lovercode/WebServer.git">WebServer</a>的改进版，之前的WebServer写的太挫了（虽然这个也很挫）</p>

### 基本思路
<img src="img/server.png">

### 目录说明
    cl_accept_task.h        接受请求的任务，直接添加到线程池的任务队列
    cl_base_task.h          用于线程池的任务队列调用的任务对象，类似基类的概念，其他任务（如接受请求的任务）想要能被线程池调用，就必须包含此对象
    cl_event.h              事件对象，epoll实现，如果一个事件想要被此对象监听，需要包含其中的cl_event_node结构体，当此对象中监听的事件发生，会把事件节点cl_event_node的任务指针指向的任务添加到线程池
    cl_http_event.h         http处理事件和处理任务的封装
    cl_link.h               通用链表
    cl_pthread_pool.h       线程池，所有线程阻塞在一个条件变量，等待任务到来，当任务到来后会调用任务的回调函数
    cl_socket.h             对socket的封装，每个socket会有两个共享内存映射，主要是作为发送数据和接受数据的缓冲区
    main.h                  主函数


### ab性能测试：

    codelover@codelover ~$ ab -c 1000 -n 1000000 http://127.0.0.1:7777/index.html
    This is ApacheBench, Version 2.3 <$Revision: 1796539 $>
    Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
    Licensed to The Apache Software Foundation, http://www.apache.org/

    Benchmarking 127.0.0.1 (be patient)
    Completed 100000 requests
    Completed 200000 requests
    Completed 300000 requests
    Completed 400000 requests
    Completed 500000 requests
    Completed 600000 requests
    Completed 700000 requests
    Completed 800000 requests
    Completed 900000 requests
    Completed 1000000 requests
    Finished 1000000 requests


    Server Software:        Codelover
    Server Hostname:        127.0.0.1
    Server Port:            7777

    Document Path:          /index.html
    Document Length:        30 bytes

    Concurrency Level:      1000
    Time taken for tests:   108.860 seconds
    Complete requests:      1000000
    Failed requests:        0
    Total transferred:      138000000 bytes
    HTML transferred:       30000000 bytes
    Requests per second:    9186.13 [#/sec] (mean)
    Time per request:       108.860 [ms] (mean)
    Time per request:       0.109 [ms] (mean, across all concurrent requests)
    Transfer rate:          1237.97 [Kbytes/sec] received

    Connection Times (ms)
                  min  mean[+/-sd] median   max
    Connect:        0   67 186.8     36    7175
    Processing:     0   41  19.1     40    1026
    Waiting:        0   29  17.7     27    1018
    Total:          0  108 189.9     77    7204

    Percentage of the requests served within a certain time (ms)
      50%     77
      66%     85
      75%     91
      80%     95
      90%    107
      95%    120
      98%   1083
      99%   1104
     100%   7204 (longest request)



### 目前情况：
    目前还没实现http协议，用ab工具测试了下并发情况如上
