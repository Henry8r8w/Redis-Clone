#include "parser.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <cstring>

std::vector<std::string> parse_resp(const char* buffer, int length) {
    std::vector<std::string> content;
    size_t pos = 0; // store theoretically size of any type, depending on computer bit limit (32 or 64)

    std::string raw(buffer, length);

    
    if (raw[pos] != '*') return {}; 

    size_t line_end = raw.find("\r\n", pos);
    if (line_end == -1) return {}; // Incomplete

    int num_elements = std::stoi(raw.substr(pos + 1, line_end - 1 - pos));
    
    pos = line_end + 2;
    
    for (int i = 0; i < num_elements; ++i) {
        if (raw[pos] != '$') break; // Error handling needed here

        line_end = raw.find("\r\n", pos);
        int str_len = std::stoi(raw.substr(pos + 1, line_end - 1 - pos));

        pos = line_end + 2;
      
        std::string payload = raw.substr(pos, str_len);
        content.push_back(payload);

        pos += str_len + 2; //Skip trailing \r\n
   
    }

    return content;
}