#include "scope.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "pc.h"


scope* mkscope()
{
	int i;
	
	scope *p = malloc(sizeof(scope));
	assert(p);

	for (i = 0; i < HASH_SIZE; i++)
		p->table[i] = NULL;

	p->prev = NULL;
	p->ret_var= NULL;


	return p;
}

void free_scope(s)
scope *s;
{
	int i;

	if (!s)
		return;

	for (i = 0; i < HASH_SIZE; i++) {
		free_list(s->table[i]);
	}

	if (s->ret_var)
		free(s->ret_var);
	s->ret_var = NULL;

	free(s);
	s = NULL;
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

void push_scope(root)
scope **root;
{
	scope *tmp = mkscope();
	
	assert(tmp);

	tmp->prev = *root;
	*root = tmp;
}

void pop_scope(root)
scope **root;
{
	scope *tmp;

	if (!*root)
		return;

	tmp = *root;
	*root = (*root)->prev;

	free_scope(tmp);
}

node* scope_insert(root, name)
scope *root;
char *name;
{
	int hash = hashpjw(name);

	node *tmp = root->table[hash];
	return root->table[hash] = list_insert(tmp, name);
}

node* scope_search(root, name)
scope *root;
char *name;
{
	int hash;
	node *tmp;

	hash = hashpjw(name);
	tmp = root->table[hash];
	return list_search(tmp, name);
}

node* scope_search_all(root, name)
scope *root;
char *name;
{
	scope *p;
	node *tmp;

	for (p = root; p; p = p->prev)
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

	for (p = root; p; p = p->prev) {
		if (tmp = scope_search(p, name))
			return tmp;
		if (p->ret_var)
			return NULL;
	}

	return NULL;
}

void print_scope(s)
scope *s;
{
	int i;
	node * tmp;

	fprintf(stderr, "\n\nSCOPE\n"
	"==========================================================\n");

	for (i = 0; i < HASH_SIZE; i++) {
		for( tmp=s->table[i]; tmp; tmp = tmp->next) {
			if(!tmp->array_info)
			fprintf(stderr, "\t%s:%s\t", tmp->name,
				pretty_type(tmp->var_type));
			else
			fprintf(stderr, "\t%s:%s [%d:%d]\t", tmp->name,
				pretty_type(tmp->var_type),
				tmp->array_info->start_idx,
				tmp->array_info->start_idx
				+ tmp->array_info->size);
			if (tmp->func_info && tmp->func_info->argv) {
			for (int i = 0; i < tmp->func_info->argc; i++)
					fprintf(stderr, " %s ", pretty_type(tmp->func_info->argv[i]));
			}
			fprintf(stderr, "\n");
		}
	}
}
