#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

#define SOCKET_PATH "/tmp/fd_socket"

int recv_fd(int socket) {
    struct msghdr msg = {};
    char m_buffer[1];
    struct iovec io = { .iov_base = m_buffer, .iov_len = sizeof(m_buffer) };

    char c_buffer[CMSG_SPACE(sizeof(int))];
    msg.msg_control = c_buffer;
    msg.msg_controllen = sizeof(c_buffer);
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    recvmsg(socket, &msg, 0);

    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
    int fd;
    memcpy(&fd, CMSG_DATA(cmsg), sizeof(fd));
    printf("fd rcvd = %d\n",fd);
    return fd;
}

int main() {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    connect(sock, (sockaddr*)&addr, sizeof(addr));

    int received_fd = recv_fd(sock);

    // Read from received FD
    char buffer[100];
    read(received_fd, buffer, sizeof(buffer));
    std::cout << "Received data: " << buffer << std::endl;

    close(received_fd);
    close(sock);
    return 0;
}
