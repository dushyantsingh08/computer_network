#include <stdio.h>    
#include <stdlib.h>     
#include <string.h>    
#include <unistd.h>     // For close()
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>  

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // 1. Create socket (IPv4, TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set SO_REUSEADDR to reuse the port immediately after the program exits
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // 3. Bind socket to all IP addresses on port 65432
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 4. Listen for incoming connections, queue up to 3
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port 8080...\n");

    // 5. Accept a connection (blocking call)
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Connection accepted\n");

    // 6. Read data from client
    ssize_t valread = read(new_socket, buffer, sizeof(buffer) - 1);
    if (valread < 0) {
        perror("read");
    } else {
        buffer[valread] = '\0';  // Null-terminate the string
        printf("Received: %s\n", buffer);
    }

    // 7. Send response to client
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent to dsr\n");

    // 8. Close sockets
    close(new_socket);
    close(server_fd);

    return 0;
}
