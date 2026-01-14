#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <algorithm>
#include <netinet/in.h>
#include "util/parser.hpp"
#include "receiver.hpp"
#include "invoker.hpp"
#include "concrete-commands/set.hpp"
#include "concrete-commands/get.hpp"


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

    // Command dispatch setup
    Receiver receiver;
    Invoker invoker;

    // Event-loop: Accept clients
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        
        if (client_fd < 0) {
            continue;
        }
        
        std::cout << "Client connected\n";
        

        std::string accumulated_buffer;
        char temp[1024];
        
        // Inner loop: Handle this client's requests
        while (true) {
            memset(temp, 0, sizeof(temp));
            int bytes_read = read(client_fd, temp, sizeof(temp));
            
            if (bytes_read <= 0) {
                break;  // Client disconnected
            }
            
            // Accumulate data
            accumulated_buffer.append(temp, bytes_read);
            
            // Try to parse
            std::vector<std::string> parts = parse_resp(
                accumulated_buffer.c_str(), 
                accumulated_buffer.length()
            );

                      
            if (parts.empty()){
                continue;  // Incomplete message, need more data
                
            } 
        
            for(int i = 0; i< parts.size(); i++){
                std::cout << parts[i] << std::endl; 
            }
            
            std::string commandName = parts[0];
            std::unique_ptr<Command> cmd = nullptr;
            
            if (commandName == "SET") {
                if (parts.size() < 3) continue; 
                cmd = std::make_unique<SetCommand>(receiver, parts[1], parts[2]);
                invoker.addCommand(std::move(cmd));
                invoker.executeAll();
                write(client_fd, "+OK\r\n", 5); 
            } 
            else if (commandName == "GET") {
                if (parts.size() < 2) continue; 
                auto getCmd = std::make_unique<GetCommand>(receiver, parts[1]);
                GetCommand* ptr = getCmd.get();
                
                invoker.addCommand(std::move(getCmd));
                invoker.executeAll();
                RedisData result = ptr->getResult();
                if (std::holds_alternative<std::string>(result)) {
                    std::string val = std::get<std::string>(result);
                    std::string resp = "$" + std::to_string(val.length()) + "\r\n" + val + "\r\n";
                    write(client_fd, resp.c_str(), resp.size());
                } else {
                    write(client_fd, "$-1\r\n", 5);
                }
            }
            else if (commandName == "PING") {
                write(client_fd, "+PONG\r\n", 7);
            }
            
            // Clear buffer after processing
            accumulated_buffer.clear();
        }
        
        close(client_fd);
        std::cout << "Client disconnected\n";
    }
    
    close(server_fd);
    return 0;
}