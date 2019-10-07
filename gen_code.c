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

void gen_code(t, name)
ptree *t;
{
	/*Test gen_label*/
	if (t->type == ASSIGNOP){
		gen_label(t->r);
		print_tree(t->r);
	}
}

