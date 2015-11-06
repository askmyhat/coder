EXEC    = test_coder
CC      = g++
CFLAGS  = -c -Wall -std=c++11
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $< -o $@

main.o: main.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXEC)

