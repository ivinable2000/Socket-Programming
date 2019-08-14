CC=gcc

all: server client
.PHONY: all

server: server.c
	$(CC) server.c -o server

client: client.c
	$(CC) client.c -o client

clean:
	rm server client
