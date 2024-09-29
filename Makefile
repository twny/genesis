CC = gcc
CFLAGS = -Wall -std=c11

# Include paths
INCLUDE = /opt/homebrew/include

# Library paths and libraries
LIBPATH = /opt/homebrew/lib
LIBS = -lglfw -lGLEW -lcglm -framework OpenGL -framework Cocoa -framework IOKit

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main -I$(INCLUDE) -L$(LIBPATH) $(LIBS)

clean:
	rm -f main
