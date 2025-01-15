#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>

const char* HTML_CONTENT = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"    <title>My C++ Web Server</title>\n"
"    <style>\n"
"        body { font-family: Arial, sans-serif; margin: 40px; }\n"
"        h1 { color: #333; }\n"
"        .container { background-color: #f0f0f0; padding: 20px; border-radius: 5px; }\n"
"    </style>\n"
"</head>\n"
"<body>\n"
"    <div class='container'>\n"
"        <h1>Welcome to My C++ Web Server!</h1>\n"
"        <p>This page is being served from a custom C++ web server.</p>\n"
"        <p>Current time: {{TIME}}</p>\n"
"    </div>\n"
"</body>\n"
"</html>\n";

std::string getCurrentTime() {
    time_t now = time(0);
    return ctime(&now);
}

int main() {
    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Allow socket reuse
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cout << "Failed to set socket options. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen to port 8080 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port 8080. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(sockfd, 10) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on http://localhost:8080" << std::endl;

    while(true) {
        // Accept incoming connection
        auto addrlen = sizeof(sockaddr);
        int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
        if (connection < 0) {
            std::cout << "Failed to grab connection. errno: " << errno << std::endl;
            continue;
        }

        // Read the HTTP request (we're not parsing it in this simple example)
        char buffer[1024] = {0};
        read(connection, buffer, 1024);

        // Replace {{TIME}} with current time in the HTML content
        std::string response = HTML_CONTENT;
        std::string timeStr = getCurrentTime();
        size_t pos = response.find("{{TIME}}");
        if (pos != std::string::npos) {
            response.replace(pos, 8, timeStr);
        }

        // Send the HTTP response
        write(connection, response.c_str(), response.length());

        // Close the connection
        close(connection);
    }

    // Close the socket (this won't be reached in this example)
    close(sockfd);
    return 0;
}