 
#include "tpool.h"


template <typename T>
Tpool<T>:: Tpool(unsigned short size): Tnumber(size), sem(size),threads(size){
    //initialize the thread vector
    for(i=0;i<size;++i){
        threads[i]=std::thread(worker);
        threads[i].detach();
    }
}

template <typename T>
void Tpool<T>::addTask(Task<T>&& task){
    que_mut.lock();
    Tque.push(std::forward(task));
    que_mut.unlock();
}

template <typename T>
void Tpool<T>:: runHead(){
    sem.wait();
    que_mut.lock();
    
    Tque.pop().execute();
    que_mut.unlock();
}






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
