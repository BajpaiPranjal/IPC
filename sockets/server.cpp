#include <iostream>
#include <sys/socket.h>   // For socket functions
#include <sys/un.h>        // For sockaddr_un
#include <unistd.h>        // For close()
#include <cstring>         // For memset

#define SOCKET_PATH "/tmp/my_unix_socket"

int main() {
    // 1. Create a socket (Domain: AF_UNIX, Type: SOCK_STREAM)
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // 2. Setup server address structure
    sockaddr_un server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;           // Use UNIX domain
    std::strcpy(server_addr.sun_path, SOCKET_PATH); // Path for the socket

    // 3. Unlink the path if it already exists (important!)
    unlink(SOCKET_PATH);

    // 4. Bind the socket to the path
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 5. Listen for incoming connections
    listen(server_fd, 5);

    std::cout << "Server listening on " << SOCKET_PATH << "...\n";

    // 6. Accept a connection
    int client_socket = accept(server_fd, nullptr, nullptr);
    std::cout << "Client connected!\n";

    // 7. Send data
    const char* message = "Hello from UNIX domain server!";
    send(client_socket, message, strlen(message), 0);

    // 8. Close sockets
    close(client_socket);
    close(server_fd);

    // 9. Remove the socket file
    unlink(SOCKET_PATH);

    return 0;
}
