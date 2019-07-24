#ifndef NODE_H
#define NODE_H

/* Linked list */

typedef struct node_s {
	char *name;
	struct node_s *next;
	int var_type;
} node;

/*constructor*/
node* mknode(char *);

/* helpers */
node* search(node*, char *);
node* insert(node*, char*);

#endif
