#ifndef PC_H
#define PC_H
char* pretty_type(int);

void debug_print(int, union YYSTYPE*);

int yyerror(char*);
#endif

#define DEBUG
