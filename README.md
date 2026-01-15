# Redis Clone (C++) 

>**Oneliner:** In-memory database solving c10k requset problem using RESP protocol for client-server interaction

## Implementation Status

### Core Architecture
- [x] **TCP Server**
- [x] **Parser Integration**
- [x] **Command Design Pattern:** Modular desgin for command dispatch
- [x] **Concurrency Control**: Poll implmentation (solving the c10k problem)
- [ ] **Pipelining**: Request batching

## Build and Run

This project uses a **Makefile** to automate the **Docker** container lifecycle

### Prerequisites
* Docker
* Make

### Commands

**1. Build the Container**
Compiles the C++ source and builds the Docker image
```bash
make build
```

**2. Run the Server:** Starts the container and binds the port (default: 6379)
```
make run
```

**3. Test the RESP commands:** You do by editing the clien_test.py in seperate terminal

