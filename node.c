#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*constructor*/
node* mknode(str)
char *str;
{
	node *p = malloc(sizeof(node));
	assert(p);

	p->name = str;
	p->next = NULL;

	p->var_type = -1;

	p->func_info = NULL;
	p->array_info = NULL;

	p->offset = 0;

	return p;
}

/* helpers */
node* list_search(root, str)
node *root;
char *str;
{
	node *p = root;
	while (p) {
		if (!strcmp(p->name, str)) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}

node* list_insert(root, str)
node *root;
char * str;
{
	node *p = mknode( str );
	p->next = root;
	return p;
}
void free_node(n)
node *n;
{
	free(n->name);
	n->name = NULL;

	if (n->func_info) {
		if(n->func_info->argv)
			free(n->func_info->argv);
		n->func_info->argv = NULL;

		free(n->func_info);
	}
	n->func_info = NULL;

	if (n->array_info)
		free(n->array_info);
	n->array_info = NULL;

	free(n);
	n = NULL;
}

void free_list(n)
node *n;
{
	node *tmp;

	tmp = n;
	while(tmp) {
		n = tmp->next;
		free_node(tmp);
		tmp = n;
	}
}
