#include <iostream>
#include <sys/socket.h>   // For socket functions
#include <sys/un.h>        // For sockaddr_un
#include <unistd.h>        // For close()
#include <cstring>         // For memset

#define SOCKET_PATH "/tmp/my_unix_socket"

int main() {
    // 1. Create a socket
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    // 2. Setup server address
    sockaddr_un server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    std::strcpy(server_addr.sun_path, SOCKET_PATH);

    // 3. Connect to the server
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Receive data
    char buffer[1024] = {0};
    recv(sock, buffer, sizeof(buffer), 0);
    std::cout << "Server says: " << buffer << std::endl;

    // 5. Close socket
    close(sock);

    return 0;
}
