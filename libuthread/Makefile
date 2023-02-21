# Compiler and linker options
CC := gcc
CFLAGS := -Wall -Werror -Wextra -g

# Target library name and source files
TARGET := libuthread.a
SOURCES := queue.c 

# Object files (generated automatically by make)
OBJECTS := $(SOURCES:.c=.o)

# AR options
AR:= ar
ARFLAGS := rcs

# Main target (build the library)
all: $(TARGET)

# Build the library
$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and library
clean:
	rm -f $(OBJECTS) $(TARGET)
