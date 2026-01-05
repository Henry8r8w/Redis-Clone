#include "receiver.hpp"
#include <unordered_map>


int Receiver::create(const std::string& key, RedisData data) {
    redis_data.insert({key, data});
    return 0;
}

int Receiver::update(const std::string& key, RedisData data) {
    redis_data.insert({key, data});
    return 0;

}


RedisData Receiver::read(const std::string& key){
    auto it = redis_data.at(key);
    try{
         auto it = redis_data.at(key);
         return it;
    }
    catch (const std::exception &e) {
        return RedisData{}; 
        }
}

int Receiver::_delete(const std::string& key){
    redis_data.clear();
    return 0;

}