
LINK.o = $(LINK.cc)
CXXFLAGS = -std=c++14 -Wall

all: correctness persistence main

correctness: kvstore.o correctness.o SSTable.o SSTable_handler.o MemTable.o

persistence: kvstore.o persistence.o SSTable.o SSTable_handler.o MemTable.o

main: kvstore.o main.o SSTable.o SSTable_handler.o MemTable.o

clean:
	-rm -f correctness persistence *.o
