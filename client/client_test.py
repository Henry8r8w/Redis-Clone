import socket
import asyncio

class RedisClient:
    def __init__(self, host, port):
          self.host = '127.0.0.1'
          self.port = 6379

    def get_client(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(2) # 2 second timeout
        try:
            s.connect((self.host, self.port))
            return s
        except BlockingIOError:
            pass

class Message:
    def test_ping(client):
        print("Testing: PING")
        client.sendall(b"*1\r\n$4\r\nPING\r\n") 
        data = client.recv(1024)
        print(f"Received {data!r}")

    def send_cmd(client, *args):
        buf = f"*{len(args)}\r\n"
        for arg in args:
            buf += f"${len(arg)}\r\n{arg}\r\n"
        client.sendall(buf.encode('utf-8'))

    def set_get(client):
        print("Testing SET name Henry")
        send_cmd(client, "SET", "name", "Henry")
        data = client.recv(1024)
        print(f"Received {data!r}")
    
        print("Testing GET name")
        send_cmd(client, "GET", "name")
        data = client.recv(1024)
        print(f"Received {data!r}")

def parse_resp(resp_str):
    pos = 0
    content = []
    if resp_str[0] != "*":
        return 
    for i in range(len(resp_str)):
        if ....


if __name__ == "__main__": 
    s = get_client()
    if s:
        test_ping(s)
        set_get(s)
    else:
        print("Could not connect")

""""
 std::vector<std::string> content;
    size_t pos = 0; 
    std::string raw(buffer, length);

    if (raw[pos] != '*') return {}; 

    size_t line_end = raw.find("\r\n", pos);
    if (line_end == -1) return {}; 

    int num_elements = std::stoi(raw.substr(pos + 1, line_end - 1 - pos));
    
    pos = line_end + 2;
    
    for (int i = 0; i < num_elements; ++i) {
        if (raw[pos] != '$') break; // Error handling needed here

        line_end = raw.find("\r\n", pos);
        int str_len = std::stoi(raw.substr(pos + 1, line_end - 1 - pos));

        pos = line_end + 2;
      
        std::string payload = raw.substr(pos, str_len);
        content.push_back(payload);

        pos += str_len + 2;
   
    }

    return content;
"""
