CC = gcc
OPTIMIZE = -O2
WARNINGS = -Wall -Wextra
LIBS = -lgmp -lpthread

all: buildfolder primemirp
buildfolder:
	mkdir -p build

primemirp: build/thread_data.o build/llist.o build/main.o
	$(CC) $(OPTIMIZE) $(WARNINGS) $^ -o $@ $(LIBS)

build/thread_data.o: thread_data.c
	$(CC) $(OPTIMIZE) $(WARNINGS) -c $^ -o $@

build/llist.o: llist.c
	$(CC) $(OPTIMIZE) $(WARNINGS) -c $^ -o $@

build/main.o: main.c
	$(CC) $(OPTIMIZE) $(WARNINGS) -c $^ -o $@

.PHONY:
clean:
	rm -rf build
	rm -f primemirp
