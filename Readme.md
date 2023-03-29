SERVER:
    Main thread receives incoming connections and read all data by itself. Then handle the job of request processing and write back to other threads.

    
    ethan@ubuntu:~$ webbench -c 10 -t 5 http://192.168.92.128:3451/

    Speed=2072868 pages/min, 5942187 bytes/sec.
    Requests: 172739 susceed, 0 failed.


    ethan@ubuntu:~$ webbench -c 1000 -t 5 http://192.168.92.128:3451/

    Speed=3322440 pages/min, 9518790 bytes/sec.
    Requests: 276870 susceed, 0 failed.

    Max QPS: 55174
SERVER_Copy:
    Now the main thread is only responsible for accepting client connections. All 
    client data will be read, processed, and sent back in working threads. Have changed the Http object to shared_ptr<Http> in composing the functor to reduce memory usage. However, as now the program's through put is heavily relied on the synchronization of threads: 
        main threads push read task onto the que, 
        working threads popped the read tasks, finished the request,and push the write request onto the que.
        working threads popped the write tasks from the que and send it back to client.
    The through put is reduced by a significant amount. 

    ethan@ubuntu:~$ webbench -c 10 -t 5 http://192.168.92.128:3456/

    Speed=1709268 pages/min, 4899901 bytes/sec.
    Requests: 142439 susceed, 0 failed.

    ethan@ubuntu:~$ webbench -c 1000 -t 5 http://192.168.92.128:3456/

    Speed=3163332 pages/min, 9067118 bytes/sec.
    Requests: 263611 susceed, 0 failed.

    Max QPS: 52722
