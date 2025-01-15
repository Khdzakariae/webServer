#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string.h>

int main() {
    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Connect to server on localhost
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Same port as server
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Failed to connect to server. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server!" << std::endl;

    // Send some message to the server
    std::string message = "Hello from client!\n";
    send(sockfd, message.c_str(), message.size(), 0);

    // Receive the server's response
    char buffer[100] = {0};
    int bytesRead = read(sockfd, buffer, 100);
    if (bytesRead < 0) {
        std::cout << "Failed to read from server. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Server's response: " << buffer;

    // Close the socket
    close(sockfd);

    return 0;
}