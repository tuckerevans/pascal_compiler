#ifndef SEMCHECK_H
#define SEMCHECK_H

#include "scope.h"
#include "tree.h"

void check_id(scope*, char*);

node* check_exists(scope*, char*);

int check_ret_type(ptree*);

void check_call(ptree*);

#endif
