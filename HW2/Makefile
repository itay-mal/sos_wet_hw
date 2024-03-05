# Makefile for the smash program
CC = g++
CPPFLAGS = -std=c++11 -g -Wall -Werror -pedantic-errors -DNDEBUG -pthread
CCLINK = $(CC)
TARGET = Bank
OBJS = main.o bank.o atm.o account.o log.o
RM = rm -f
# Creating the  executable
$(TARGET): $(OBJS)
	$(CCLINK) -pthread -o $(TARGET) $(OBJS)
# Creating the object files
atm.o: atm.cpp atm.hpp account.hpp log.hpp
log.o: log.cpp log.hpp
bank.o: bank.cpp bank.hpp account.hpp log.hpp
account.o: account.cpp account.hpp
main.o: main.cpp account.hpp log.hpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.* 
