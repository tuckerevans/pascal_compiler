#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "y.tab.h"
#include "pc.h"

extern char *yytext;
extern int line_num;

char* pretty_type(t)
int t;
{
	if (t == ARRAY) {
		return "ARRAY";
	} else if (t == INT - ARRAY) {
		return "ARRAY of INT";
	} else if (t == INT - ARRAY) {
		return "ARRAY of INT";
	} else if (t == INT) {
		return "INT";
	} else if (t == REAL) {
		return "REAL";
	} else if (t == BOOL) {
		return "BOOL";
	} else if (t == -1) {
		return "NO TYPE";
	} else {
		return "Unknown type";
	}
}

void debug_print(d, y)
int d;
union YYSTYPE* y;
{
#ifdef DEBUG
	if (d == PROG){
		fprintf(stderr, "[PROG]");
	} else if (d == VAR){
		fprintf(stderr, "[VAR]");
	} else if (d == PROC){
		fprintf(stderr, "[PROC]");
	} else if (d == FUNC){
		fprintf(stderr, "[FUNC]");
	} else if (d == BEG){
		fprintf(stderr, "[BEG]");
	} else if (d == END){
		fprintf(stderr, "[END]");
	} else if (d == ARRAY){
		fprintf(stderr, "[ARRAY]");
	} else if (d == OF){
		fprintf(stderr, "[OF]");
	} else if (d == INT){
		fprintf(stderr, "[INT]");
	} else if (d == REAL){
		fprintf(stderr, "[REAL]");
	} else if (d == IF){
		fprintf(stderr, "[IF]");
	} else if (d == THEN){
		fprintf(stderr, "[THEN]");
	} else if (d == ELSE){
		fprintf(stderr, "[ELSE]");
	} else if (d == WHILE){
		fprintf(stderr, "[WHILE]");
	} else if (d == DO){
		fprintf(stderr, "[DO]");
	} else if (d == FOR){
		fprintf(stderr, "[FOR]");
	} else if (d == TO){
		fprintf(stderr, "[TO]");
	} else if (d == DT){
		fprintf(stderr, "[DOWNTO]");
	} else if (d == NOT){
		fprintf(stderr, "[NOT]");
	} else if (d == DOTS){
		fprintf(stderr, "[ .. ]");
	} else if (d == ASSIGNOP){
		fprintf(stderr, "[ASSIGN]");
	} else if (d == RELOP){
		fprintf(stderr, "[RELOP:");
		if (y->opval == EQ){
			fprintf(stderr, "EQ");
		} else if (y->opval == NE){
			fprintf(stderr, "NE");
		} else if (y->opval == LT){
			fprintf(stderr, "LT");
		} else if (y->opval == LE){
			fprintf(stderr, "LE");
		} else if (y->opval == GT){
			fprintf(stderr, "GT");
		} else if (y->opval == GE){
			fprintf(stderr, "GE");
		} else if (y->opval == ADD){
			fprintf(stderr, "ADD");
		} else if (y->opval == SUB){
			fprintf(stderr, "SUB");
		} else if (y->opval == OR){
			fprintf(stderr, "OR");
		} else if (y->opval == MUL){
			fprintf(stderr, "MUL");
		} else if (y->opval == DIV){
			fprintf(stderr, "DIV");
		} else if (y->opval == AND){
			fprintf(stderr, "AND");
		}
		fprintf(stderr,"]");
	} else if (d == INUM) {
		fprintf(stderr, "[INUM: %d]", y->ival);
	} else if (d == RNUM) {
		fprintf(stderr, "[INUM: %f]", y->rval);
	} else if (d == ID) {
		fprintf(stderr, "[ID: %s]", y->sval);
	} else if (d == '\n') {
		fprintf(stderr, "\n");
	} else {
		fprintf(stderr, "{%c}", d);
	}
	fprintf(stderr, " ");
#endif
	return;
}

int yyerror(msg)
char* msg;
{
	fprintf(stderr, "\nError, line %d: %s\n", line_num, msg);
#ifdef DEBUG
	fprintf(stderr, "%s\n", yytext);
	exit(1);
#endif
	return 0;
}

int main()
{
	yyparse();

	return 0;
}
