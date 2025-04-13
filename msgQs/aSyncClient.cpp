#include <mqueue.h>
#include <fcntl.h>     // O_CREAT, O_RDONLY
#include <sys/stat.h>  // mode constants
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <thread>
using namespace std;

#define ct cout
#define el endl

const char *qName = "/myQueue";
const int BUF_SIZE = 1024;

// Notification handler
void messageHandler(union sigval sv)
{
    mqd_t mq = *((mqd_t *)sv.sival_ptr);
    char buffer[BUF_SIZE];

    int bytes = mq_receive(mq, buffer, sizeof(buffer), nullptr);
    if (bytes >= 0)
    {
        ct << "Notification received! Message: " << buffer << el;
    }
    else
    {
        perror("mq_receive failed");
    }

    // Re-register for future notifications
    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = messageHandler;
    sev.sigev_notify_attributes = nullptr;
    sev.sigev_value.sival_ptr = sv.sival_ptr;

    if (mq_notify(mq, &sev) == -1)
    {
        perror("mq_notify (re-register) failed");
    }
}

int main()
{
    // Open the message queue
    mqd_t mq = mq_open(qName, O_RDONLY | O_NONBLOCK);
    if (mq == -1)
    {
        perror("mq_open failed");
        return -1;
    }

    // Register for notification
    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = messageHandler;
    sev.sigev_notify_attributes = nullptr;
    sev.sigev_value.sival_ptr = &mq;

    if (mq_notify(mq, &sev) == -1)
    {
        perror("mq_notify failed");
        return -1;
    }

    ct << "Waiting for message..." << el;

    // Keep main thread alive
    while (true)
    {
        this_thread::sleep_for(chrono::seconds(1));
    }

    mq_close(mq);
    return 0;
}
