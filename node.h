#ifndef NODE_H
#define NODE_H
/*function/array info structs*/
struct fi {
	int argc;
	int *argv;
};

struct ai {
	int size;
	int start_idx;
};

/* Linked list */

typedef struct node_s {
	char *name;
	struct node_s *next;
	int var_type;
	int offset;

	struct fi* func_info;
	struct ai* array_info;
} node;

/*constructor*/
node* mknode(char *);

/* helpers */
node* list_search(node*, char *);
node* list_insert(node*, char*);

void free_list(node*);

#endif
