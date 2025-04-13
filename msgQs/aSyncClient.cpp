#include <mqueue.h>
#include <fcntl.h>     // For O_* constants
#include <sys/stat.h>  // For mode constants
#include <csignal>     // For signal handling
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

using namespace std;

#define ct cout
#define el endl

const char *qName = "/myQueue";
const int BUF_SIZE = 1024;

mqd_t mq;  // global to access in signal handler

void register_notification(); // forward declaration

void signalHandler(int sig)
{
    if (sig == SIGUSR1)
    {
        mq_attr att;

        mq_getattr(mq,&att);

        char buffer[att.mq_msgsize];

        int bytes = mq_receive(mq, buffer, sizeof(buffer), nullptr);
        if (bytes >= 0)
        {
            ct << "[SIGUSR1] Received message: " << buffer << el;
        }
        else
        {
            perror("mq_receive failed");
        }

        // Re-register after handling the message
        register_notification();
    }
}

void register_notification()
{
    struct sigevent sev{};
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    sev.sigev_value.sival_ptr = nullptr;

    if (mq_notify(mq, &sev) == -1)
    {
        perror("mq_notify failed");
    }
    else
    {
        ct << "mq_notify() registered!" << el;
    }
}

int main()
{
    mq = mq_open(qName, O_RDONLY | O_NONBLOCK); // non-blocking so we can drain queue
    if (mq == -1)
    {
        perror("mq_open failed");
        return -1;
    }

    // Install signal handler
    struct sigaction sa{};
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, nullptr) == -1)
    {
        perror("sigaction failed");
        return -1;
    }

    // Drain any existing messages before registering notification
    char dummy[BUF_SIZE];
    while (mq_receive(mq, dummy, sizeof(dummy), nullptr) != -1)
    {
        ct << "Drained old message: " << dummy << el;
    }

    // Register for SIGUSR1 on new message arrival
    register_notification();

    ct << "Waiting for messages (SIGUSR1)... Press Ctrl+C to exit." << el;

    while (true)
    {
        pause(); // wait for signal
    }

    mq_close(mq);
    return 0;
}
