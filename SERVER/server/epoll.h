#pragma once
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
class Epoll{// Encapsulate the complicate epoll interface.
    public:
        Epoll(int size);
        void add_fd(int fd, unsigned int op=EPOLLIN|EPOLLET);//bydefault fd are set to non-blocking
        void mod_fd(int fd, unsigned int op);
        void del_fd(int fd);
        std::vector<epoll_event> Epoll_wait(int timeout);
    private:
        const int SIZE, epfd;
        std::vector<epoll_event> events;

        
};