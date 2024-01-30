CC = gcc
CFLAGS = -O -Wall
LEX = flex
YACC = bison -d
HDRS = compiler.tab.h
LDFLAGS =
LIBS = -lfl -ly
OBJS = compiler.tab.o lex.yy.o ast.o
PROGRAM = compiler

all: $(PROGRAM)

$(PROGRAM): $(OBJS) $(HDRS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)
lex.yy.c: compiler.l
	$(LEX) compiler.l
compiler.tab.c: compiler.y
	$(YACC) compiler.y
clean:; rm -f *.o *~
###
ast.o: ast.c ast.h
