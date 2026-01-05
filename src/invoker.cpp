#include "invoker.hpp"
#include <memory>
#include <vector>


void Invoker::addCommand(std::unique_ptr<Command> cmd){
    command_queue.push_back(std::move(cmd));
}

void Invoker::executeAll() {
    for (auto& cmd : command_queue) {
        if (cmd) { 
            cmd->execute();
        }
    }
    command_queue.clear();
}