#ifndef COMMAND_HPP
#define COMMAND_HPP

// forward declaration
class Receiver;

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
};

#endif