FROM gcc:latest

WORKDIR /app

COPY . .

RUN g++ -o server server.cpp parser.cpp -std=c++17

# Expose the Redis port
EXPOSE 6379

CMD ["./server"]