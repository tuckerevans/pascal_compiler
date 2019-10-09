#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "gen_code.h"
#include "pc.h"
#include "scope.h"

#define FLOAT_ERROR "Cannot produce code with reals\n"

scope *cur_scope;

#define REG_SWAP {\
	char *tmp_reg_swap = *reg_ptr;\
	*reg_ptr = *(reg_ptr + 1);\
	*(reg_ptr + 1) = tmp_reg_swap;\
}
#define REG_POP {if (reg_cnt > 0) reg_ptr++, reg_cnt--;\
	else yyerror("CAN'T POP");}
#define REG_PUSH {if (reg_cnt < 13) reg_ptr--, reg_cnt++;\
		else yyerror("CAN'T PUSH");}
#define GEN_EXPR(x) {gen_label(x); gen_expr(x);}

char **reg_stack, **reg_ptr;
int reg_cnt;

int gen_label(t)
ptree *t;
{
	int tmp;

	if (!(t->l && t->r)) {
		t->label = 0;
		return 0;
	}

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

/*Based on Dragon Book gen_code()*/
void gen_expr(t)
ptree *t;
{
	if (!t) {
		fprintf(stderr, "GEN_EXPR: NOT T\n");
		return;
	}

	if ((!t->r) && (!t->l) && t->label == 0){
		switch (t->type) {
		case ID:
			fprintf(stdout, "movq\t%d(%%rbp), %s\n",
					- t->attr.nval->offset * OFFSET_SIZE, *reg_ptr);
			break;
		case INUM:
			fprintf(stdout, "movq\t$%d, %s\n",t->attr.ival, *reg_ptr);
			break;
		default:
			fprintf(stdout, "movq OTHER");
		}
	}
	/*case 0
	 * t is a left leaf*/
	else if ((!t->r) && (!t->l) && t->label != 0) {
		/*TODO check if correct*/
		switch (t->type) {
		case ID:
			fprintf(stdout, "movq\t%s, %s\n",t->attr.nval->name, *reg_ptr);
			break;
		case INUM:
			fprintf(stdout, "movq\t$%d, %s\n",t->attr.ival, *reg_ptr);
			break;
		default:
			fprintf(stdout, "movq OTHER");
		}
	}
	/*case 1
	 * t has a right child with label 0*/
	else if(t->r && t->r->label == 0) {
		GEN_EXPR(t->l);
		if (t->r && t->r->type == INT) {
			fprintf(stdout, "op\t$%d,\n", t->r->attr.ival);
			fprintf(stdout, "op\t$%d,%s\n", t->r->attr.ival, *reg_ptr);
		} else {
			yyerror(FLOAT_ERROR);
		}
	}
	/*case 2
	 * 1 <= t->l->label < t->r->label AND t->l->lable < reg_cnt*/
	else if ((t->l->label <= 1 && t->l->label < t->r->label)
			&& t->l->label < reg_cnt) {

		REG_SWAP
		GEN_EXPR(t->l);
		REG_POP
		GEN_EXPR(t->r);
		fprintf(stdout, "op\t%s, %s\n", *(reg_ptr + 1), *reg_ptr);
		REG_PUSH
		REG_SWAP
	}
	/*case 3
	 * 1 <= t->r->label <= t->l->label */
	else if (t->r->label <= 1 && t->r->label <= t->l->label) {
		GEN_EXPR(t->l);
		REG_POP
		GEN_EXPR(t->r);
		fprintf(stdout, "op\t%s, %s\n", *reg_ptr, *(reg_ptr + 1));
		REG_PUSH
	}
	/*case 4
	 *t->l->label, t->r->label >= reg_cnt*/
	else if (t->l->label >= reg_cnt && t->r->label >= reg_cnt){
		/*TODO implement case 4*/
		yyerror("CASE 4 of gen_expr() NOT IMPLEMENTED\n");
	} else {
		yyerror("NOT ONE OF DEFINED CASES [gen_expr()]\n");
	}

}

void gen_arguments(t)
ptree *t;
{
	if (t->type != LIST){
		GEN_EXPR(t)
		fprintf(stdout, "pushq\t%s", *reg_ptr);

		fprintf(stdout, "\n##ARGUMENT BOUNDARY\n");
		return;
	}

	gen_arguments(t->r);
	gen_arguments(t->l);
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
			gen_label(t->r);
			print_tree(t);
			GEN_EXPR(t->r);
			fprintf(stdout, "movq %s, %d(%%rbp)\n", *reg_ptr,
					- t->l->attr.nval->offset * OFFSET_SIZE);
		} else {
			yyerror(FLOAT_ERROR);
			fprintf(stderr, "ASSIGN (REAL) %s\n", t->l->attr.nval->name);
		}
		break;
	case PCALL:
		if (t->r->ret_type == REAL)
				yyerror(FLOAT_ERROR);

		if (!strcmp(t->l->attr.nval->name, "write")) {
			GEN_EXPR(t->r);
			fprintf(stdout, "mov %s, %%rsi\n", *reg_ptr);
			fprintf(stdout, "leaq int_print(%%rip), %%rdi\n");
			fprintf(stdout, "mov $0, %%rax\n");
			fprintf(stdout, "call printf\n");
		} else if (!strcmp(t->l->attr.nval->name, "read")) {
			fprintf(stderr,"Read\n");
		}
		fprintf(stderr, "PCALL\n");
		break;
	case FCALL:
		if (t->l->ret_type == INT) {
			fprintf(stderr, "FCALL (INT) %s\n", t->l->attr.nval->name);
		} else {
			fprintf(stderr, "FCALL (REAL) %s\n", t->l->attr.nval->name);
			yyerror(FLOAT_ERROR);
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
			yyerror(FLOAT_ERROR);
		}

		break;
	case WHILE:
		if (t->l->l->ret_type == INT) {
			fprintf(stderr, "WHILE (INT)\n");
		} else {
			fprintf(stderr, "WHILE (REAL)\n");
			yyerror(FLOAT_ERROR);
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
		fprintf(stdout, "\n##STATEMENT BOUNDARY\n");
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
	reg_stack[0] = "%rax";  reg_stack[1] = "%rcx";
	reg_stack[2] = "%rdx";  reg_stack[3] = "%rsi";
	reg_stack[4] = "%rdi";  reg_stack[5] = "r8";
	reg_stack[6] = "r9";   reg_stack[7] = "%r10";
	reg_stack[8] = "%r11";  reg_stack[9] = "%r12";
	reg_stack[10] = "%r13"; reg_stack[11] = "%r14";
	reg_stack[12] = "%r15";

	reg_ptr = reg_stack;
	reg_cnt = 13;

	fprintf(stdout, "\n.globl %s\n", name);
	fprintf(stdout, ".type %s, @function\n%s:\n", name, name);

	fprintf(stdout, "pushq\t%%rbp\nmovq\t%%rsp, %%rbp\n");
	fprintf(stdout, "subq\t $%d, %%rsp\n", cur_scope->offset * OFFSET_SIZE);
	gen_statement_order(t);

	fprintf(stdout, "movq\t%%rbp, %%rsp\npop\t%%rbp\nret\n\n");

}
