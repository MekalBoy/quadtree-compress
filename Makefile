OBJS = main.o tree.o queue.o

build: target

target: $(OBJS)
	gcc $(OBJS) -Wall -g -o quadtree

%.o: %.c 
	gcc -c $< -Wall -g -o $@

clean:
	rm -rf *.o quadtree

run:
	./quadtree
