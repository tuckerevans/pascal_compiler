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

ptree* mkid(n)
node *n;
{
	ptree *p = mktree(ID, NULL, NULL);
	p->attr.nval = n;
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

void update_type_info(list, type)
int type;
ptree *list;
{
	assert(list);
	while (list->r && list->r->type == ID) {
		/*Set type of right child through list*/
		list->r->attr.nval->var_type = type;
		
		if (list->l) {
			if (list->l->type == LIST) {
				list = list->l;
				continue; /*Continue down list*/
			} else if (list->l->type == ID)
				/*Set type of first declared ID
				    (only left node in LIST)*/
				list->l->attr.nval->var_type = type;
		}
		return; /*At _end_ of list (did not continue)*/
	}
}


/*PRINT FUNCS*/

void print_tree(t)
ptree *t;
{
	fprintf(stderr, "\n\nTREE\n"
	"==========================================================\n");
	aux_tree_print(t, 0);
	fprintf(stderr,
	"**********************************************************\n");
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
				if (t->r && t->r->attr.nval)
					fprintf(stderr, "[ID: %s %s]",
						t->r->attr.nval->name,
						pretty_type(
							t->attr.nval->var_type));
				else
					fprintf(stderr, "[ID: %s %s]",
						t->attr.nval->name,
						pretty_type(
							t->attr.nval->var_type));
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
