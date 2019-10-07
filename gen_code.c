#include <stdio.h>
#include "gen_code.h"
#include "pc.h"

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

		break;
	case PCALL:

		break;
	case LIST:

		break;
	case IF:

		break;
	case WHILE:

		break;

	case FOR:
		break;
	default:
		snprintf(buf, 100, "Unknown statement type: %d\n", t->type);
		yyerror(buf);
	}
}

gen_statement_order(t)
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
	printf("\n.globl %s\n.type %s, @function\n%s:\n", name, name, name);

	gen_statement_order(t);
}
