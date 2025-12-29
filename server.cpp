#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "parser.hpp"


int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // --- TCP SETUP (Standard Boilerplate) ---
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return 1;

    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(6379);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    listen(server_fd, 5);
    std::cout << "Redis Server listening on 6379...\n";

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        
       if (client_fd < 0) {
            continue;
        }

        std::cout << "Client connected\n";

        char buffer[1024];
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytes_read = read(client_fd, buffer, sizeof(buffer));

            std::vector<std::string> parts = parse_resp(buffer, bytes_read);

            if (parts.empty()){
                continue; 
            } 
      
            for(int i = 0; i< parts.size(); i++){
                std::cout << parts[i] << std::endl; // debug
            }
            std::string command = parts[0];
            std::string response;
            
            if (command == "PING") {
                response = "+PONG\r\n";
            }
            else {
                response = "-ERR unknown command\r\n";
            }
            write(client_fd, response.c_str(), response.size());
        }
        close(client_fd);
        std::cout << "Client disconnected\n";
    }
    
    close(server_fd);
    return 0;
}