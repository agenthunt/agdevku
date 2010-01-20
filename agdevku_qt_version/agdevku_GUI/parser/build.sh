yacc -d -p parser -b parser parser.y
rm -f parser_yacc.h parser_yacc.cpp
mv -f parser.tab.h parser_yacc.h
mv -f parser.tab.c parser_yacc.cpp
g++ -c -o parser_yacc.o  parser_yacc.cpp

flex -Plexer lexer.l
rm -f lexer_lex.cpp
mv -f lex.lexer.c lexer_lex.cpp

g++ -c -o lexer_lex.o lexer_lex.cpp

g++ -c -o main.o main.cc 

g++ -o parser_main main.o lexer_lex.o parser_yacc.o -ly -ll
