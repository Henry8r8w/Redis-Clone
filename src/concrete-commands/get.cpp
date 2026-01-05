#include "concrete-commands/get.hpp"
#include "receiver.hpp"

void GetCommand::execute() {
    result = receiver.read(key);
}


RedisData GetCommand::getResult() {
    return result;
}

