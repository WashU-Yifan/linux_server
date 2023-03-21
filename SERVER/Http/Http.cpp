#include"Http.h"
#include <iostream>
#include <unistd.h>
#include <errno.h>
using std::string;
using std::shared_ptr;
using std::endl;
using std::cout;


Http::Http(string  s,int _fd,const shared_ptr<Epoll>& ep):epoll(ep),
data(s),fd(_fd),_again(true),_del(false){
    //std::cout << "rp.use_count() == " << ep.use_count() << "; "<<endl;
}

Http::Http(const Http& _http):epoll(_http.epoll),data(_http.data)
,fd(_http.fd),_again(true),_del(false){}

Http Http::handleHttp( const Http & _http){ 

    Http http(_http);
    int writed=http.write_back();

    switch (writed){
        case -1:
        //if the resource is not available
            http._again=false;
            http._del=true;
            return http;
        case 1:
            http._again=false;
            http._del=false;
            return http;
        case EAGAIN:
            http._again=true;
            http._del=false;
            return http;
        default://0
            http._again=false;
            http._del=true;
            return http;
    }   
}


int Http::write_back(){
    size_t len=data.size();
    const char * dat=data.c_str();
    ssize_t w_bytes=0,sum_bytes=0;
    while((w_bytes=write(fd,dat,len))>0){
        sum_bytes+=w_bytes;
        if(sum_bytes==data.size()) return 1;
        dat += w_bytes;
        len -= w_bytes;
    }
    if(errno==EAGAIN) return EAGAIN;
    return w_bytes;
}

string upper(string s){
    for(char &c:s){
        if(c<='z'&&c>='a')c-=32;
    }
    return s;
}

