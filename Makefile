.PHONY: clean install uninstall

CC=g++
CXXFLAGS=-lwiringPi
PREFIX=~

all: src/*.o
	$(CC) $(CXXFLAGS) src/*.cpp
src/%.o: src/%.cpp
	$(CC) $(CXXFLAGS) -c $^

debug: CXXFLAGS += -g
debug: all

clean: 
	-rm -f src/*.o
	-rm -f src/*.gch
	-rm -f a.out
