#pragma once
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
//Encapsulate the POSIX semaphore
class Semaphore{
    sem_t semaphore;
public:
    Semaphore(int val);
    void wait();
    void post();
    ~Semaphore();

};
