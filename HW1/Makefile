# Makefile for the smash program
CC = g++
CPPFLAGS = -std=c++11 -g -Wall -Werror -pedantic-errors -DNDEBUG
CCLINK = $(CC)
TARGET = smash
OBJS = smash.o commands.o signals.o job.o
RM = rm -f
# Creating the  executable
$(TARGET): $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
job.o: job.cpp job.hpp
commands.o: commands.cpp commands.hpp
smash.o: smash.cpp commands.hpp
signals.o: signals.cpp signals.hpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.* 
