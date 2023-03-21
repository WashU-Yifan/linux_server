#pragma once
#include <string>
#include <memory>
#include "../server/epoll.h"
class Http{
    

    public:
    
        Http()=default;
        Http(std::string &&, int,std::shared_ptr<Epoll> );
        Http(const Http&  );
        int write_back();
        static Http& handleHttp(Http&);

        inline int getfd(){return fd;}
        inline bool again(){return _again;}
        inline bool del(){return _del;}
        std::weak_ptr<Epoll> epoll;
    private:
        std::string data;
        int fd;
        bool _again;
        bool _del;
        
        
};