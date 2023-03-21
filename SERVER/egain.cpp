#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fcntl.h>
using namespace std;


int main(){
    char buf[1024]={0};
    if(fcntl(0,F_SETFL,(fcntl(0, F_GETFL, 0)|O_NONBLOCK))==-1){
        perror("fcntl");
    }
    while(1){
        int i=read(0,buf,1024);
        if(i==-1)
            perror("read");
        cout<<i<<endl;
        sleep(1);

    }
    return 0;
}