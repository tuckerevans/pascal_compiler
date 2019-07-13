#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "node.h"

/*constructor*/
node_t* mknode(str)
char *str;
{
	node_t *p = malloc(sizeof(node_t));
	assert(p);

	p->name = strdup(str);
	p->next = NULL;

	return p;
}

/* helpers */
node_t* search(root, str)
node_t *root;
char *str;
{
	node_t *p = root;
	while (p) {
		if (!strcmp(p->name, str)) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}

node_t* insert(root, str) /*TODO change to accept double pointer*/
node_t *root;
char * str;
{
	node_t *p = mknode( str );
	p->next = root;
	return p;
}
