#pragma once
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
class Epoll{// Encapsulate the complicate epoll interface.
    public:
        Epoll(int size);

        // Add a file descriptor with specified events (default: EPOLLIN|EPOLLET|EPOLLONESHOT)
        //EPOLLET: edge-trigger, will notify only if new data/connection is ready
        //EPOLLONESHOT: will rm the fd from epoll events to avoid concurrency issue.
       // EPOLLERR|EPOLLHUP detect whether client socket goes wrong or is closed.
        void add_fd(int fd, unsigned int op=EPOLLIN|EPOLLET|EPOLLERR|EPOLLHUP);
        // Modify the events associated with a file descriptor
        void mod_fd(int fd, unsigned int op);
        // Delete a file descriptor from the epoll instance
        int del_fd(int fd);

        std::vector<epoll_event> Epoll_wait(int timeout);
    private:
        const int SIZE, epfd;


        
};