#ifndef PC_H
#define PC_H

#include "y.tab.h"
#include "tree.h"

char* pretty_type(int);

void debug_print(int, union YYSTYPE*);

int yyerror(char*);

int count_args(ptree*);
int set_func_types(ptree*, int*, int);
#endif

#define DEBUG
