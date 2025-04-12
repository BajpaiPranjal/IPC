
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

	

	if (mkfifo(path, 0666) == -1)
	{
		ct << "failed to create name pipe" << el;
	}

	int counter = 10;

	int fd = open(path, O_WRONLY);

	string msg = "Hello Pranjal";

	while (counter-- > 0)
	{

		int bytes = write(fd, msg.c_str(), msg.size() + 1);

		if (bytes <= 0)
		{
			ct << "Write failed at = " << counter << el;

			break;
		}

		this_thread::sleep_for(chrono::milliseconds(500));
	}

	ct << "child done counter = " << counter << el;

	close(fd);
	unlink(path);
	return 0;
}