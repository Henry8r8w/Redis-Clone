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

    // Event-loop
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
                std::cout << parts[i] << std::endl; 
            }

            std::string commandName = parts[0];
            std::unique_ptr<Command> cmd = nullptr;

            if (commandName == "SET") {
                cmd = std::make_unique<SetCommand>(receiver, parts[1], parts[2]);
                invoker.addCommand(std::move(cmd));
                invoker.executeAll();

                write(client_fd, "+OK\r\n", 5); 
            } 
            else if (commandName == "GET") {
                auto getCmd = std::make_unique<GetCommand>(receiver, parts[1]);
                GetCommand* ptr = getCmd.get(); // Keep a raw pointer to grab the result later
                
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

           
        }
        close(client_fd);
        std::cout << "Client disconnected\n";
    }
    
    close(server_fd);
    return 0;
}