#include "tree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "y.tab.h"
#include "scope.h"
#include "pc.h"
#include "sem_check.h"

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
	t->ret_type = 0;
	t->attr.nval = 0;
	t->label = -1;

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

int update_offsets(list, i)
ptree *list;
int i;
{
	if (!list) {
		return i;
	}

	if (list->type != LIST) {
		if (list->type == ID && list->attr.nval) {
			list->attr.nval->offset = i--;
			return i;
		} else {
			yyerror("updating offsets\n");
		}
	}

	i = update_offsets(list->l, i);
	i = update_offsets(list->r, i);
	return i;
}

void update_type_info(list, t)
ptree *list, *t;
{
	int type;
	struct ai *info, *tmp;
	info = tmp = NULL;
	assert(list && t);

	type = t->type;
	if (type != INT && type != REAL){
		assert(info = malloc(sizeof(struct ai)));
		info->size = t->r->attr.ival - t->l->attr.ival;
		info->start_idx = t->l->attr.ival;
	}
	free_tree(t);

	if (list->type == ID) {
		list->attr.nval->var_type = type;
		if (info)
			list->attr.nval->array_info = info;
	} else while (list->r && list->r->type == ID) {
		/*Set type of right child through list*/
		list->r->attr.nval->var_type = type;
		if (info){
			list->r->attr.nval->array_info = info;

			assert(tmp =  malloc(sizeof(struct ai)));
			memcpy(tmp, info, sizeof(struct ai));
			info = tmp;
		}

		if (list->l) {
			if (list->l->type == LIST) {
				list = list->l;
				continue; /*Continue down list*/
			} else if (list->l->type == ID) {
				/*Set type of first declared ID
				    (only left node in LIST)*/
				list->l->attr.nval->var_type = type;
				if (info){
					list->l->attr.nval->array_info = info;

					assert(tmp = malloc(sizeof(struct ai)));
					memcpy(tmp, info, sizeof(struct ai));
					info = tmp;
				}
			}
		}
		break; /*At _end_ of list (did not continue)*/
	}

	return;
}

void set_ret_type(t)
ptree *t;
{
	if (!t)
		return;

	if (! (t->l && t->l->ret_type == 1))
		set_ret_type(t->l);
	if (! (t->r && t->r->ret_type == 1))
		set_ret_type(t->r);

	t->ret_type = check_ret_type(t);

	return;
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
	if (!t)
		return;

	for (i = 0; i < spaces - 1; i++)
		if (!(i%2))
			fprintf(stderr,"|");
		else
			fprintf(stderr," ");
	if (spaces)
		fprintf(stderr,"-");


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
		fprintf(stderr, "[ID: %s %s, ",
			t->attr.nval->name,
			pretty_type(
				t->attr.nval->var_type));
		if (t->attr.nval->func_info)
			fprintf(stderr, "\t %d",
					t->attr.nval->func_info->argc);
		fprintf(stderr, "] O: %d", t->attr.nval->offset);
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
	case IF:
		fprintf(stderr, "[IF]");
		break;
	case THEN:
		fprintf(stderr, "[THEN]");
		break;
	case WHILE:
		fprintf(stderr, "[WHILE]");
		break;
	case FOR:
		fprintf(stderr, "[FOR]");
		break;
	case TO:
		fprintf(stderr, "[TO]");
		break;
	case DT:
		fprintf(stderr, "[DOWN-TO]");
		break;
	case SUB:
		fprintf(stderr, "[SUB]");
		break;
	case PCALL:
		fprintf(stderr,"[P]");
	case FCALL:
		fprintf(stderr, "[CALL]");
		break;
	case INT:
	case REAL:
		fprintf(stderr, "[STD TYPE]");
		break;
	case ARRAY - INT:
	case ARRAY - REAL:
		fprintf(stderr, "[ARRAY]");
		break;
	default:
		fprintf(stderr, "[?: %d]", t->type);
		yyerror("Error in tree_print");
	}
	fprintf(stderr,", T: %d, L: %d\n", t->ret_type, t->label);
	aux_tree_print(t->l, spaces + 2);
	aux_tree_print(t->r, spaces + 2);
}

void free_tree(t)
ptree *t;
{
	if (!t)
		return;

	free_tree(t->l);
	free_tree(t->r);
	free(t);

	/*t->attr.nval freed in free_scope*/
}
