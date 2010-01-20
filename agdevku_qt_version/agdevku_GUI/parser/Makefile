# Makefile

OBJS	= parser.o lexer.o main.o Schema.o ExecuteStructures.o

CC	= g++
CFLAGS	= -g -Wall -ansi -pedantic

sql_exp:	$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o sql_exp -ly -ll

lexer.o:	lexer.c
		$(CC) $(CFLAGS) -c lexer.c -o lexer.o

lexer.c:	lexer.l 
		flex lexer.l
		mv lex.yy.c lexer.c

parser.o:	parser.c
		$(CC) $(CFLAGS) -c parser.c -o parser.o

parser.c:	parser.y
		bison -d -v parser.y
		cp parser.tab.c parser.c
		cmp -s parser.tab.h tok.h || cp parser.tab.h tok.h

Schema.o:	../catalog/Schema.cpp
		$(CC) $(CFLAGS) -c ../catalog/Schema.cpp -o Schema.o	

ExecuteStructures.o:	../executor/ExecuteStructures.cpp
		$(CC) $(CFLAGS) -c ../executor/ExecuteStructures.cpp -o ExecuteStructures.o	

main.o:		main.cc
		$(CC) $(CFLAGS) -c main.cc -o main.o

lexer.o parser.o main.o	: heading.h
lexer.o main.o		: tok.h
Schema.o		: ../catalog/Schema.h
ExecuteStructures.o	:../executor/ExecuteStructures.h

clean:
	rm -f *.o *~ lexer.c lex.yy.c parser.c tok.h parser.tab.c parser.tab.h parser.output sql_exp 

