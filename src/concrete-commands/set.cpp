#include "../include/concrete-commands/set.hpp"
#include "../include/receiver.hpp"

void SetCommand::execute() {
    receiver.update(key, value); 
}
