#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "util/parser.hpp"
#include "receiver.hpp"
#include "invoker.hpp"
#include "concrete-commands/set.hpp"
#include "concrete-commands/get.hpp"

#define MAX_EVENTS 10

void setnonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // TCP setup
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
    setnonblocking(server_fd); // server non-block
    
    std::cout << "Redis Server listening on 6379...\n";

    // Command dispatch setup
    Receiver receiver;
    Invoker invoker;


    // Epoll instance
    int epoll_fd = epoll_create1(0);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev); // Watch for server's incoming event
    
    // Store accumulated buffer per client
    std::map<int, std::string> client_buffers;

    // Event loop
    while (true) {
        int numEvents = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        
        for (int i = 0; i < numEvents; i++) {
            int fd = events[i].data.fd;
            
            // NEW CONNECTION: First in line 
            if (fd == server_fd) {
                
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                
                if (client_fd < 0) continue;
                
                setnonblocking(client_fd); 
                
                // Add client to epoll
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                
                std::cout << "Client connected: " << client_fd << "\n";
            }
            // CLIENT DATA
            else {
                char temp[1024];
                int bytes_read = read(fd, temp, sizeof(temp));
                
                if (bytes_read <= 0) {
                    // Client disconnected
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                    client_buffers.erase(fd);  // Remove buffer
                    std::cout << "Client disconnected: " << fd << "\n";
                }
                else {
                    // Accumulate data for THIS client
                    client_buffers[fd].append(temp, bytes_read);
                    
                    // Try to parse
                    std::vector<std::string> parts = parse_resp(
                        client_buffers[fd].c_str(),
                        client_buffers[fd].length()
                    );
                    
                    if (!parts.empty()) {
                        std::string commandName = parts[0];
                        
                        if (commandName == "PING") {
                            write(fd, "+PONG\r\n", 7);
                        }
                        else if (commandName == "SET" && parts.size() >= 3) {
                            auto cmd = std::make_unique<SetCommand>(receiver, parts[1], parts[2]);
                            invoker.addCommand(std::move(cmd));
                            invoker.executeAll();
                            write(fd, "+OK\r\n", 5);
                        }
                        else if (commandName == "GET" && parts.size() >= 2) {
                            auto getCmd = std::make_unique<GetCommand>(receiver, parts[1]);
                            GetCommand* ptr = getCmd.get();
                            invoker.addCommand(std::move(getCmd));
                            invoker.executeAll();
                            RedisData result = ptr->getResult();
                            if (std::holds_alternative<std::string>(result)) {
                                std::string val = std::get<std::string>(result);
                                std::string resp = "$" + std::to_string(val.length()) + "\r\n" + val + "\r\n";
                                write(fd, resp.c_str(), resp.size());
                            } else {
                                write(fd, "$-1\r\n", 5);
                            }
                        }
                        
                        // Clear buffer after processing
                        client_buffers[fd].clear();
                    }
                }
            }
        }
    }
    close(server_fd);
    return 0;
}