import socket
import asyncio
import threading
import time 

HOST = '127.0.0.1'
PORT = 6379

def get_client():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(2) # 2 second timeout
    try:
        s.connect((HOST, PORT))
        return s
    except BlockingIOError:
        pass

def send_cmd(client, *args):
    buf = f"*{len(args)}\r\n"
    for arg in args:
        buf += f"${len(arg)}\r\n{arg}\r\n"
    client.sendall(buf.encode('utf-8'))

def set(client):
    send_cmd(client, "SET", "name", "Henry")
    data = client.recv(1024)
    return data

def get(client):
    send_cmd(client, "GET", "name")
    data = client.recv(1024)
    return data

def ping(client, ops_per_client):
    client = get_client()  # NEW socket per thread
    if not client:
        return
    try:
        for i in range(ops_per_client):
            client.sendall(b"*1\r\n$4\r\nPING\r\n")
            data = client.recv(1024)
            if b"+PONG" not in data:
                print(f"Client {client} unexpected response: {data}")
    except Exception as e:
        print(f"Client:{client} | Error: {e}")
    
    client.close()
    return data



if __name__ == "__main__": 
    OP_PER_CLIENT = 1000
    NUM_CLIENTS = 10
    
    print(f"Starting {NUM_CLIENTS} clients, {OP_PER_CLIENT} ops each...")
    start_time = time.time()
    
    threads = []
    for i in range(NUM_CLIENTS):
        t = threading.Thread(target=ping, args=(i, OP_PER_CLIENT))
        threads.append(t)
        t.start()
    for t in threads:
        t.join()
    
    elapsed_time = time.time() - start_time
    total_ops = OP_PER_CLIENT * NUM_CLIENTS
    ops_per_sec = total_ops // elapsed_time
    
    print(f"\nCompleted {total_ops} ops in {elapsed_time:.2f}s")
    print(f"Throughput: {ops_per_sec} ops/sec")
    
