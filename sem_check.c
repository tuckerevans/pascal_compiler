#include "sem_check.h"

#include <assert.h>
#include <stdio.h>

#include "y.tab.h"
#include "pc.h"

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
		fprintf(stderr, "TYPE: %d\n", t->type);

	switch (t->type) {
	case ID:
		if (!(t->attr.nval))
			yyerror("Missing ID\n");

		return t->attr.nval->var_type;

	case ADDOP :
	case MULOP :
		if (!(t->r && t->l))
			yyerror("Missing nodes\n");

		if (t->attr.opval == AND || t->attr.opval == OR) {
			if(t->l->ret_type == BOOL && t->r->ret_type ==BOOL)
				return BOOL;
			else {
				type = t->l->ret_type == BOOL ?
					t->r->ret_type : t->l->ret_type;

				snprintf(buf, 100, "Mismached types:"
						"Cannot use boolean "
						"operator on type %s\n",
						pretty_type(type));
				break;
			}
		}

		if (t->r->ret_type == t->l->ret_type)
			return t->r->ret_type;
		else
			snprintf(buf, 100, "Mismached types: "
					"Type %s "
					"cannot be used with type %s\n",
					pretty_type(t->r->ret_type),
					pretty_type(t->l->ret_type));

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
		break;
	case NOT:
		if (t->l && t->l->ret_type == BOOL)
			return BOOL;
		yyerror("NOT needs bool input\n");
		break;
	case INUM:
		return INT;
	case RNUM:
		return REAL;
	case ASSIGNOP:
		if (!(t->r && t->l))
			yyerror("Incomplete parse tree\n");

		if (t->l->ret_type == t->r->ret_type)
			return 1;
		else
			snprintf(buf, 100, "Mismached types: "
					"Cannot assign type %s "
					"to variable \"%s\" of type %s\n",
					pretty_type(t->r->ret_type),
					t->l->attr.nval->name,
					pretty_type(t->l->attr.nval->var_type));



		break;
	case ARRAY_ACCESS:
		if (!(t->r && t->l && t->l->attr.nval))
			yyerror("Incorrect Array Access\n");

		if (t->r->ret_type != INT) {
			snprintf(buf, 100, "Cannot access array"
					"with type %s\n",
					pretty_type(t->r->ret_type));
			break;
		}

		type = t->l->attr.nval -> var_type;
		if (type == ARRAY - INT || type == ARRAY - REAL)
			return ARRAY - type;
		break;
	case IF:
	case WHILE:
		if (!(t->r && t->l))
			yyerror("Incomplete parse tree\n");

		if (t->l->ret_type != BOOL)
			yyerror("If condition must be of type BOOL\n");
		return 1;
	case FOR:
		/*
		                  FOR (0)
		                 /   \
		            TD(0)     STATEMENT(0)
		           /     \
		ASSIGNOP(0)       INUM(INT)
		*/
		if (!(t->r && t->l))
			yyerror("Missing nodes\n");
		if (t->l->ret_type == 1 && t->r->ret_type == 1)
			return 1;
		snprintf(buf, 100, "Incorrect types in for statement...\n");
		break;
	case TO:
	case DT:
		if (!(t->r && t->l))
			yyerror("Missing nodes\n");

		if (t->l->ret_type == 1 && t->r->ret_type == INT)
			return 1;
		snprintf(buf, 100, "Incorrect types HERE...\n");
	case SUB:
		return t->l->ret_type;
		break;
	default:
		return -200;
		snprintf(buf, 100, "Unknown tree node: %d...\n", t->type);
	}

	yyerror(buf);
	return -1;

}

