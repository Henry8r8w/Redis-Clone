FROM gcc:latest

WORKDIR /app

COPY . .


RUN g++ -o server -std=c++17 -Iinclude src/*.cpp src/concrete-commands/*.cpp

# Expose the Redis port
EXPOSE 6379

CMD ["./server"]