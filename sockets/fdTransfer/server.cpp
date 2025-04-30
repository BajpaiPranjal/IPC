#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#define SOCKET_PATH "/tmp/fd_socket"

void send_fd(int socket, int fd_to_send) {
    struct msghdr msg = {};
    struct cmsghdr *cmsg; // pointer to cmsg inside msg
    char buf[CMSG_SPACE(sizeof(fd_to_send))]; // Control message buffer
    memset(buf, 0, sizeof(buf));

    struct iovec io = { .iov_base = (void*)"F", .iov_len = 1 }; // Dummy data

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    cmsg = CMSG_FIRSTHDR(&msg); // returns pointer to cmsg inside msg
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd_to_send));

    memcpy(CMSG_DATA(cmsg), &fd_to_send, sizeof(fd_to_send));

    printf("fd sent = %d\n",fd_to_send);
    sendmsg(socket, &msg, 0);
}

int main() {
    int server_sock = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    unlink(SOCKET_PATH);
    bind(server_sock, (sockaddr*)&addr, sizeof(addr));
    listen(server_sock, 1);

    int client_sock = accept(server_sock, nullptr, nullptr);

    // Open a file to send
    int fd = open("/etc/hostname", O_RDONLY);

    // Send the FD
    send_fd(client_sock, fd);

    close(fd);
    close(client_sock);
    close(server_sock);
    unlink(SOCKET_PATH);

    return 0;
}
