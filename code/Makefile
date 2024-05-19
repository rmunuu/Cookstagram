# Makefile for building the project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Executable name
TARGET = my_program

# Source files
SRCS = main.c sign.c home.c global_var.c print.c

# Object files directory
OBJDIR = obj

# Object files
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the build directory
clean:
	rm -f $(OBJS) $(TARGET)
	rmdir $(OBJDIR)

.PHONY: all clean

