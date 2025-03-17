// write in a other terminal: nc localhost 6667

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int main(){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];
    int port = 6667; // Default IRC port

	// Socket Creation: The function socket(2) is used to create a TCP socket for the server.
	// It establishes a communication endpoint for the server to listen for client connections.
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr)); // Set up the server address struct
    server_addr.sin_family = AF_INET;			// Set the address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
    server_addr.sin_port = htons(port);		// Convert the port to network byte order

	// Binding: The function bind(2) binds the created socket to a specific port and IP address,
	// as defined in the struct sockaddr_in.
	// This associates the socket with a network address.
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

	// Listening: The listen(2) function prepares the socket to accept incoming connections.
	// It specifies the maximum number of connections that can be queued for acceptance.
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

	// Accepting Connections: The accept(2) function waits for a client to connect.
	// When a connection is established, it creates a new socket
	// (referred to as client_fd) to handle the communication with the connected client.
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    std::cout << "Client connected" << std::endl;

	// Reading Data: The read(2) function is used to read data from the client socket.
	// It retrieves data sent by the client, which can then be processed by the server.
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("read");
        close(client_fd);
        close(server_fd);
        return 1;
    }

    buffer[bytes_read] = '\0'; // Null-terminate the buffer
    std::cout << "Received message: " << buffer << std::endl;

    // Close the client and server sockets
    close(client_fd);
    close(server_fd);

    return 0;
}