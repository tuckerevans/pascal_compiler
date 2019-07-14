%{

#include "y.tab.h"
#include "pc.h"

int line_num=1;

%}

whitespace	[ \t]+
number	[0-9]+
id	[A-Za-z][A-Za-z0-9_]*

%x COMMENT

%%

<COMMENT>. {}
<COMMENT>\n {}

^[ \t]*"{"	{BEGIN COMMENT;}
<COMMENT>"}"	{BEGIN INITIAL;}

^[ \t]*"(*"	{BEGIN COMMENT;}
<COMMENT>"*)"	{BEGIN INITIAL;}


{whitespace}	;

"program" {
	debug_print(PROG, NULL);
	return PROG;
}

"var" {
	debug_print(VAR, NULL);
	return VAR;
}

"procedure" {
	debug_print(PROC, NULL);
	s = push_scope(s);
	return PROC;
}

"function" {
	debug_print(FUNC, NULL);
	s = push_scope(s);
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
	debug_print(RELOP, &yylval);
	return ADDOP;
}

"-" {
	yylval.opval = SUB;
	debug_print(RELOP, &yylval);
	return ADDOP;
}

"or" {
	yylval.opval =  OR;
	debug_print(RELOP, &yylval);
	return ADDOP;
}

"*" {
	yylval.opval = MUL;
	debug_print(RELOP, &yylval);
	return MULOP;
}

"/" {
	yylval.opval = DIV;
	debug_print(RELOP, &yylval);
	return MULOP;
}

"and" {
	yylval.opval = AND;
	debug_print(RELOP, &yylval);
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
