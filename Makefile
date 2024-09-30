CC = gcc
CFLAGS = -Wall -std=c17

# Detect the operating system
UNAME_S := $(shell uname -s)

# macOS-specific settings
ifeq ($(UNAME_S),Darwin)
    INCLUDE = /opt/homebrew/include
    LIBPATH = /opt/homebrew/lib
    LIBS = -lglfw -lGLEW -lcglm -framework OpenGL -framework Cocoa -framework IOKit
    INSTALL_DEPS = mac_deps
endif

# Linux-specific settings
ifeq ($(UNAME_S),Linux)
    INCLUDE = /usr/include
    LIBPATH = /usr/lib
    LIBS = -lglfw -lGLEW -lcglm -lGL -lm
    INSTALL_DEPS = linux_deps
endif

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main -I$(INCLUDE) -L$(LIBPATH) $(LIBS)

mac_deps:
	@echo "Installing dependencies for macOS environment"
	brew install glew glfw cglm

linux_deps:
	@echo "Installing dependencies for Linux environment"
	sudo apt-get install libglfw3-dev libglew-dev libcglm-dev

deps: $(INSTALL_DEPS)

clean:
	rm -f main