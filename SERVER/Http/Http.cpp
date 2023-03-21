#include"Http.h"
#include <iostream>
#include <unistd.h>
#include <errno.h>
using std::string;
using std::shared_ptr;



Http::Http(string && s,int _fd,shared_ptr<Epoll> ep):epoll(ep),
data(std::forward<string>(s)),fd(_fd),_again(true),_del(false){}

Http::Http(const Http& h):epoll(h.epoll),data(h.data), fd(h.fd),
_again(true),_del(false){}

Http& Http::handleHttp(Http &http){ 

    int writed=http.write_back();
    if(writed<0){
        //if the resource is not available
        if(errno!=EAGAIN&&errno!=EWOULDBLOCK){
            http._again=false;
            http._del=true;
        }
        return http;
    }
    if(writed){
        if((size_t)writed==http.data.size())
            http._again=false;
        
        return http;
    }  
    http._again=false;
    http._del=true;
    return http;
}


int Http::write_back(){
    size_t len=data.size();
    
    return write(fd,data.c_str(),len);
}
string upper(string s){
    for(char &c:s){
        if(c<='z'&&c>='a')c-=32;
    }
    return s;
}

