#include "socket.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>



#define Maxconn 1024
using std::cout;
using std::endl;
using std::ostream;
using std::shared_ptr;




Socket::Socket(unsigned short port, const char* IPadd):
    sfd(0),
    port(port),
    sock_add(std::make_shared<sockaddr_in>()),
    client_add(std::make_shared<sockaddr_in>())
{

    setup(port,IPadd);
}

Socket::Socket(unsigned short port):
    sfd(0),
    port(port),
    sock_add(std::make_shared<sockaddr_in>()),
    client_add(std::make_shared<sockaddr_in>())
{
    setup(port,nullptr);
}

Socket::~Socket(){
    if(sfd) close(sfd);
}

void Socket::setup(unsigned short port, const char* IPadd ){
    //set up listening socket
    if((sfd=socket(AF_INET, SOCK_STREAM,0))==-1){
        perror("socket: ");
        exit(1);
    }

    std::memset(sock_add.get(), 0, sizeof(*sock_add));
    sock_add->sin_family=AF_INET;
    sock_add->sin_port=htons(port);
    if(IPadd==nullptr)
        sock_add->sin_addr.s_addr=INADDR_ANY;
    else
        sock_add->sin_addr.s_addr=inet_addr(IPadd);
    if(bind(sfd,(sockaddr *)sock_add.get(),sizeof(*sock_add))==-1) {
        perror("bind: ");
        exit(0);
    }
    if(listen(sfd, Maxconn)==-1){
        perror("listen: ");
        exit(0);
    }
}



int Socket::accept_client(){
    socklen_t len=sizeof(*client_add);
    std::memset(client_add.get(), 0, len);
    int cfd=0;
    if((cfd=accept(sfd,(sockaddr *)client_add.get(), &len))==-1){
        perror("accept");
        return -1;
    }
    return cfd;
}

ostream& operator<< (ostream& os,shared_ptr<struct sockaddr_in> add){
    char buf[INET_ADDRSTRLEN]={0};
    inet_ntop(AF_INET,&(add->sin_addr),buf,INET_ADDRSTRLEN);
    os<<"IP: "<<buf<<"\n Port: "<<ntohs(add->sin_port);
    return os;
}