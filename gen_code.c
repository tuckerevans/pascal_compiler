#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gen_code.h"
#include "pc.h"

#define REG_SWAP {\
	char *tmp_reg_swap = *reg_ptr;\
	*reg_ptr = *(reg_ptr + 1);\
	*(reg_ptr + 1) = tmp_reg_swap;\
}
#define REG_POP {if (reg_cnt > 0) reg_ptr++, reg_cnt--;\
	else yyerror("CAN'T POP");}
#define REG_PUSH {if (reg_cnt < 13) reg_ptr--, reg_cnt++;\
		else yyerror("CAN'T PUSH");}

char **reg_stack, **reg_ptr;
int reg_cnt;

int gen_label(t)
ptree *t;
{
	int tmp;

	if (!(t->l && t->r))
		return 0;

	if (t->l) {
		tmp = gen_label(t->l);
		t->l->label = (!tmp ? 1 : tmp);
	} else
		yyerror("GEN_LABEL: left child NULL, shouldn't happen!\n");

	if (t->r)
		t->r->label = gen_label(t->r);
	else
		t->r->label = 0;


	if (t->r->label == t->l->label) {
		return 1 + t->l->label;
	} else {
		return t->r->label > t->l->label ? t->r->label : t->l->label;
	}

}

void gen_expr(t)
ptree *t;
{
}

void gen_statement(t)
ptree *t;
{
	char buf[100];

	if (!t)
		return;

	switch (t->type){
	case ASSIGNOP:
		if (t->l->ret_type == INT) {
			fprintf(stderr, "ASSIGN (INT) %s\n", t->l->attr.nval->name);
		} else {
			fprintf(stderr, "ASSIGN (REAL) %s\n", t->l->attr.nval->name);
		}
		break;
	case PCALL:
		fprintf(stderr, "PCALL\n");
		break;
	case FCALL:
		if (t->l->ret_type == INT) {
			fprintf(stderr, "FCALL (INT) %s\n", t->l->attr.nval->name);
		} else {
			fprintf(stderr, "FCALL (REAL) %s\n", t->l->attr.nval->name);
		}
		break;
	case LIST:
		yyerror("Issue with statement code generation\n");
		break;
	case IF:
		if (t->l->l->ret_type == INT) {
			fprintf(stderr, "IF (INT)\n");
		} else {
			fprintf(stderr, "IF (REAL)\n");
		}

		break;
	case WHILE:
		if (t->l->l->ret_type == INT) {
			fprintf(stderr, "WHILE (INT)\n");
		} else {
			fprintf(stderr, "WHILE (REAL)\n");
		}

		break;

	case FOR:
		fprintf(stderr, "FOR\n");
		break;
	default:
		snprintf(buf, 100, "Unknown statement type: %d\n", t->type);
		yyerror(buf);
	}
}

void gen_statement_order(t)
ptree *t;
{
	if (t->type != LIST){
		gen_statement(t);
		return;
	}

	gen_statement_order(t->l);
	gen_statement_order(t->r);

}

void gen_code(t, name)
ptree *t;
char *name;
{
	reg_stack = malloc(13 * sizeof(char*));
	assert(reg_stack);
	reg_stack[0] = "RAX";  reg_stack[1] = "RCX";
	reg_stack[2] = "RDX";  reg_stack[3] = "RSI";
	reg_stack[4] = "RDI";  reg_stack[5] = "R8";
	reg_stack[6] = "R9";   reg_stack[7] = "R10";
	reg_stack[8] = "R11";  reg_stack[9] = "R12";
	reg_stack[10] = "R13"; reg_stack[11] = "R14";
	reg_stack[12] = "R15";

	reg_ptr = reg_stack;
	reg_cnt = 13;

	fprintf(stdout, "\n.globl %s\n", name);
	fprintf(stdout, ".type %s, @function\n%s:\n", name, name);

	gen_statement_order(t);
}
