#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <iostream>

using namespace std;

#define ct cout
#define el endl


/*


the current implentatiaon is working without any sync 
    - memcpy is very fast race condtion is not coming
    - for large data we cant verify what client is reading. may be it is reading half updated frame, while server was in the 
        middle of weriting
*/

int main(int argc, char const *argv[])
{

    const char *mName = "/my_shm";

    const int SIZE = 4;

    int fd = shm_open(mName, O_CREAT | O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open failed: ");
        return -1;
    }

    if(ftruncate(fd, SIZE) == -1){
        perror("ftruncate failde");
        return -1;
    }

    int *ptr = (int*) mmap(nullptr, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    *ptr = 1;

    if(ptr == MAP_FAILED){
        perror("mmap failed");
        return -1;
    }

    // string msg = "Hello World ";
    char msg[SIZE];

    for(int i = 0; i < SIZE; i++){
        msg[i] = 'B';
    }

    ct<<"msg ready"<<el;

    ct<<"server waiting..."<<el;
    // sleep(5);
    
    int counter = 0;
    
    int value = 0;
    
    mutex m;
    
    
    
    ct<<"server started "<<el;
    
    while (counter < 100)
    {

        m.lock();
        value = *ptr;


        
        if(value == 0){
            ct<<"value read = "<<value<<el;
            
            ++value;
    
            *ptr = value;
    
            ct<<"value put = "<<*ptr<<el;

            counter++;
        }


        m.unlock();

        // this_thread::sleep_for(chrono::milliseconds(5));
    }

    
    ct << "press any key to stop..." << el;
    
    getchar();
    
    
    ct<<"Final value = "<<*ptr<<el;
    
    munmap(ptr, SIZE);

    close(fd);

    shm_unlink(mName);

    return 0;
}
