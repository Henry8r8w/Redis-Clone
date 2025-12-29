IMAGE_NAME = redis-clone
CONTAINER_NAME = redis-runner

build:
	docker build -t $(IMAGE_NAME) .

run:
	docker run -p 6379:6379 --rm --name $(CONTAINER_NAME) $(IMAGE_NAME) 

stop:
	docker stop $(CONTAINER_NAME)

clean:
	docker rmi $(IMAGE_NAME)