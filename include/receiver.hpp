#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <string>
#include <variant>
#include <unordered_map>

using RedisData = std::variant<std::monostate, std::string>; 

class Receiver {
public:
    int create(const std::string& key, RedisData data);
    
    int update(const std::string& key, RedisData data);

    RedisData read(const std::string& key);

    int _delete(const std::string& key);

private:
    std::unordered_map<std::string, RedisData> redis_data;

};

#endif