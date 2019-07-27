#include <stdio.h>
#include <stdilb.h>
#include <assert.h>
#include <string.h>

#include "node.h"
#include "hash.h"

scope* mkscope(prev)
scope* prev;
{
	int i;
	
	scope *p = malloc(sizeof(scope));
	assert(p);

	for (i = 0; i < HASH_SIZE; i++)
		p->table[i] = NULL;

	p->next = next;
	p->function_boundry = 0;

	return p;
}

void free_scope(s)
scope *s;
{
	int i;

	if (!s)
		return;

	for (i = 0; i < HASH_SIZE; i++) {
		free_nodes(s->table[i]);
	}

	free(s);
}

/*Copied from Compilers, Aho*/
#define EOS '\0'
int hashpjw(s)
char* s;
{
	char *p;
	unsigned h = 0, g;

	for (p = s; *p != EOS; p++) {
		h = (h<<4) + *p;
		if (g = h & 0xf0000000) {
			h^= g>>24;
			h^= g;
		}
	}

	return h % HASH_SIZE;
}

scope* push_scope(root);
scope* root;
{
	scope *p = mkscope();
	p->next = root;
	return p;
}

scope* pop_scope(root);
scope *root;
{
	scope *p;

	if (!root)
		return NULL;

	p = root->next;

	free_scope(root);
	return p;
}

node* scope_insert(s, name)
scope *s;
char *name;
{
	int hash = hashpwj(name);

	node *tmp = root->table[hash];
	return root->table[hash] = list_insert(tmp, name);
}

node* scope_search(root, name)
scope *root;
char *name;
{
	int hash = hashpwj(name);

	node *tmp = root->table[hash];
	return list_search(tmp, name);
}

node* scope_search_all(root, name)
scope *root;
char *name;
{
	scope *p;
	node *tmp;

	for (p = root; p; p = p->next)
		if (tmp = scope_search(p, name))
			return tmp;

	return NULL;
}

node* scope_safe_search(root, name)
scope *root;
char *name;
{
	scope *p;
	node *tmp;

	for (p = root; p; p = p->next) {
		if (tmp = scope_search(p, name))
			return tmp;
		if (p->f)
			return NULL
	}

	return NULL;
}

void print_scope(s)
scope *s;
{
	int i;
	node_t * tmp;
	for (i = 0; i < HASH_SIZE; i++) {
		for( tmp=s->table[i]; tmp; tmp = tmp->next) {
			fprintf(stderr, "\t%s\n", tmp->name);
		}
	}
}
