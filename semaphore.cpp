#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>

using namespace std;

#define ct cout
#define el  endl

#define MAX 10000

int val= 0;

int upCnt = 0, dwnCnt  = 0;

counting_semaphore<1> inc(1);
counting_semaphore<1> decsmp(0); 

void increse(){
    
    while(upCnt < MAX){
        inc.acquire();
        val++;
        // ct<<"val incremented = "<<val<<el;
        upCnt++;
        decsmp.release();
    }
}

void descrease(){
    while(dwnCnt < MAX){
        decsmp.acquire();
        val--;
        // ct<<"val decerased = "<<val<<el;
        dwnCnt++;
        inc.release();
    }    
}



void makeThreadAndRunCode()
{
    thread t1(increse);
    
    
    thread t2(descrease);
    
    t1.join();    
    t2.join();
}
int main()
{
    ct<<"Hii"<<el;
    
    
    int tc = 0;
    
    auto start = chrono::system_clock::now();
    
    while(tc++ < 5){
        makeThreadAndRunCode();
        ct<<"tc = "<<tc<<" final val = "<<val<<el;
    }
    
    chrono::duration<double> dur = chrono::system_clock::now() - start;
    
    ct<<"time taken = "<<dur.count()<<el;
    // ct<<"final val = "<<val<<el;
    return 0;
}