#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string>
#include <thread>

#include <iostream>
#include <string.h>

using namespace std;

#define ct cout
#define el endl

int main(int argc, char const *argv[])
{

    const char *pName = "/my_shm";

    const int SIZE = 2000*2000;

    int fd = shm_open(pName, O_RDONLY, 0666);

    if (fd == -1)
    {
        perror("shm_open failed: ");
        return -1;
    }

    void *ptr = mmap(nullptr, SIZE, PROT_READ, MAP_SHARED, fd, 0);

    char buffer[SIZE];

    if (ptr == MAP_FAILED)
    {
        perror("mmap failed");
        return -1;
    }

    int counter = 0;

    while (counter++ < 100)
    {

        memcpy(buffer, ptr, SIZE);

        ct << buffer[1] << el;

        this_thread::sleep_for(chrono::milliseconds(5));
    }

    munmap(ptr, SIZE);

    close(fd);

    // unlink only server

    return 0;
}

/*

Value	Meaning
4	    Read permission (r)
2	    Write permission (w)
1	    Execute (or search) permission (x)


Scenario	        Recommended Mode	Why?
Development/test	0666	            Easy access for all users/processes
Multi-user system	0600 or 0644	    More secure — restricts access
Public/shared IPC	0666 or 0644	    Allows readers/writers, depending on needs
Sensitive data	    0600	            Only your process can access it

*/
