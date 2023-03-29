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


        static std::shared_ptr<Http> handleHttp(const std::shared_ptr<Http>& );
        static std::shared_ptr<Http> readHttp(const std::shared_ptr<Http> &);
        inline int getfd(){return fd;}
        inline bool again(){return _again;}
        inline bool del(){return _del;}
        inline bool read_down(){return _read_done;}
        //std::shared_ptr<Epoll> epoll;
        std::weak_ptr<Epoll> epoll;
    private:
        std::string data;
        std::string res;
        int fd;
        bool _again;
        bool _del;
        bool _read_done;
        
        
};


