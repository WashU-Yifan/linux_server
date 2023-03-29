#include "epoll.h"

#include <assert.h>  
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


using std::vector;


Epoll::Epoll(int size): SIZE(size),
epfd(epoll_create(SIZE))
{
    if (epfd==-1) {
        perror("epoll_create");
        exit(1);
    }
    
}

void Epoll::add_fd(int fd, unsigned int op){
    struct epoll_event ev;
    bzero(&ev,sizeof(ev));
    ev.events=op;
    ev.data.fd=fd;

    if(op&EPOLLET)//set to non-blocking iff events is set to ET
        fcntl(fd,F_SETFL,(fcntl(fd, F_GETFL, 0)|O_NONBLOCK));
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev)==-1)
       perror("EPOLL add: ");
} 


void Epoll::mod_fd(int fd, unsigned int op){
    struct epoll_event ev;
    bzero(&ev,sizeof(ev));
    ev.events=op;
    ev.data.fd=fd;
    if(epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev)==-1)
        perror("EPOLL mod: ");

}
int Epoll::del_fd(int fd){
    return epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);

}

vector<epoll_event> Epoll::Epoll_wait(int timeout){
    epoll_event event_store[SIZE];
    int nfd=epoll_wait(epfd,event_store,SIZE,timeout);
    if(nfd==-1) 
        perror("epoll_wait");
    return vector<epoll_event>(event_store,event_store+nfd);
}