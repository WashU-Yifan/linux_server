#include "server.h"
#include <sys/types.h>    

using std::vector;
using std::cout;using std::endl;
using  std::unordered_map;

Server::Server( int epollsize, const char * inet,int portnum,int time):
epoll(epollsize),
listenSocket(Socket()),
epollWaiTime(time)
{
    //set up listening socket
    listenSocket.setup(portnum,inet);
     //add listen socket to the epoll
    epoll.add_fd(listenSocket.getfd(),EPOLLIN|EPOLLET);
   

}
Server::Server( int epollsize, int portnum,int time):
epoll(epollsize),
listenSocket(Socket()),
epollWaiTime(time)
{
    //set up listening socket
    listenSocket.setup(portnum);
     //add listen socket to the epoll
    epoll.add_fd(listenSocket.getfd(),EPOLLIN|EPOLLET);
   

}

void Server::Run(){
   
    
    vector<epoll_event> ready_fd=epoll.Epoll_wait(epollWaiTime);
    for(auto& event:ready_fd){
        if(event.data.fd==listenfd){
            add_client();
        }
        else if(event.events&EPOLLIN){//Read available from client
            read_client(event.fd);
            //pool.add_task(event_map[fd]);
        }
        else if(event.events&EPOLLOUT){
            write_client(event.fd);
        }

        }
    }
}

void Server::add_client(){
    struct sockaddr_in *addr;
    int cfd=0;//cfd will be zero when listenfd is set to non-block and ET
    while (cfd=listenSocket.accept_client()>=0){
        addr=listenSocket.get_client();
        //by default the flag is set to EPOLLIN|EPOLLET
        cout<<"connection accepted!, Client info: \n"<<addr<<endl;
        epoll.add_fd(cfd);
    }
}


void Server::read_client(int fd){// read data from client store the http connection.
    Http conn;
    char buf[1024]={0};
    int len=0,read_bytes=0;
    string data;
    while(read_bytes=read(fd, buf, strlen(buf))>=0){
        len+=read_bytes;
        data.append(data,read_bytes);
    }
    if(len){
        conn(std::move(data));
    }
    event_map[fd]=conn;
}
void Server::write_client(){

}