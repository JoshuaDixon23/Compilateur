CC = gcc
CFLAGS = -Wall -I$(INCDIR) -g

HOMEDIR = ../..
INCDIR = $(HOMEDIR)/include
SRCDIR = $(HOMEDIR)/src
OBJDIR = $(HOMEDIR)/obj

BIN = ast_test

all: $(BIN)

.PHONY: all test clean

$(BIN): ast_test.c $(OBJDIR)/ast.o $(INCDIR)/ast.h
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

test:
	./$(BIN)

clean:
	rm -f *~ 
