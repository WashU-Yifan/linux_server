#include "task.h"
#include "tpool.h"
#include "../Http/Http.h"
#include <iostream>
#include <stdlib.h>
void add(int a, int b){
    std::cout<<a+b<<std::endl;
}


template <typename F> auto compose (F f,Http a) {
  return [f,a]() { return f(a); };
}



int main(){
    std::string s("aaaaaaa");

    Http http(std::move(s),1);

    //std::function<decltype(add)>f(add);
    auto a=compose(Http::handleHttp,http);
    a();
    
    Tpool<std::function<void()>> threads(4,10000);

    while(1){
        threads.addTask(a);

    }
    
    return 0;
}