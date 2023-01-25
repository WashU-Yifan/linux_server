#pragma once
#include <sys/types.h>        
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
class Socket{
    
   // friend const std::ostream& operator<<(const std::ostream&,const Socket&);
    public:
        Socket();

        void setup(int port, const char* add );//set up listening socket
        void setup(int port );//set up listening socket with inet_addr=0;

        int accept_client();
        inline struct sockaddr_in* get_client(){return add;}
        inline int getfd() {return sfd;};
        ~Socket();
    private:

        int sfd,port;
        struct sockaddr_in *add;//store client connection from accept_client()
};

 std::ostream& operator<<( std::ostream&,const struct sockaddr_in *add);