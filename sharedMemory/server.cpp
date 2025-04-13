#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>
#include <thread>

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

    const int SIZE = 2000*2000;

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

    void *ptr = mmap(nullptr, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);

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


    int num = 0;

    while (num++ < 100)
    {

        // string payload = msg + to_string(num);

        memcpy(ptr, msg, SIZE);

        ct << "payload dropped, msg[1] =  " << msg[1] << el;

        this_thread::sleep_for(chrono::milliseconds(50));
    }

    ct << "press any key to stop..." << el;

    getchar();
    munmap(ptr, SIZE);

    close(fd);

    shm_unlink(mName);

    return 0;
}
