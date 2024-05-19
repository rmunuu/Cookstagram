# # Makefile for building the project

# # Compiler
# CC = gcc

# # Compiler flags
# CFLAGS = -Wall -g

# # Executable name
# TARGET = my_program

# # Source files
# SRCS = basic_funcs.c feed.c global_vars.c home.c main.c recipe_utils.c sign.c structs.c

# # Object files directory
# OBJDIR = obj

# # Object files
# OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

# # Default target
# all: $(TARGET)

# # Link object files to create the executable
# $(TARGET): $(OBJDIR) $(OBJS)
# 	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# # Compile source files to object files
# $(OBJDIR)/%.o: %.c | $(OBJDIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Clean the build directory
# clean:
# 	rm -f $(OBJS) $(TARGET)
# 	rmdir $(OBJDIR)

# .PHONY: all clean

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -Iheaders

# Executable name
TARGET = my_program

# Source files
SRCS = basic_funcs.c feed.c global_var.c home.c main.c recipe_utils.c sign.c structs.c

# Object files directory
OBJDIR = obj

# Object files
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

# Default target
all: $(OBJDIR) $(TARGET)

# Create the object files directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the build directory
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)
	rmdir $(OBJDIR)

.PHONY: all clean
