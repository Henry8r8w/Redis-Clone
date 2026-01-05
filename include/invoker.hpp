#include <memory>
#include <vector>
#include "command.hpp"

class Invoker {
private:
    std::vector<std::unique_ptr<Command>> command_queue;

public:
    void addCommand(std::unique_ptr<Command> cmd);
    void executeAll();
};
