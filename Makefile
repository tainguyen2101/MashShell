CC=gcc
CFLAGS=-pthread -I. -Wall

binaries1=mash

all: $(binaries1) 

clean:
	$(RM) -f $(binaries1) *.o
	
