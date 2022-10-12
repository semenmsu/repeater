CC=g++-12
#CFLAGS=-std=c++17 -g -O3
#LIBFLAGS=-lpcap -lpthread -lstdc++fs
#LDFLAGS +=-L/lib64/

all: check_dir clean repeator

.PHONY: check_dir
check_dir:
	mkdir -p bin

repeator:
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/main main.cpp $(LIBFLAGS)

.PHONY: clean
clean:
	rm -rf bin/*
