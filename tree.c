#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "node.h"
#include "tree.h"
#include "y.tab.h"
#include "pc.h"

/* parse tree funcs */
ptree* mktree(type, l, r)
int type;
ptree *l, *r;
{
	ptree *t = (ptree*)malloc(sizeof(ptree));
	assert(t);

	t->type = type;
	t->l = l;
	t->r = r;

	return t;
}

ptree* mkid(str)
char *str;
{
	ptree *p = mktree(ID, NULL, NULL);
	p->attr.sval = strdup(str); /* memory leak? double strdup*/
	return p;
}

ptree* mkinum(n)
int n;
{
	ptree *p = mktree(INUM, NULL, NULL);
	p->attr.ival = n;
	return p;
}

ptree* mkrnum(n)
float n;
{
	ptree *p = mktree(RNUM, NULL, NULL);
	p->attr.rval = n;
	return p;
}

ptree* mkop(type, sub, l, r)
int type, sub;
ptree *l, *r;
{
	ptree *p = mktree(type, l, r);
	p->attr.opval = sub;
	return p;
}

void print_tree(t)
ptree *t;
{
	fprintf(stderr, "\n\nTREE\n"
	"==========================================================\n");
	aux_tree_print(t, 0);
	return;
}

void aux_tree_print(t, spaces)
ptree* t;
int spaces;
{
	int i;
	if ( t ) {
		for (i = 0; i < spaces; i++)
			fprintf(stderr," ");
		switch (t->type) {
		
			case ADDOP:
				fprintf(stderr, "[ADDOP]");
				break;
			case MULOP:
				fprintf(stderr, "[MULOP]");
				break;
			case RELOP:
				fprintf(stderr, "[RELOP]");
				break;
			case NOT:
				fprintf(stderr, "[NOT]");
				break;
			case ARRAY_ACCESS:
				fprintf(stderr, "[ARRAY ACCESS]");
				break;
			case LIST:
				fprintf(stderr, "[LIST]");
				break;
			case ID:
				fprintf(stderr, "[ID: %s]", t->attr.sval);
				break;
			case INUM:
				fprintf(stderr, "[INUM: %d]", t->attr.ival);
				break;
			case RNUM:
				fprintf(stderr, "[RNUM: %f]", t->attr.rval);
				break;
			case ASSIGNOP:
				fprintf(stderr, "[ASSIGN]");
				break;
			default:
				fprintf(stderr, "\t%d", t->type);
				yyerror("Error in tree_print");
		}
		fprintf(stderr,"\n");
		aux_tree_print(t->l, spaces + 2);
		fprintf(stderr,"\n");
		aux_tree_print(t->r, spaces + 2);
		fprintf(stderr,"\n");
	}

}
