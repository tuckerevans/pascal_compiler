#ifndef SCOPE_H
#define SCOPE_H

#include "node.h"

#define HASH_SIZE 211

typedef struct hash {
	node* table[HASH_SIZE];
	struct hash *prev;
	node* ret_var;
	int offset;
} scope;

scope* mkscope();
void free_scope(scope*);

/*stack routines*/
void pop_scope(scope**);
void push_scope(scope**);

/*helpers*/
node* scope_insert(scope*, char*);
node* scope_search_all(scope*, char*);
node* scope_search(scope*, char*);
node* scope_safe_search(scope*, char*);

void print_scope(scope *);

/*hash function*/
int hashpjw(char*);
#endif
