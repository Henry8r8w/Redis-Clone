# Redis Clone (C++)

>**Oneliner:**

## Implementation Status

### Core Architecture
- [x] **TCP Server**
- [x] **Parser Integration**: 
    - [x] RESP2 Compliance (Array/Bulk String focus)
    - [ ] ~~RESP3 Compliance~~ *(Deferred)*
- [ ] **Concurrency Control**: Threading model pending
- [ ] **Pipelining**: Request batching

### Data Types and Commands
- [x] **PING/PONG**
- [ ] **Strings**: Basic `GET`/`SET` (Partial)
- [ ] **Hashes**: `HGET`/`HSET` (Partial)
- [ ] **Lists**: `LPUSH`/`LPOP` (Partial)
- [ ] **DEL**
- [ ] **FlushALL**

### Performance & Reliability
- [ ] **Benchmarking**: Latency/Throughput testing

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

