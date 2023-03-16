#include "task.h"
#include "tpool.h"
#include <iostream>
void add(int a, int b){
    std::cout<<a+b<<std::endl;
}
int main(){
    Tpool<decltype(add)> threads(4,10000);
    while(1){
        Task<decltype(add),int,int> t(add, 0,1);
        threads.addTask(t);
    }
    return 0;
}