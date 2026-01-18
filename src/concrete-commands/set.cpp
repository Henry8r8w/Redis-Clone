#include "../include/concrete-commands/set.hpp"
#include "../include/receiver.hpp"
#include <iostream>

void SetCommand::execute() {
    // receiver.update(key, value); 
    std::cout << "SET key=" << key << std::endl;
    receiver.update(key, value);
    
    // Verify it stored
    RedisData verify = receiver.read(key);
    if (std::holds_alternative<std::string>(verify)) {
        std::cout << "Verified: " << std::get<std::string>(verify) << std::endl;
    } else {
        std::cout << "FAIL: Key not found after SET!" << std::endl;
    }
}
