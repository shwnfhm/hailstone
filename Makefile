#######
# Makefile for Hailstone
#######
CC = gcc
CFLAGS = -I. -o

all: client server

client:
	$(CC) $(CFLAGS) client client.c csapp.c

server:
	$(CC) $(CFLAGS) server server.c compute.c csapp.c

clean:
	rm -f *.o *~ client
	rm -f *.o *~ server
	rm -rf *.dSYM