#include <assert.h>
#include <stdio.h>

#include "node.h"
#include "scope.h"
#include "tree.h"
#include "y.tab.h"
#include "pc.h"
#include "sem_check.h"

void check_id(s, n)
scope *s;
char *n;
{
	char buf[100];

	if (scope_search(s, n)) {
		snprintf(buf, 100, "\"%s\" already defined in scope...\n", n);
		yyerror(buf);
	}
}

node* check_exists(s, n)
scope *s;
char *n;
{
	node *tmp;
	char buf[100];

	if(!(tmp = scope_search(s,n))) {
		snprintf(buf, 100, "Cannot find \"%s\"\n", n);
		yyerror(buf);
	}

	return tmp;
}

int check_ret_type(t)
ptree *t;
{
	char buf[100];
	int type;

	if (!t)
	printf("TYPE: %d\n", t->type);

	switch (t->type) {
	case ID:
		if (!(t->attr.nval))
			yyerror("Missing ID\n");

		return t->attr.nval->var_type;

	case ADDOP :
	case MULOP :
		if (!(t->r && t->l))
			yyerror("Missing nodes\n");

		if (t->r->ret_type == t->l->ret_type)
			return t->r->ret_type;
		else {
			snprintf(buf, 100, "Mismached types: "
					"Type %s "
					"cannot be used with type %s\n",
					pretty_type(t->r->ret_type),
					pretty_type(t->l->ret_type));
			yyerror(buf);
		}
		break;
	case RELOP :
		if (!(t->r && t->l))
			yyerror("Missing nodes\n");

		if (t->r->ret_type == t->l->ret_type)
			return BOOL;
		else
			snprintf(buf, 100, "Mismached types: "
					"Type %s "
					"cannot be compared to type %s\n",
					pretty_type(t->r->ret_type),
					pretty_type(t->l->ret_type));
			yyerror(buf);
		break;
	case NOT:
		if (t->ret_type == BOOL)
			return BOOL;
		yyerror("NOT needs bool input\n");
		break;
	case INUM:
		return INT;
	case RNUM:
		return REAL;
	case ASSIGNOP:
		if (!(t->r && t->l && t->r->attr.nval))
			yyerror("Incomplete parse tree\n");

		if (t->l->attr.nval->var_type == t->r->ret_type)
			return 0;
		else {
			snprintf(buf, 100, "Mismached types: "
					"Cannot assign type %s "
					"to variable \"%s\" of type %s\n",
					pretty_type(t->r->ret_type),
					t->l->attr.nval->name,
					pretty_type(t->l->attr.nval->var_type));
			yyerror(buf);
		}


		break;
	case ARRAY_ACCESS:
		if (!(t->r && t->l && t->l->attr.nval))
			yyerror("Incorrect Array Access\n");

		type = t->l->attr.nval -> var_type;
		if (type == ARRAY - INT || type == ARRAY - REAL)
			return 0;
		break;
	default:
		return -200;
		snprintf(buf, 101, "Unknown tree node: %d...\n", t->type);
		yyerror(buf);
	}


	return -1;

}

