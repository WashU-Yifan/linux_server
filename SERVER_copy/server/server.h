#pragma once
#include "epoll.h"
#include "socket.h"
#include "../pool/tpool.h"
#include "../Http/Http.h"
#include <unordered_map>
#include <memory>
#include <functional>



class Server{

    public:
        Server( int portnum,int time,const char * inet="0.0.0.0",
          int max_request_size=4096,int thread_num=4);
        void Run();


    private:
        void add_client();
 
        //void write_client(int);
        
        std::shared_ptr<Epoll> epoll;
        Socket listenSocket;
        int listenfd,epollWaiTime;
        std::unordered_map<int, Http> event_map;
        Tpool<std::function<std::shared_ptr<Http>()>> thread_pool;


};
