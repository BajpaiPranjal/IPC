#include <mqueue.h>
#include <csignal>
#include <iostream>

using namespace std;

mqd_t mqd;

void handler(int sig) {
    char buf[1024];
    ssize_t bytes = mq_receive(mqd, buf, sizeof(buf), nullptr);
    if (bytes >= 0) {
        cout << "Got message: " << buf << endl;
    } else {
        perror("mq_receive");
    }

    // Re-arm the notification!
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;

    if (mq_notify(mqd, &sev) == -1) {
        perror("mq_notify");
    }
}

int main() {
    string qName = "/myQueue";

    mqd = mq_open(qName.c_str(), O_RDONLY | O_NONBLOCK);
    if (mqd == -1) {
        perror("mq_open");
        return 1;
    }

    // Setup signal handler
    signal(SIGUSR1, handler);

    // Register for notification
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;

    if (mq_notify(mqd, &sev) == -1) {
        perror("mq_notify");
        return 1;
    }

    cout << "Waiting for messages..." << endl;

    // Wait forever (or use pause/sleep/loop)
    while (true) {
        pause(); // Wait for signal
    }

    mq_close(mqd);
    return 0;
}
