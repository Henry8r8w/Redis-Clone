IMAGE_NAME = redis-clone
CONTAINER_NAME = redis-runner

build:
	docker build -t $(IMAGE_NAME) .

run:
	docker run -p 6379:6379 --rm --name $(CONTAINER_NAME) $(IMAGE_NAME)


shell:
	docker exec -it $(CONTAINER_NAME) /bin/bash

# Test from inside the container
test:
	docker exec $(CONTAINER_NAME) sh -c 'printf "*1\r\n\$$4\r\nPING\r\n" | nc localhost 6379'

# Test fragmentation from inside container
test-frag:
	docker exec $(CONTAINER_NAME) sh -c '(printf "*3\r\n\$$3\r\nSET\r\n\$$3\r\nkey"; sleep 0.1; printf "\r\n\$$5\r\nvalue\r\n") | nc localhost 6379'

stop:
	docker stop $(CONTAINER_NAME)

clean:
	docker rmi $(IMAGE_NAME)