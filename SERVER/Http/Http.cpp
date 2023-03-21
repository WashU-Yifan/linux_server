#include"Http.h"
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sstream>
using std::string;
using std::shared_ptr;
using std::endl;
using std::cout;
using std::stringstream;

Http::Http(string  s,int _fd,const shared_ptr<Epoll>& ep):epoll(ep),
data(s),fd(_fd),_again(true),_del(false){
    //std::cout << "rp.use_count() == " << ep.use_count() << "; "<<endl;
}

Http::Http(const Http& _http):epoll(_http.epoll),data(_http.data)
,fd(_http.fd),_again(true),_del(false){}

Http Http::handleHttp( const Http & _http){ 

    Http http(_http);
    cout<<http.data<<endl;
    
    http.response();
    cout<<http.res<<endl;
    int writed=http.write_back();
    cout<<writed<<endl;
    switch (writed){
        case -1:
        //if the resource is not available
            http._again=false;
            http._del=true;
            return http;
        case 1://success
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
    size_t len=res.size();

    const char * dat=res.c_str();
    ssize_t w_bytes=0,sum_bytes=0;
    while((w_bytes=write(fd,dat,len))>0){
        sum_bytes+=w_bytes;
        if(sum_bytes==res.size()) return 1;
        dat += w_bytes;
        len -= w_bytes;
    }
    if(errno==EAGAIN) return EAGAIN;
    return w_bytes;
}

void Http::response(){
    string http_version = "HTTP/1.1";
    string status_code = "200";
    string status_message = "OK";

    // Prepare headers
    string content_type = "text/html";
    string content = "<html><head><title>Sample Page</title></head><body><h1>Hello, World!</h1></body></html>";
    string content_length = std::to_string(content.length());

    // Create the response
    stringstream response;

    // Start line
    response << http_version << " " << status_code << " " << status_message << "\r\n";

    // Headers
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << content_length << "\r\n";
    response << "Connection: close\r\n"; // Add this line
    // End of headers
    response << "\r\n";

    // Body
    response << content;
    response << "\r\n";
    res=response.str();
}


string upper(string s){
    for(char &c:s){
        if(c<='z'&&c>='a')c-=32;
    }
    return s;
}

