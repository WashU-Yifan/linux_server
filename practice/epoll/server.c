#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/epoll.h>
#define ARGC 2
#define EPOLL_SIZE 10
#define EPOLL_TIME 100
#define BUF_LEN 1024
#define CHECKFLAG(event, flag) if(event->mask& flag) printf("%s\n", #flag);


int main(int argc, const char ** argv){
   // printf("Hello world \n");
	if(argc !=ARGC){
		printf("usage: ./server path \n");
		exit(0);
    }
    
    //printf("Hello world \n");
    int fd=inotify_init1(0);
    
    if(fd==-1){
        printf("inotify initiate failed , fd:%d, path %s \n", fd, argv[1]);
        exit(0);
    }
    else{
        printf("inotify successfully initiate, fd:%d \n", fd);
    }
    
    //printf("Hello world \n");
    int wd;
    if(wd=inotify_add_watch(fd,argv[1],IN_ALL_EVENTS)==-1){
        perror("add watch failed on  path\n ");
        exit(0);
    }
    else{
        printf("inotify event added on fd %d, and path %s\n", fd, argv[1]);
    }
    
    //printf("Hello world \n");
    int epfd=epoll_create(EPOLL_SIZE);
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd=fd;
    ev.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev );
    
    
    while(1){
        struct epoll_event *event;
        int num=0;
        num=epoll_wait(epfd, event,EPOLL_SIZE,EPOLL_TIME);
  
        for(int j=0;j<num;++j){//event was monitored on the inotify instance
            ssize_t len=0,i=0;
            char buf[BUF_LEN]__attribute__((aligned(sizeof(struct inotify_event)))); 
            memset(buf,0,BUF_LEN);
            len=read(event[j].data.fd,buf,BUF_LEN);
            while(i<len){
                struct inotify_event * ievent=(struct inotify_event *) &buf[i];
                
                printf("wd %d, mask %d, cookie %d, len %d, masks:\n",
                ievent->wd,ievent->mask, ievent->cookie,
                ievent->len);
                
                if(ievent->len){
                    
                    printf("name %s\n", ievent->name);
                    i+=ievent-> len;
                }
                CHECKFLAG(ievent,IN_ACCESS);
                CHECKFLAG(ievent,IN_MODIFY);
                CHECKFLAG(ievent,IN_ATTRIB);
                CHECKFLAG(ievent,IN_CLOSE_WRITE);
                CHECKFLAG(ievent,IN_CLOSE_NOWRITE);
                CHECKFLAG(ievent,IN_OPEN);
                CHECKFLAG(ievent,IN_MOVED_FROM);
                CHECKFLAG(ievent,IN_MOVED_TO);
                CHECKFLAG(ievent,IN_CREATE);
                CHECKFLAG(ievent,IN_DELETE);
                CHECKFLAG(ievent,IN_DELETE_SELF);
                CHECKFLAG(ievent,IN_MOVE_SELF);
                CHECKFLAG(ievent,IN_ALL_EVENTS);
                CHECKFLAG(ievent,IN_CLOSE);
                CHECKFLAG(ievent,IN_MOVE);
                i+=sizeof (struct inotify_event);
            }
        }
    }
  
    
    return 0;
}
