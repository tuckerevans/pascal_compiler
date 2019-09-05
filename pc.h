#ifndef PC_H
#define PC_H

#include "y.tab.h"
#include "tree.h"

char* pretty_type(int);

void debug_print(int, union YYSTYPE*);

int yyerror(char*);
#endif

int count_args(ptree*);

#define DEBUG
