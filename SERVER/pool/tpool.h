#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include "task.h"



//Encapsulate the POSIX semaphore
class Semaphore{
    sem_t semaphore;
public:
    Semaphore(int val);
    void wait();
    void post();
    ~Semaphore();

};


//#include <semaphore>
/*这是一个简单的线程池（Tpool）实现，用于处理任务队列。
线程池的作用是管理一组线程，这些线程可以并发地执行任务。
线程池使用一个任务队列（Tque）来存储待执行的任务，以及
一个信号量（sem）来同步线程。*/

template <typename T>
class Tpool{
    public:
        Tpool(unsigned short size, int);
        bool addTask(const T & task);
        //Worker function of the thread, continuously pop the task que and run the task.
        void runHead();
    private:
        int max_request;
        std::mutex que_mut;
        Semaphore sem;
        std::queue<T> Tque;
        std::vector<std::thread> threads;
        const unsigned short Tnumber;
        
};



