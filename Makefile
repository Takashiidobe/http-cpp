CXX := g++

CXXFLAGS := -std=c++17 -O2 -lsqlite3

all: program

program:
	$(CXX) $(CXXFLAGS) main.cc -o server

run: server
	./server

clean:
	rm -f server

dev:
	make program && make run
