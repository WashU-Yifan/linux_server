#include "server.h"
#include <sys/types.h>    

using std::vector;
using std::cout;using std::endl;
using std::unordered_map;
using std::string;
using std::shared_ptr;


Server::Server(  int portnum,int time,const char * inet,int epollsize,int thread_num):
epoll(std::make_shared<Epoll>(epollsize)),
listenSocket(Socket(portnum,inet)),
listenfd(listenSocket.getfd()),
epollWaiTime(time),
thread_pool(thread_num,epollsize)
{
     //add listen socket to the epoll
    epoll->add_fd(listenSocket.getfd(),EPOLLIN|EPOLLET);
   

}


void Server::Run(){
   
    
    vector<epoll_event> &&ready_fd=epoll->Epoll_wait(epollWaiTime);
    for(auto& event:ready_fd){
        if(event.data.fd==listenfd){
            add_client();
        }  
        else if(event.events&EPOLLIN){//Read available from client
            read_client(event.data.fd);
            //pool.add_task(event_map[fd]);
        }
        //else if(event.events&EPOLLOUT){
          //  write_client(event.data.fd);
        //}

    }
    
}

void Server::add_client(){
    int cfd=0;//cfd will be zero when listenfd is set to non-block and ET
    while ((cfd=listenSocket.accept_client())>=0){
        //by default the flag is set to EPOLLIN|EPOLLET
        cout<<"connection accepted!, Client info: \n"<<listenSocket.get_client()<<endl;
        epoll->add_fd(cfd);
    }
}


void Server::read_client(int fd){// read data from client store the http connection.
    
    char buf[1024]={0};
    int len=0,read_bytes=0;
    string data;
    //continuous read from the non-blocking fd
    while((read_bytes=read(fd, buf, sizeof(buf)))>=0){
        len+=read_bytes;
        data.append(buf,read_bytes);
    }
    if(len){
        //conn=std::move(data);
        Http http(std::move(data),fd,epoll);
        //event_map[fd]=std::move(data);
        auto fun=compose(Http::handleHttp,http);
        thread_pool.addTask(fun);
       // tpool.addTask(Task<decltype(Http::handleHttp),Http>(Http::handleHttp, std::move(http)));
    }
    
}
