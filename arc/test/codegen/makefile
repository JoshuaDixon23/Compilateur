CC = gcc
CFLAGS = -Wall -I$(INCDIR) -g

HOMEDIR = ../..
INCDIR = $(HOMEDIR)/include
SRCDIR = $(HOMEDIR)/src
OBJDIR = $(HOMEDIR)/obj

BIN = codegen_test

all: $(BIN)

.PHONY: all test clean

$(BIN): codegen_test.c $(OBJDIR)/codegen.o $(OBJDIR)/ast.o $(OBJDIR)/ts.o $(INCDIR)/codegen.h
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

test:
	./$(BIN)

clean:
	rm -f *~ 
