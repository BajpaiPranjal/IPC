/*A message queue is a kernel-managed data structure that allows processes to exchange structured messages asynchronously.
Think of it like a mailbox where one process drops messages and another one picks them up.

Asynchronous communication

Messages have boundaries and optional priorities

Persistent during the lifetime of the queue

Stored in kernel space

FIFO order (unless priority is used)

*/

#include <mqueue.h>
#include <fcntl.h>    // O_CREAT, O_RDONLY, etc.
#include <sys/stat.h> // Mode constants
#include <cstring>
#include <thread>

#include <iostream>

using namespace std;

#define ct cout
#define el endl

int main(int argc, char const *argv[])
{

    string qName = "/myQueue";

    struct mq_attr attr;

    mqd_t mq = mq_open(qName.c_str(), O_CREAT | O_WRONLY, 0644, nullptr);

    if (mq == -1)
    {
        perror("mq creation failed: \n");
        return -1;
    }

    string msg = "Hello ";

    int counter = 10;

    ct<<"server idle..."<<el;
    this_thread::sleep_for(chrono::milliseconds(5000));
    ct<<"Server started "<<el;

    while (counter-- > 0)
    {

        string payload = msg + to_string(counter);

        int res = mq_send(mq, payload.c_str(), payload.size() + 1, 0);

        if (res == -1)
        {
            ct << "failed to send message" << el;
            return -1;
        }

        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    ct << "message sent...press any key to close" << el;

    getchar();

    mq_close(mq);

    mq_unlink(qName.c_str());
    //unlink should be done by server

    return 0;
}
