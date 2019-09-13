CC = gcc
FLAGS = -g -O0
YACC = yacc
LEX = lex

mypc: y.tab.o lex.yy.o tree.o scope.o node.o pc.o sem_check.o
	$(CC) $(FLAGS) -o mypc main.o tree.o scope.o sem_check.o node.o y.tab.o lex.yy.o -lfl -ly

pc.o: main.c headers
	$(CC) $(FLAGS) -c main.c

tree.o: tree.c headers
	$(CC) $(FLAGS) -c tree.c

sem_check.o: sem_check.c headers
	$(CC) $(FLAGS) -c sem_check.c

scope.o: scope.c headers
	$(CC) $(FLAGS) -c scope.c

node.o: node.c headers
	$(CC) $(FLAGS) -c node.c

y.tab.o: y.tab.c
	$(CC) $(FLAGS) -c y.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(FLAGS) -c lex.yy.c

y.tab.c: pc.y
	$(YACC) -y -dv pc.y

lex.yy.c: pc.l
	$(LEX) -l pc.l

headers: pc.h tree.h sem_check.h y.tab.h scope.h node.h y.tab.c

clean:
	rm -f mypc *.o y.tab.* lex.yy.*
