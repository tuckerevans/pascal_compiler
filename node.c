#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "node.h"

/*constructor*/
node* mknode(str)
char *str;
{
	node *p = malloc(sizeof(node));
	assert(p);

	p->name = strdup(str);
	p->next = NULL;

	p->var_type = -1;

	p->func_info = NULL;
	p->array_info = NULL;

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

node* list_insert(root, str) /*TODO change to accept double pointer*/
node *root;
char * str;
{
	node *p = mknode( str );
	p->next = root;
	return p;
}

void free_list(n)
node *n;
{
	node *tmp;

	for(tmp = n; tmp;) {
		n = tmp->next;
		free(tmp);
		tmp = NULL;
		tmp = n;
	}
}
