#include "socket.h"
#include <iostream>
#include <unistd.h>
int main() {
    unsigned short port = 3456;

    try {
        Socket server(port);

        std::cout << "Server is listening on port " << port << std::endl;

        while (true) {
            int client_fd = server.accept_client();
            if (client_fd != -1) {
                std::cout << "New client connected: " << server.get_client() << std::endl;
                // You can interact with the client using the client_fd file descriptor.
                // ...
                // When you're done, close the client connection:
                close(client_fd);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}