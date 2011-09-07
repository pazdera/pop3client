CC=g++
CFLAGS=-c -g -Wall -pedantic 
LDFLAGS=
EXECUTABLE=pop3client

SOURCES_DIR=src/
SOURCES=$(addprefix $(SOURCES_DIR), main.cpp cliarguments.cpp error.cpp socket.cpp pop3session.cpp)

OBJECTS=$(SOURCES:.cpp=.o)


.PHONY: all clean doc

all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

doc:
	doxygen Doxyfile

#install: $(EXECUTABLE)
#	cp $(EXECUTABLE) $(INSTALL_PATH)

