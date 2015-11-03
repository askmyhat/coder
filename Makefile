EXEC    = test_coder
CC      = g++
CFLAGS  = -c -Wall
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.cpp %.h
	$(CC) -c $< -o $@

clean:
	rm -f *.o $(EXEC)

