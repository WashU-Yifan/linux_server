#include "task.h"
#include "tpool.h"
#include <iostream>
#include <stdlib.h>
void add(int a, int b){
    std::cout<<a+b<<std::endl;
}
int main(){
    Task<decltype(add),int,int> t(add, 0,1);;
    Tpool<decltype(t)> threads(4,10000);
    while(1){
        t=Task<decltype(add),int,int>(add,0,1);
        threads.addTask(t);

    }
    return 0;
}