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
       // cout<<ready_fd.size()<<" fds are ready"<<endl;
        for(auto& event:ready_fd){
            if(event.data.fd==listenfd){
                add_client();
            }  
            else if(event.events&(EPOLLHUP|EPOLLERR)){
                if(epoll->del_fd(event.data.fd)!=-1)
                    close(event.data.fd);
                else
                    perror("EPOLL del in server: ");
            }
            else if(event.events&EPOLLIN){//Read available from client
                //pool.add_task(event_map[fd]);
                int fd=event.data.fd;
                auto http=std::make_shared<Http>("",fd,epoll);
                thread_pool.addTask(compose(Http::readHttp,http));
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
        //cout<<" add fd "<<endl;
    }
    //<<"add fds done"<<endl;
}



 
