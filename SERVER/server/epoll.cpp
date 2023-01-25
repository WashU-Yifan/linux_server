#include "epoll.h"

#include <assert.h>  
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
using std::vector;


Epoll::Epoll(int size): SIZE(size),
epfd(epoll_create(SIZE)), 
events(std::vector<epoll_event>(SIZE))
{
    if (epfd==-1) {
        perror("epoll_create");
        exit(1);
    }
    
}

void Epoll::add_fd(int fd, unsigned int op=EPOLLIN|EPOLLET){
    struct epoll_event ev;
    bzero(&ev,sizeof(ev));
    ev.events=op;
    ev.data.fd=fd;
    fcntl(fd,F_SETFD,(fcntl(fd, F_GETFD, 0)|O_NONBLOCK));
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev)==-1)
       perror("EPOLL add: ");
} 
void Epoll::mod_fd(int fd, unsigned int op=EPOLLOUT|EPOLLET){
    struct epoll_event ev;
    bzero(&ev,sizeof(ev));
    ev.events=op;
    ev.data.fd=fd;
    if(epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev)==-1)
        perror("EPOLL mod: ");

}
void Epoll::del_fd(int fd){
    if(epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL)==-1)
        perror("EPOLL del: ");
}

vector<epoll_event> Epoll::Epoll_wait(int timeout){
    epoll_event event_store[SIZE];
    int nfd=epoll_wait(epfd,event_store,SIZE,timeout);
    return vector<epoll_event>(event_store,event_store+nfd);
}