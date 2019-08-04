#include <assert.h>
#include <stdio.h>

#include "node.h"
#include "scope.h"
#include "tree.h"
#include "y.tab.h"
#include "pc.h"
#include "sem_check.h"

void check_id(s, n)
scope *s;
char *n;
{
	char buf[100];

	if (scope_search(s, n)) {
		snprintf(buf, 100, "\"%s\" already defined in scope...\n", n);
		yyerror(buf);
	}
}

node* check_exists(s, n)
scope *s;
char *n;
{
	node *tmp;
	char buf[100];

	if(!(tmp = scope_search(s,n))) {
		snprintf(buf, 100, "Cannot find \"%s\"\n", n);
		yyerror(buf);
	}

	return tmp;
}
