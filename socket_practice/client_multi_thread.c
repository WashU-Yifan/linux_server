#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <string.h>
#include<pthread.h>
const int THREAD_NUM=10;
pthread_mutex_t  mutex;
void  Thread_work(int i);
int main(){
    pthread_t thread[THREAD_NUM]={0};
    if(pthread_mutex_init(&mutex,NULL)){
        perror("mutex init");
        exit(0);
    }
    for( int i=0;i<THREAD_NUM;++i){
        if(pthread_create(&thread[i],NULL, Thread_work, i)){
            printf("error creating thread%d\n",i);
            exit(0);
        }
        printf("thread %d id  %d create \n",i,thread[i]);
        pthread_detach(thread[i]);
    }
    
    /*
    int cfd= socket(AF_INET, SOCK_STREAM, 0);
    if(cfd==-1){
        perror("socket: ");
        exit(0);
    }
    struct sockaddr_in sock;
    const char * ip="192.168.3.106";
    inet_pton(AF_INET,ip, &sock.sin_addr.s_addr);
    sock.sin_port=htons(1234);
    sock.sin_family=AF_INET;
    if(connect(cfd,(struct sockaddr*) &sock,sizeof(sock))==-1){
        perror("connect");
        exit(0);
    }

    */
    
    return 0;
}

void  Thread_work(int i){
    int cfd= socket(AF_INET, SOCK_STREAM, 0);
    if(cfd==-1){
        perror("socket: ");
        pthread_exit(0);
    }
    struct sockaddr_in sock;
    const char * ip="192.168.3.106";
    inet_pton(AF_INET,ip, &sock.sin_addr.s_addr);
    sock.sin_port=htons(1234);
    sock.sin_family=AF_INET;

    printf("%d thread connecting to server\n",i);
    pthread_mutex_lock(&mutex);
    if(connect(cfd,(struct sockaddr*) &sock,sizeof(sock))==-1){
        perror("connect");
        pthread_exit(0);
    }
    char wbuf[128]="hello from client\n";
    if(write(cfd, wbuf, strlen(wbuf))==-1){
        perror("write: ");
        exit(0);
    }
    char buf[128]="";
    if(read(cfd,buf,128)==0){
        printf("server closed\n");
        exit(0);
    }
    
    
    close(cfd);
    pthread_mutex_unlock(&mutex);
    printf("%d thread send to server:   %s",(int)i,wbuf);
    printf("%d thread received from server:  %s",(int)i,buf);
   
}