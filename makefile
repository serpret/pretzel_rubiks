CC=g++
CFLAGS= -c -O3 -pthread -std=c++11 
LDFLAGS= -pthread
EXECUTABLE=rubikSolve
SOURCES=main.cpp rubiks.cpp
OBJECTS=main.o rubiks.o   #i couldn't get substitution references to work :(
DEPS=rubiks.h++

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) $<

clean:
	rm *.o $(EXECUTABLE)
