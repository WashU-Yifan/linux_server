#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include "task.h"

//#include <semaphore>

template <typename T>
class Tpool{
    public:
        Tpool(unsigned short size); Tnumber(size), sem(size);
        void addTask(Task<T>&& task);
        void runHead();
    private:
        std::mutex que_mut;
        Semaphore sem;
        std::queue<Task<T>> Tque;
        std::vector<thread> threads;
        const unsigned short Tnumber;
        
};

//Encapsulate the semaphore in C language
class Semaphore{
    sem_t semaphore;
    public:
        Semaphore(int val);
        void wait();
        void post();
        ~Semaphore();

};

