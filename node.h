#ifndef NODE_H
#define NODE_H

/* Linked list */

typedef struct node_s {
	char *name;
	struct node_s *next;
} node_t;

/*constructor*/
node_t* mknode(char *);

/* helpers */
node_t* search(node_t*, char *);
node_t* insert(node_t*, char*);

#endif
