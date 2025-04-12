
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string>
using namespace std;

#define ct cout
#define el endl

int main()
{

    char path[] = "/tmp/Pranjal";


    int counter = 10;

    ct << "parent" << el;

    char val[1024];

    int fd = open(path, O_RDONLY);

    int tmp = 0;

    int bytesRead = read(fd, val, sizeof(val));

    while (bytesRead > 0)
    {
        ct << "bytes read = " << bytesRead << el;
        tmp++;
        ct << val << el;

        bytesRead = read(fd, val, sizeof(val));
    }

    ct << "parent done, tmp = " << tmp << el;

    close(fd);

    return 0;
}