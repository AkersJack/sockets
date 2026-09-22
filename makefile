CC = gcc
CFLAGS = -Wall -Wextra -O0


all: server client


# $@ expands to the name of the target file being built
# $^ expands to the name of all prerequisites (dependencies), separated by spaces, with duplicates removed. 
# $< expands to the file name of the first prerequisite
server : server.o 
	$(CC) $(CFLAGS) -o $@ $^

client : client.o 
	$(CC) $(CFLAGS) -o $@ $^

server.o : server.c 
	cc -c server.c  

client.o : client.c
	cc -c client.c  


clean: 
	rm -rf *.o
	rm -f server client

