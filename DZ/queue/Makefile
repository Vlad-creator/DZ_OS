CXX = clang++
CXXFLAGS = -c 

.PHONY: clean

all: main

main: main.o queue.o
	 $(CXX) queue.o main.o -o $@

queue.o: queue.cpp
	 $(CXX) queue.cpp $(CXXFLAGS)

main.o: main.cpp
	 $(CXX) main.cpp $(CXXFLAGS)

clean:
	rm -rf *.o main queue