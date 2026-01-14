FROM gcc:latest

WORKDIR /app

COPY . .

RUN apt-get update && apt-get install -y \
    build-essential \
    netcat-openbsd \
    && rm -rf /var/lib/apt/lists/*

RUN g++ -o server -std=c++17 -Iinclude src/*.cpp src/concrete-commands/*.cpp

# Expose the Redis port
EXPOSE 6379

CMD ["./server"]

