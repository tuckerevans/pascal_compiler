CC = gcc
FLAGS = -g
YACC = yacc
LEX = lex

mypc: y.tab.o lex.yy.o tree.o scope.o node.o pc.o
	$(CC) $(FLAGS) -o mypc main.o tree.o scope.o node.o y.tab.o lex.yy.o -lfl -ly

pc.o: main.c pc.h
	$(CC) $(FLAGS) -c main.c

tree.o: tree.c tree.h
	$(CC) $(FLAGS) -c tree.c

scope.o: scope.c scope.h
	$(CC) $(FLAGS) -c scope.c

node.o: node.c node.h
	$(CC) $(FLAGS) -c node.c

y.tab.o: y.tab.c
	$(CC) $(FLAGS) -c y.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(FLAGS) -c lex.yy.c

y.tab.c: pc.y
	$(YACC) -y -dv pc.y

lex.yy.c: pc.l
	$(LEX) -l pc.l

clean:
	rm -f mypc *.o y.tab.* lex.yy.*
