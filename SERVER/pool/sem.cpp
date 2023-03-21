#include  "sem.h"


Semaphore::Semaphore(int val){
    if(sem_init(&semaphore, 0, val)){
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
}
void Semaphore::wait(){
    if(sem_wait(&semaphore)){
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }

}
void Semaphore::post(){
    if(sem_post(&semaphore)){
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}


Semaphore::~Semaphore(){
    sem_destroy(&semaphore);
}
