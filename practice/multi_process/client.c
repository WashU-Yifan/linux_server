#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <string.h>
#include<pthread.h>

int main(){

    
    int cfd= socket(AF_INET, SOCK_STREAM, 0);
    if(cfd==-1){
        perror("socket: ");
        pthread_exit(0);
    }
    struct sockaddr_in sock;
    const char * ip="128.252.167.161";
    inet_pton(AF_INET,ip, &sock.sin_addr.s_addr);
    sock.sin_port=htons(1234);
    sock.sin_family=AF_INET;
    if(connect(cfd,(struct sockaddr*) &sock,sizeof(sock))==-1){
        perror("connect");
        pthread_exit(0);
    }
    int i=10;
    while(i--){
        char wbuf[128]="hello from client\n";
        if(write(cfd, wbuf, strlen(wbuf))==-1){
            perror("write: ");
            exit(0);
        }

        printf("  send to server:   %s",wbuf);

        char buf[128]="";
        if(read(cfd,buf,128)==0){
            printf("server closed\n");
            exit(0);
        }
        
        printf(" received from server:  %s",buf);
        sleep(1);
    }
    close(cfd);
    
    return 0;
}

