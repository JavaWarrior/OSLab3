CC=gcc
CFLAGS=-Wall -Wno-unused-value -c
LINKLIB= -lpthread
all: caltrain user_made
	$(CC) caltrain_user.o caltrain-runner.o -o caltrain $(LINKLIB)
caltrain: caltrain-runner.c
	$(CC) $(CFLAGS)  caltrain-runner.c -o caltrain-runner.o 
user_made: caltrain.c caltrain.h
	$(CC) $(CFLAGS)  caltrain.c -o caltrain_user.o 
clean:
	rm caltrain caltrain_user.o caltrain-runner.o
