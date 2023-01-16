#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
int sfd;
void* thread_work(void* param);

int main(){
    sfd= socket(AF_INET, SOCK_STREAM, 0);
    int cfd;
    pthread_t tid;
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
    printf("server ready\n");
    while (1){
        if((cfd=accept(sfd,(struct sockaddr*) &c_sock,&size))==-1){
                perror("accept: ");
                exit(0);
        }

        char addr[16]=""; 
        printf("client IP: %s \nclient port: %d\n",inet_ntoa(c_sock.sin_addr),c_sock.sin_port);
        printf("fd %d\n", cfd);
        if(pthread_create(&tid,NULL,thread_work,(void *)&cfd)){
            printf("thread initate failed\n");
            exit(0);
        }
        printf("initiating thread, thread id %d\n",tid);
    }
    
    close (sfd);
    return 0;
}


void* thread_work(void* param){
    int cfd= *(int*) param;
    printf(" client fd %d\n", cfd);
    pthread_detach(pthread_self());
    while(1){
        char buf[128]="";
        int size=read(cfd,buf,128);
        if(size==0){
            printf("client closed\n");
            break;
        }
        else if(size==-1){
            perror("read");
            break;
        }
            printf("received from client:  %s",buf);

            char wbuf[128]="hello from server\n";
            printf("send to client:   %s",wbuf);
            if(write(cfd, wbuf, strlen(wbuf))==-1){
                perror("write: ");
                exit(0);
            }
    }


    printf("thread %d ending, closing fd %d\n", pthread_self(), cfd);
    close(cfd);
    pthread_exit(0);
    
}