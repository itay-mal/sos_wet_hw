# Makefile for the smash program
CC = g++
CPPFLAGS = -std=c++11 -g -Wall -Werror -pedantic-errors -DNDEBUG -pthread
CCLINK = $(CC)
TARGET = output_redirection
OBJS = main.o
RM = rm -f
# Creating the  executable
$(TARGET): $(OBJS)
	$(CCLINK) -pthread -o $(TARGET) $(OBJS)
# Creating the object files
main.o: main.cpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.* 
