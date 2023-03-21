#pragma once
#include <string>
#include <memory>
#include "../server/epoll.h"
class Http{
    

    public:
    
        Http()=default;
        Http(std::string , int,const std::shared_ptr<Epoll>& );
        Http(const Http&);
      //  Http(const Http&  );
        void response();
        int write_back();


        static Http handleHttp(const Http&);

        inline int getfd(){return fd;}
        inline bool again(){return _again;}
        inline bool del(){return _del;}
        
        //std::shared_ptr<Epoll> epoll;
        std::weak_ptr<Epoll> epoll;
    private:
        std::string data;
        std::string res;
        int fd;
        bool _again;
        bool _del;
        
        
};


