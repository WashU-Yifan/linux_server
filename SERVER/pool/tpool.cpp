 
#include "tpool.h"

template class Tpool<Task<void(int, int), int, int>>;
template <typename T>
Tpool<T>:: Tpool(unsigned short size,int max_request):
 Tnumber(size), sem(0),max_request(max_request)
 {
    //initialize the thread vector
    for(int i=0;i<size;++i){
        threads.emplace_back(&Tpool::runHead, this);
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
    Tque.push(task);
    que_mut.unlock();
    sem.post();//sem +1
    return true;
}

template <typename T>
void Tpool<T>:: runHead(){
    while(1){
        sem.wait();//sem -1
        que_mut.lock();

        T task = Tque.front(); // Get the task from the front of the queue
        Tque.pop();
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
