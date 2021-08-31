CC=g++
CFLAGS= -std=c++11 -Wall -pedantic -g
SOURCES= XMLParser.cpp
DEPS= XML.h XMLParser.h XMLDisplayer.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=exec

all: $(EXECUTABLE)
	echo Make has finished.

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp

clean:
	$(RM) *.o *~ $(EXECUTABLE)
