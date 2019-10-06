#ifndef TREE_H
#define TREE_H

#include "node.h"

typedef struct parse_tree {
	int type;
	union {
		int ival; /* NUM */
		float rval; /* RNUM */
		node *nval;
		int opval; /* RELOP: LT LE GT GE EQ NE
			      ADDOP: PLUS MINUS OR
			      MULOP: MUL DIV
		*/
	} attr;
	int ret_type;
	struct parse_tree *l, *r;

	int label;
} ptree;

void aux_tree_print(ptree*, int);
void print_tree(ptree*);

ptree* mktree(int, ptree*, ptree*);
ptree* mkid(node*);
ptree* mkinum(int);
ptree* mkrnum(float);
ptree* mkop(int, int, ptree*, ptree*);

void update_type_info(ptree*, ptree*);
void set_ret_type(ptree*);

void free_tree(ptree*);

#endif
