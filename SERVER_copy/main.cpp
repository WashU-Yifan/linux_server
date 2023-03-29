#include "server/server.h"
#include <regex>
#include <iostream>

using std::cout;
using std::endl;

const int argc_min=3;
const int argc_max=6;
static int portnum=0;
static int wait_time=0;
//these are default numbers
static char * inet="0.0.0.0";
static int max_request_size=4096;
static int thread_num=4;


const char* check_input(int, char **);


int main(int argc , char ** argv){
    if(argc<argc_min||argc>argc_max){
        cout<< "Usage: ./Webserver <port> <timeout> <inet> <max_request_size> <thread_num>\n"<<endl;
        return 0;
    }
    const char * usage=check_input(argc,argv);
    if(usage!=nullptr){
        cout<<usage<<endl;
        return 0;
    }
    Server Webserver(portnum,wait_time, inet, max_request_size,thread_num);
    Webserver.Run();
    return 0;    
}



const char * check_input(int argc, char ** argv){
    std::regex pattern("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
   
    switch (argc){
        case 6:
            thread_num=atoi(argv[5]);
            if (thread_num <= 0) {
                return "Error: Invalid thread_num value. It must be a positive integer.\n";
            }
        case 5:
            max_request_size=atoi(argv[4]);
            if (max_request_size <= 0) {
                return "Error: Invalid max_request_size value. It must be a positive integer.\n";
            }
        case 4:
            inet=argv[3];
            if (!std::regex_match(inet, pattern)) 
                return "The input string is not a valid IPv4 address.";
        case 3:
            portnum=atoi(argv[1]);
            if (portnum < 1 || portnum > 65535) {
                return "Error: Invalid port number. Port must be between 1 and 65535.\n";
            }
            wait_time=atoi(argv[2]);
            if (wait_time < -1) {
                return "Error: Invalid timeout value. Timeout must be a non-negative integer or -1 for no timeout.\n";
            }
            break;
        default:
            return nullptr;
    }
    return nullptr;
    
}