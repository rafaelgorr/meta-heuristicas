CC         = gcc
CFLAGS     = -c -lm -Wall -std=c11
GCFLAGS    = -lm -Wall
SOURCES    = functions.c \
							globals.c \
							main.c
OBJECTS    = $(SOURCES:.c=.o)
EXECUTABLE = main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(GCFLAGS) -o $@


run:	
	./main ${n} < ${input}
	./main ${n} < ${input}
	./main ${n} < ${input}
	./main ${n} < ${input}
	./main ${n} < ${input}
	./main ${n} < ${input}
	./main ${n} < ${input}
	./main ${n} < ${input}
	./main ${n} < ${input}
	./main ${n} < ${input}

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)