#include "epoll.h"
#include "socket.h"
#include <iostream>
#include <unistd.h>

int main() {
    // Create a socket, bind it to a port, and start listening for connections.
    Socket server_socket(3456);
    int server_fd = server_socket.getfd();

    // Create an epoll instance and add the server socket file descriptor.
    Epoll epoll(10);
    epoll.add_fd(server_fd, EPOLLIN);

    while (true) {
        // Wait for events on the epoll instance.
        auto events = epoll.Epoll_wait(-1);

        for (const auto& event : events) {
            if (event.data.fd == server_fd) {
                // If the event is on the server socket, accept a new client connection.
                int client_fd = server_socket.accept_client();
                std::cout<<server_socket.get_client()<<std::endl;
                if (client_fd != -1) {
                    // Add the client file descriptor to the epoll instance.
                    epoll.add_fd(client_fd);
                }
            } else {
                // If the event is on a client socket, read data from the client.
                int client_fd = event.data.fd;
                char buffer[1024] = {0};
                ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

                if (bytes_read <= 0) {
                    // If the client closed the connection or there was an error, remove the client file descriptor.
                    epoll.del_fd(client_fd);
                    close(client_fd);
                } else {
                    // Print the received data and echo it back to the client.
                    std::cout << "Received from client: " << buffer << std::endl;
                    write(client_fd, buffer, bytes_read);
                }
            }
        }
    }

    return 0;
}