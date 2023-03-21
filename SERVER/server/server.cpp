#include "server.h"
#include <sys/types.h>    

using std::vector;
using std::cout;
using std::endl;
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
   
    while(1){
        vector<epoll_event> ready_fd=epoll->Epoll_wait(epollWaiTime);
        cout<<ready_fd.size()<<" fds are ready"<<endl;
        for(auto& event:ready_fd){
            if(event.data.fd==listenfd){
                add_client();
            }  
            else if(event.events&(EPOLLHUP|EPOLLERR)){
                if(epoll->del_fd(event.data.fd)!=-1)
                    close(event.data.fd);
                else
                    perror("EPOLL del: ");
            }
            else if(event.events&EPOLLIN){//Read available from client
                read_client(event.data.fd);
                //pool.add_task(event_map[fd]);
            }

        }
    }
    
}

void Server::add_client(){
    int cfd=0;//cfd will be zero when listenfd is set to non-block and ET
   
    while ((cfd=listenSocket.accept_client())>=0){
        //by default the flag is set to EPOLLIN|EPOLLET
       // cout<<"connection accepted!, Client info: \n"<<listenSocket.get_client()<<endl;
        epoll->add_fd(cfd);
        cout<<" add fd "<<endl;
    }
    cout<<"add fds done"<<endl;
}


void Server::read_client(int fd){// read data from client store the http connection.
    
    char buf[1024]={0};
    int len=sizeof(buf),read_bytes=0;
    string data;
    //continuous read from the non-blocking fd
    cout<<"begin read"<<endl;
    while((read_bytes=read(fd, buf, len))>0){
        data.append(buf,read_bytes);
        if(read_bytes<len) break;
        
    }


    if(read_bytes==0){//socket closed
        epoll->del_fd(fd);
        close(fd);
        cout<<"socket close"<<endl;
        return;
    }
    if(read_bytes==-1){
        if(errno!=EAGAIN){
            perror("read");
            epoll->del_fd(fd);
            close(fd);
           
        }
        //nothing happens, read it again when new data comes in
        cout<<"socket error"<<endl;
        return ;
    }

    cout<<"read complete"<<endl;

    Http http(data,fd,epoll);

    auto fun=compose(Http::handleHttp,http);
    thread_pool.addTask(fun);

}
