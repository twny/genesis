CC = gcc
CFLAGS = -Wall  -g -fsanitize=address -std=c17

# Include paths
INCLUDE = -I/opt/homebrew/include

# Library paths and libraries
LIBPATH = -L/opt/homebrew/lib
LIBS = -lglfw -lGLEW -lcglm -framework OpenGL -framework Cocoa -framework IOKit

# Source directory
SRCDIR = src

# Find all .c files in the source directory
SOURCES := $(shell find $(SRCDIR) -name '*.c')
OBJECTS := $(SOURCES:.c=.o)

all: main

run: main
	./main
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LIBPATH) $(LIBS)

mac_deps:
	@echo "Installing dependencies for macos environment"
	brew install glew glfw cglm


clean:
	rm -f $(OBJECTS) main

fclean: clean
	rm -f main

re: fclean all
