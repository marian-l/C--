# define compiler and compiler-flags, like version and all warning messages (WarningAll = Wall)
CC = g++
CFLAGS = -std=c++17 -Wall

# directories
SRCDIR = .
LIBDIR = $(SRCDIR)/Library
OBJDIR = $(SRCDIR)/Networking/Objects

# SourceFiles to compile
SRCS = $(LIBDIR)/StringFunction.cpp $(OBJDIR)/HTTPRequest.cpp

# Object-Files to be created
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

# Output file
TARGET = HTTPRequest.exe

# Special Makefile Targets: 
.PHONY: all clean

# run when not supplying a target
all: $(TARGET)

# how to build target: needs objects to work. g++ .std=c++11 -Wall -o $Objects 
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# only compile 
$(OBJDIR)/%.o: $(OBJDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# do it with library directory
$(LIBDIR)/%.o: $(LIBDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# delete all
clean:
	rm -f $(OBJS) $(TARGET)