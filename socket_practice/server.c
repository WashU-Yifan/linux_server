#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <string.h>
int main(){
    int sfd= socket(AF_INET, SOCK_STREAM, 0);
    int cfd;
    if(sfd==-1){
        perror("socket: ");
        exit(0);
    }
    struct sockaddr_in sock;
    sock.sin_family = AF_INET;   
    sock.sin_addr.s_addr=htonl(INADDR_ANY);
    sock.sin_port=htons(1234);
    
    if(bind(sfd,(struct sockaddr*) &sock,sizeof(sock))==-1){
        perror("bind: ");
        exit(0);
    }
    if (listen(sfd, 128)==-1){
        perror("listen: ");
        exit(0);
    }
    struct sockaddr_in c_sock;
    unsigned int size=sizeof(c_sock);
    if((cfd=accept(sfd,(struct sockaddr*) &c_sock,&size))==-1){
        perror("accept: ");
        exit(0);
    }
    char addr[16]=""; 
    printf("client IP: %s \nclient port: %d\n",inet_ntoa(c_sock.sin_addr),c_sock.sin_port);
  


    char buf[128]="";
    if(read(cfd,buf,128)==0){
        printf("client closed\n");
        exit(0);
    }
    printf("received from client:  %s",buf);

    char wbuf[128]="hellor from server\n";
    printf("send to client:   %s",wbuf);
    if(write(cfd, wbuf, strlen(wbuf))==-1){
        perror("write: ");
        exit(0);
    }
    close(cfd);
    close(sfd);
    return 0;
}
