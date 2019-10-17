%{
#include "node.h"
#include "scope.h"
#include "y.tab.h"
#include "pc.h"
#include "assert.h"

int line_num=1;
extern scope *cur_scope;

%}

whitespace	[ \t]+
number	[0-9]+
id	[A-Za-z][A-Za-z0-9_]*

%x COMMENT

%%

"{"	{BEGIN COMMENT;}
<COMMENT>"}"	{BEGIN INITIAL;}

"(*"	{BEGIN COMMENT;}
<COMMENT>"*)"	{BEGIN INITIAL;}

<COMMENT>. {}
<COMMENT>\n {}


{whitespace}	;

"program" {
	debug_print(PROG, NULL);
	return PROG;
}

"output" {
	node *tmp;
	tmp = scope_insert(cur_scope, strdup("write"));
	tmp->func_info = malloc(sizeof(struct fi));
	assert(tmp->func_info);
	tmp->func_info->argc = -1;
	tmp->func_info->argv = NULL;

#ifdef GENERATE_CODE
	fprintf(stdout, ".data\n");
	fprintf(stdout, "int_print:\n\t .asciz \"%%d\\n\"\n");
	fprintf(stdout, "float_print:\n\t .asciz \"%%f\\n\"\n.text\n");
#endif


	return IO;
}

"input" {
	node *tmp;
	tmp = scope_insert(cur_scope, strdup("read"));
	tmp->func_info = malloc(sizeof(struct fi));

	return IO;
}

"var" {
	debug_print(VAR, NULL);
	return VAR;
}

"procedure" {
	debug_print(PROC, NULL);
	push_scope(&cur_scope);
	return PROC;
}

"function" {
	debug_print(FUNC, NULL);
	push_scope(&cur_scope);
	return FUNC;
}

"begin" {
	debug_print(BEG, NULL);
	return BEG;
}

"end" {
	debug_print(END, NULL);
	return END;
}

"array" {
	debug_print(ARRAY, NULL);
	return ARRAY;
}

"of" {
	debug_print(OF, NULL);
	return OF;
}

"integer" {
	debug_print(INT, NULL);
	return INT;
}

"real" {
	debug_print(REAL, NULL);
	return REAL;
}

"if" {
	debug_print(IF, NULL);
	return IF;
}

"then" {
	debug_print(THEN, NULL);
	return THEN;
}

"else" {
	debug_print(ELSE, NULL);
	return ELSE;
}

"while" {
	debug_print(WHILE, NULL);
	return WHILE;
}

"do" {
	debug_print(DO, NULL);
	return DO;
}

"for" {
	debug_print(FOR, NULL);
	return FOR;
}

"to" {
	debug_print(TO, NULL);
	return TO;
}

"downto" {
	debug_print(DT, NULL);
	return DT;
}

"not" {
	debug_print(NOT, NULL);
	return NOT;
}

".." {
	debug_print(DOTS, NULL);
	return DOTS;
}

":=" {
	debug_print(ASSIGNOP, NULL);
	return ASSIGNOP;
}

"=" {
	yylval.opval =  EQ;
	debug_print(RELOP, &yylval);
	return RELOP;
}

"<>" {
	yylval.opval =  NE;
	debug_print(RELOP, &yylval);
	return RELOP;
}

"<" {
	yylval.opval =  LT;
	debug_print(RELOP, &yylval);
	return RELOP;
}

"<=" {
	yylval.opval =  LE;
	debug_print(RELOP, &yylval);
	return RELOP;
}

">" {
	yylval.opval =  GT;
	debug_print(RELOP, &yylval);
	return RELOP;
}

">=" {
	yylval.opval =  GE;
	debug_print(RELOP, &yylval);
	return RELOP;
}

"+" {
	yylval.opval = ADD;
	debug_print(ADDOP, &yylval);
	return ADDOP;
}

"-" {
	yylval.opval = SUB;
	debug_print(ADDOP, &yylval);
	return ADDOP;
}

"or" {
	yylval.opval =  OR;
	debug_print(ADDOP, &yylval);
	return ADDOP;
}

"*" {
	yylval.opval = MUL;
	debug_print(MULOP, &yylval);
	return MULOP;
}

"/" {
	yylval.opval = DIV;
	debug_print(MULOP, &yylval);
	return MULOP;
}

"and" {
	yylval.opval = AND;
	debug_print(MULOP, &yylval);
	return MULOP;
}

{number} {
	yylval.ival = atoi(yytext);
	debug_print(INUM, &yylval);
	return INUM;
}

{number}"."{number} {
	yylval.rval = atof(yytext);
	debug_print(RNUM, &yylval);
	return RNUM;
}

{number}[eE][+-]?{number} {
	yylval.rval = atof(yytext);
	debug_print(RNUM, &yylval);
	return RNUM;
}
{number}.{number}[eE][+-]?{number} {
	yylval.rval = atof(yytext);
	debug_print(RNUM, &yylval);
	return RNUM;
}

{id} {
	yylval.sval = strdup(yytext);
	debug_print(ID, &yylval);
	return ID;
}

\n {
	debug_print('\n', NULL);
	++line_num;
}

. {
	debug_print(yytext[0], NULL);
	return yytext[0];
}
