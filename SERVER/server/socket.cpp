#include "socket.h"

#include <arpa/inet.h>
#define Maxconn 1024
using std::cout;
using std::ostream;

Socket::Socket(){
    add=(sockaddr_in*)malloc(sizeof (sockaddr_in));
    if(sfd=socket(AF_INET, SOCK_STREAM,0)==-1){
        perror("socket: ");
        exit(1);
    }
}
Socket::Socket(){
    free(add);
}

void Socket::setup(int port, const char* add ){
    //set up listening socket
    struct sockaddr_in addr;

    bzero(&addr, sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(add);
   
    if(bind(sfd,(sockaddr *)&addr,sizeof(addr))==-1) {
        perror("bind: ");
        exit(0);
    }
    if(listen(sfd, Maxconn)==-1){
        perror("listen: ");
        exit(0);
    }

}


void Socket::setup(int port ){
    //set up listening socket with inet_addr=0;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(INADDR_ANY);
    if(bind(sfd,(sockaddr *)&addr,sizeof(addr))==-1) {
        perror("bind: ");
        exit(0);
    }
    if(listen(sfd, Maxconn)==-1){
        perror("listen: ");
        exit(0);
    }

}
int Socket::accept_client(){
    socklen_t len=sizeof(&add);
    bzero(add, len);
    int cfd=0;
    if(cfd=accept(sfd,(sockaddr *)add, &len)==-1){
        perror("accept");
        return -1;
    }
    return cfd;
}

ostream& operator<< (ostream& os,struct sockaddr_in *add){
    char buf[INET_ADDRSTRLEN]={0};
    inet_ntop(AF_INET,&(add->sin_addr),buf,INET_ADDRSTRLEN);
    os<<"IP: "<<buf<<"\n Port: "<<ntohs(add->sin_port);
    return os;
}