CC = gcc
CFLAGS = -Wall -I$(INCDIR) -g

HOMEDIR = ../..
INCDIR = $(HOMEDIR)/include
SRCDIR = $(HOMEDIR)/src
OBJDIR = $(HOMEDIR)/obj

BIN = ts_test

all: $(BIN)

.PHONY: all test clean

$(BIN): ts_test.c $(OBJDIR)/ts.o $(INCDIR)/ts.h
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

test:
	./$(BIN)

clean:
	rm -f *~ 
