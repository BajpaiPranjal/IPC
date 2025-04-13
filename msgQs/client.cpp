#include <mqueue.h>
#include <fcntl.h>    // O_CREAT, O_RDONLY, etc.
#include <sys/stat.h> // Mode constants
#include <cstring>

#include <iostream>

using namespace std;

#define ct cout
#define el endl

int main(int argc, char const *argv[])
{

    string qName = "/myQueue";

    mqd_t mQ = mq_open(qName.c_str(), O_RDONLY);

    if (mQ == -1)
    {
        perror("client can't create q");
        return -1;
    }

    mq_attr attr;

    mq_getattr(mQ, &attr);

    char val[attr.mq_msgsize];

    int times = 0;
    int bytesRead = mq_receive(mQ, val, sizeof(val), nullptr);
    ++times;

    while (bytesRead >= 0)
    {
        ct << "bytesRead = " << bytesRead << el;

        ct << "msg is = " << val << el;

        bytesRead = mq_receive(mQ, val, sizeof(val), 0);
        ++times;

        if (times == 10)
        {
            break;
        }
    }

    ct << "times " << times << el;
    if (bytesRead == -1)
    {
        perror("rcv failed");
        return -1;
    }

    mq_close(mQ);

    mq_unlink(qName.c_str());

    return 0;
}
