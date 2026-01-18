#include "receiver.hpp"
#include <unordered_map>


int Receiver::create(const std::string& key, RedisData data) {
    redis_data[key] = data;
    return 0;
}

int Receiver::update(const std::string& key, RedisData data) {
    redis_data[key] = data;
    return 0;

}


RedisData Receiver::read(const std::string& key){
    try {
        return redis_data.at(key);
    }
    catch (const std::exception &e) {
        return RedisData{}; 
    }
}

int Receiver::_delete(const std::string& key){
    redis_data.erase(key);
    return 0;

}