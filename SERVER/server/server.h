#pragma once
#include "epoll.h"
#include "socket.h"
#include "http.h"
#include <unordered_map>
class Server{
   
    public:
        Server( int epollsize, const char * inet,int portnum,int time);
        Server( int epollsize,int portnum,int time);
        void Run();


    private:
        void add_client();
        void read_client();
        void write_client();
        
        Epoll epoll;
        Socket listenSocket;
        int listenfd,epollWaiTime;
        std::unordered_map<int, Http> event_map;
};

int socket_init(const char * inet, int portnum);