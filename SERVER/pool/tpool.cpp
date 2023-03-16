 
#include "tpool.h"


template <typename T>
Tpool<T>:: Tpool(unsigned short size,int max_request):
 Tnumber(size), sem(size),threads(size),max_request(max_request){
    //initialize the thread vector
    for(i=0;i<size;++i){
        threads[i]=std::thread(runHead);
        threads[i].detach();
    }
}

template <typename T>
bool Tpool<T>::addTask(const T& task){
    que_mut.lock();
    if(Tque.size()>max_request){
        que_mut.unlock();
        return false;
    }
    Tque.push(std::move(task));
    que_mut.unlock();
    sem.post();
    return true;
}

template <typename T>
void Tpool<T>:: runHead(){
    while(1){
        sem.wait();
        que_mut.lock();
    
        auto task=Tque.pop();
        task();
        que_mut.unlock(); 
    }
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
