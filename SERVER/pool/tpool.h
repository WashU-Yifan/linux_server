#include <thread>
#include <queue>
class Tpool{
    public:
        Tpool(unsigned short size): Tnumber(size){}
        void addTask();
    private:
        std::queue<std::thread> Tque;
        const unsigned short Tnumber;
        
};