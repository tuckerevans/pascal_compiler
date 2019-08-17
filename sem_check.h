#ifndef SEMCHECK_H
#define SEMCHECK_H

void check_id(scope*, char*);

node* check_exists(scope*, char*);

int check_ret_type(ptree*);

#endif
