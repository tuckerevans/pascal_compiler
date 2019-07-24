#ifndef TREE_H
#define TREE_H

typedef struct parse_tree {
	int type;
	union {
		int ival; /* NUM */
		float rval; /* RNUM */
		char *sval; /* ID */
		node *nval;
		int opval; /* RELOP: LT LE GT GE EQ NE
			      ADDOP: PLUS MINUS OR
			      MULOP: MUL DIV
		*/
	} attr;
	struct parse_tree *l, *r;
} ptree;

void aux_tree_print(ptree*, int);
void print_tree(ptree*);

ptree* mktree(int, ptree*, ptree*);
ptree* mkid(char*);
ptree* mkinum(int);
ptree* mkrnum(float);
ptree* mkop(int, int, ptree*, ptree*);

#endif
