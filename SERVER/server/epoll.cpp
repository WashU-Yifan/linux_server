#include "epoll.h"
using std::cout, std::endl;
void EPOLL::add_fd(int fd, struct epoll_event &event){
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,event)==-1)
       perror("EPOLL add");
}
void EPOLL::mod_fd(int fd, struct epoll_event &event){
   if(epoll_ctl(epfd,EPOLL_CTL_MOD,fd,event)==-1)
        perror("EPOLL mod");

}
void EPOLL::del_fd(int fd){
    if(epoll_ctl(epfd,EPOLL_CTL_DEL,fd,event)==-1)
        perror("EPOLL del");
}

