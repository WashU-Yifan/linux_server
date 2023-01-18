#pragma once
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
class EPOLL{
    public:
        epoll(int size,int sfd): SIZE(size),epfd(epoll_create(SIZE)),listenfd(sfd){
            if (epfd) cout<<"epoll create failed"<<endl;
            assert(epfd!=-1);
        };
        void add_fd(int fd, struct epoll_event &event);
        void mod_fd(int fd, struct epoll_event &event);
        void del_fd(int fd);
    private:
        const int SIZE, listenfd,epfd;

        
};