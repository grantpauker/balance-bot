.PHONY: clean install uninstall

CC=g++
CXXFLAGS=
PREFIX=~

all: src/*.o
	$(CC) $(CXXFLAGS) src/*.cpp
src/%.o: src/%.cpp
	$(CC) $(CXXFLAGS) -c $^

debug: CXXFLAGS += -g
debug: all

clean: 
	-rm src/*.o
	-rm src/*.gch
	-rm a.out
