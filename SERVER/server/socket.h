#pragma once
#include <sys/types.h>        
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <memory>
class Socket{
    
   // friend const std::ostream& operator<<(const std::ostream&,const Socket&);
    public:
        Socket(unsigned short port, const char* IPadd);//set up listening socket
        Socket(unsigned short port);//set up listening socket with inet_addr=0;
        void setup(unsigned short port, const char* add );
        int accept_client();//return the client port number


        inline std::shared_ptr<struct sockaddr_in>get_client(){return client_add;}
        inline std::shared_ptr<struct sockaddr_in>get_socket(){return sock_add;}
        inline int getfd() {return sfd;};
        ~Socket();
    private:

        int sfd;
        unsigned short port;
        std::shared_ptr<struct sockaddr_in>sock_add;
        std::shared_ptr<struct sockaddr_in>client_add;//store client connection from accept_client()
        
};

 std::ostream& operator<<( std::ostream&,std::shared_ptr<struct sockaddr_in> add);