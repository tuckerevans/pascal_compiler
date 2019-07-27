CC = gcc
FLAGS = -g
YACC = yacc
LEX = lex

mypc: y.tab.o lex.yy.o tree.o node.o pc.o
	$(CC) $(FLAGS) -o mypc main.o tree.o node.o y.tab.o lex.yy.o -lfl -ly

pc.o: main.c pc.h
	$(CC) $(FLAGS) -c main.c

tree.o: tree.c tree.h
	$(CC) $(FLAGS) -c tree.c

hash.o: hash.c hash.h
	$(CC) $(FLAGS) -c hash.c

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
