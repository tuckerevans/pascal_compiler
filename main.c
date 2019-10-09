#include "pc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "node.h"
#include "scope.h"
#include "y.tab.h"

extern char *yytext;
extern int line_num;

scope *cur_scope;

char* pretty_type(t)
int t;
{
	if (t == ARRAY) {
		return "ARRAY";
	} else if (t == ARRAY - INT) {
		return "ARRAY of INT";
	} else if (t == ARRAY - REAL) {
		return "ARRAY of REAL";
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
	exit(1);
	return 0;
}

int count_args(t)
ptree *t;
{
	int r, l;

	if (!t)
		return 0;

	if (t->type == LIST){
		r = count_args(t->r);
		l = count_args(t->l);
	} else if (t->type != LIST) {
		return 1;
	} else {
		yyerror("NOT A PARAMETER LIST\n");
	}

	return r + l;
}

int set_func_types(t, nxt, size)
ptree *t;
int size, *nxt;
{
	int tmp;

	if (!t)
		return size;

	if (t->type == LIST){
		tmp = set_func_types(t->l, nxt, size);
		for (;size > tmp; --size) ++nxt;
		size = set_func_types(t->r, nxt, size);
	} else if (t->type == ID){
		if (--size == -1)
			yyerror("VARIABLE COUNT CHANGED!!!\n");

		*nxt = t->attr.nval->var_type;
		return size;
	} else if (t->type == INUM) {
		if (--size == -1)
			yyerror("VARIABLE COUNT CHANGED!!!\n");

		*nxt = INT;
		return size;
	} else if (t->type == RNUM) {
		if (--size == -1)
			yyerror("VARIABLE COUNT CHANGED!!!\n");

		*nxt = REAL;
		return size;
	}
	return size;
}

int get_call_types(t, nxt, size)
ptree *t;
int size, *nxt;
{
	int tmp;

	if (!t)
		return size;

	if (t->type == LIST){
		tmp = set_func_types(t->l, nxt, size);
		for (;size > tmp; --size) ++nxt;
		size = set_func_types(t->r, nxt, size);
	} else {
		if (--size == -1)
			yyerror("VARIABLE COUNT CHANGED!!!\n");
		set_ret_type(t);
		*nxt = t->ret_type;
		return size;
	}
	return size;
}

int main()
{
#ifdef DEBUG_TYPES
	fprintf(stderr,
		"\nPROG\t\t%d\n"
		"VAR\t\t%d\n"
		"PROC\t\t%d\n"
		"FUNC\t\t%d\n"
		"BEG\t\t%d\n"
		"END\t\t%d\n"
		"ID\t\t%d\n"
		"ADDOP\t\t%d\n"
		"MULOP\t\t%d\n"
		"RELOP\t\t%d\n"
		"ASSIGNOP\t\t%d\n"
		"ADD\t\t%d\n"
		"SUB\t\t%d\n"
		"MUL\t\t%d\n"
		"DIV\t\t%d\n"
		"NOT\t\t%d\n"
		"AND\t\t%d\n"
		"OR\t\t%d\n"
		"EQ\t\t%d\n"
		"NE\t\t%d\n"
		"LT\t\t%d\n"
		"LE\t\t%d\n"
		"GT\t\t%d\n"
		"GE\t\t%d\n"
		"INUM\t\t%d\n"
		"RNUM\t\t%d\n"
		"INT\t\t%d\n"
		"REAL\t\t%d\n"
		"BOOL\t\t%d\n"
		"ARRAY\t\t%d\n"
		"OF\t\t%d\n"
		"DOTS\t\t%d\n"
		"IF\t\t%d\n"
		"ELSE\t\t%d\n"
		"THEN\t\t%d\n"
		"WHILE\t\t%d\n"
		"DO\t\t%d\n"
		"FOR\t\t%d\n"
		"TO\t\t%d\n"
		"DT\t\t%d\n"
		"FCALL\t\t%d\n"
		"PCALL\t\t%d\n"
		"ARRAY_ACCESS\t\t%d\n"
		"LIST\t\t%d\n",

		PROG, VAR, PROC, FUNC, BEG, END, ID, ADDOP, MULOP, RELOP, ASSIGNOP, ADD,
		SUB, MUL, DIV, NOT, AND, OR, EQ, NE, LT, LE, GT, GE, INUM, RNUM, INT, REAL,
		BOOL, ARRAY, OF, DOTS, IF, ELSE, THEN, WHILE, DO, FOR, TO, DT, FCALL, PCALL,
		ARRAY_ACCESS, LIST);
#endif

	cur_scope = mkscope();
	assert(cur_scope);

	yyparse();
	free_scope(cur_scope);

	return 0;
}
