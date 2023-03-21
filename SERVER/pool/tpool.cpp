 
#include "tpool.h"
#include <iostream>
//template class Tpool<Task<void(int, int), int, int>>;


template <typename T>
Tpool<T>:: Tpool(unsigned short size,int max_request):
 Tnumber(size), max_request(max_request),sem(0)
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
    if(Tque.size()>(size_t)max_request){
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
        que_mut.unlock(); 


        Http& http=task();
        if(http.del()){
            //If wirte back is not perfromed and is not due to no-nonblocking issues
            close(http.getfd());
            auto epoll_shared_ptr = http.epoll.lock();
            if (epoll_shared_ptr) {
                epoll_shared_ptr->del_fd(http.getfd());
            } else {
                std::cout<<"epoll pointer not valid"<<std::endl;
                throw new std::exception();
            }
            

        }
        else{
            if(http.again())
                addTask(compose(Http::handleHttp,http));
        }
    }
}





