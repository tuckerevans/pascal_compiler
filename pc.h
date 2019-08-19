#ifndef PC_H
#define PC_H

#include "y.tab.h"

char* pretty_type(int);

void debug_print(int, union YYSTYPE*);

int yyerror(char*);
#endif

#define DEBUG
