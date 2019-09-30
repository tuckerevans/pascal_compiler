%{
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "node.h"
#include "scope.h"
#include "tree.h"
#include "y.tab.h"
#include "pc.h"
#include "sem_check.h"


extern int yylex();
extern scope *cur_scope;

%}

%union {
	int ival;
	float rval;
	char *sval;
	int opval;
	struct parse_tree *tval;
}

%token PROG
%token VAR
%token PROC FUNC
%token BEG END
%token <sval> ID

%token <opval> ADDOP
%token <opval> MULOP
%token <opval> RELOP
%token ASSIGNOP

%token ADD SUB
%token MUL DIV

%token NOT
%token AND OR

%token EQ NE
%token LT LE
%token GT GE

%token <ival> INUM
%token <rval> RNUM

%token INT REAL
%token BOOL

%token ARRAY OF
%token DOTS

%token IF ELSE THEN
%token WHILE DO
%token FOR TO DT

%token FCALL PCALL
%token ARRAY_ACCESS

%token LIST


%type <tval> factor
%type <tval> term
%type <tval> expr
%type <tval> simple_expr

%type <tval> id_list
%type <tval> param_list
%type <tval> arguments
%type <tval> expr_list

%type <tval> statement
%type <tval> statement_list
%type <tval> compound_statement
%type <tval> opt_statements
%type <tval> proc_statement

%type <tval> var
%type <tval> type
%type <ival> standard_type

%type <ival> TD

%%

program
	:PROG ID '(' id_list ')' ';'
	var_declarations
	sub_prog_declarations
	compound_statement
	'.'
	{
		set_ret_type($9);
#ifdef DEBUG
		print_tree($9);
#endif
		free_tree($9);
		free_tree($4);
#ifdef DEBUG
		print_scope(cur_scope);
#endif
		pop_scope(&cur_scope);
	}
;

id_list
	:ID
	{
		/*TODO remove check_ids*/
		node *tmp;
		check_id(cur_scope, $1);

		tmp = scope_insert(cur_scope, $1);
		$$ = mkid(tmp);
	}
	|id_list ',' ID
	{
		node *tmp;

		check_id(cur_scope, $3);
		tmp = scope_insert(cur_scope, $3);
		$$ = mktree(LIST, $1, mkid(tmp));
	}
;

var_declarations
	:var_declarations VAR id_list ':' type ';'
	{
		update_type_info($3, $5);
		free_tree($3);
	}
	|/*empty*/
;

type
	:standard_type
	{
		$$ = mktree($1, NULL, NULL);
	}
	|ARRAY '[' INUM DOTS INUM ']' OF standard_type
	{
		$$ = mktree(ARRAY - $8, mkinum($3), mkinum($5));
	}
;

standard_type
	:INT
	{
		$$ = INT;
	}
	|REAL
	{
		$$ = REAL;
	}
;

sub_prog_declarations
	:sub_prog_declarations sub_prog_declaration ';'
	{
	}
	|/*empty*/
;
sub_prog_declaration
	:sub_prog_head
	 var_declarations
	 sub_prog_declarations
	 compound_statement
	{
		set_ret_type($4);
#ifdef DEBUG
		print_tree($4);
#endif
		free_tree($4);
#ifdef DEBUG
		print_scope(cur_scope);
#endif
		pop_scope(&cur_scope);
	}
;

/*push_scope called in pc.l*/
sub_prog_head
	:FUNC ID arguments ':' standard_type ';'
	{
		node *tmp;
		int i = 0;

		check_id(cur_scope->prev, $2);
		tmp = scope_insert(cur_scope->prev, $2);

		i = count_args($3);

		tmp->func_info = malloc(sizeof(struct fi));
		assert(tmp->func_info);
		tmp->func_info->argc = i;
		assert(tmp->func_info->argv = malloc(i * sizeof(int)));

		assert(!set_func_types($3, tmp->func_info->argv, i));
		free_tree($3);

		tmp->var_type = $5;

		/* Need to duplicate ID.name so it is not freed with inner
		 * scope*/
		cur_scope->ret_var = scope_insert(cur_scope, strdup($2));
		cur_scope->ret_var->var_type = $5;
	}
	|PROC ID arguments ';'
	{
		node *tmp;
		int i = 0;

		check_id(cur_scope->prev, $2);
		tmp = scope_insert(cur_scope->prev, $2);

		i = count_args($3);

		tmp->func_info = malloc(sizeof(struct fi));
		assert(tmp->func_info);
		tmp->func_info->argc = i;
		assert(tmp->func_info->argv = malloc(i * sizeof(int)));

		assert(!set_func_types($3, tmp->func_info->argv, i));
		free_tree($3);
	}
;

arguments
	:'(' param_list ')'
	{
		$$ = $2;
	}
	|/*empty*/{
		$$ = NULL;
	}
;

param_list
	:id_list ':' type
	{
		$$ = $1;
		update_type_info($1, $3);
	}
	|param_list ';' id_list ':' type
	{
		$$ = mktree(LIST, $1, $3);
		update_type_info($3, $5);
	}
;

compound_statement
	:BEG opt_statements END
	{
		$$ = $2;
	}
;

opt_statements
	: statement_list
	{
		$$ = $1;
	}
	|/*empty*/
	{
		$$ = NULL;
	}
;

statement_list
	:statement
	{
		$$ = $1;
	}
	|statement_list ';' statement
	{
		$$ = mktree(LIST, $1, $3);
	}
;
statement
	: var ASSIGNOP expr
	{
		$$ = mktree(ASSIGNOP, $1, $3);
	}
	|proc_statement
	{
		$$ = $1;
		check_call($$);
	}
	|compound_statement
	{
		$$ = $1;
	}
	|IF expr THEN statement ELSE statement
	{
		$$ = mktree(IF, $2, mktree(THEN, $4, $6));
	}
	|WHILE expr DO statement
	{
		$$ = mktree(WHILE, $2, $4);
	}
	|FOR var ASSIGNOP expr TD expr DO statement
	{
		/*
		              FOR
		             /   \
		          TD       STATEMENT
		        /    \
		ASSIGNOP       INUM
		*/
		ptree *tmp;

		tmp = mktree(ASSIGNOP, $2, $4);
		tmp = mktree($5, tmp, $6); //$5 is TD

		$$ = mktree(FOR, tmp, $8);
	}
	| expr
	{
		$$ = $1;
	}
;

TD
	:TO
	{
		$$ = TO;
	}
	|DT
	{
		$$ = DT;
	}
;

var
	:ID
	{
		$$ = mkid(scope_safe_search(cur_scope,$1));
		free($1);
	}
	|ID '[' expr ']'
	{
		node* tmp;
		tmp = scope_safe_search(cur_scope, $1);

		$$ = mktree(ARRAY_ACCESS, mkid(tmp), $3);
		$$->attr.nval = $$->l->attr.nval;
		free($1);
	}
;

proc_statement
	:ID
	{
		node *tmp;

		tmp = check_exists(cur_scope, $1);
		$$ = mktree(PCALL, mkid(tmp), NULL);
		free($1);
	}
	|ID '(' expr_list ')'
	{
		node *tmp;

		tmp = check_exists(cur_scope, $1);
		$$ = mktree(PCALL, mkid(tmp), $3);

		free($1);
	}
	/*calls checked with proc_statement*/
;

expr_list
	:expr
	{
		$$ = $1;
	}
	|expr_list ',' expr
	{
		$$ = mktree(LIST, $1, $3);
	}
;

expr
	:simple_expr
	{
		$$ = $1;
	}
	|simple_expr RELOP simple_expr
	{
		$$ = mkop(RELOP, $2, $1, $3);
	}
;

simple_expr
	:term
	{
		$$ = $1;
	}
	|simple_expr ADDOP term
	{
		$$ = mkop(ADDOP, $2, $1, $3);
	}
;

term
	:factor
	{
		$$ = $1;
	}
	|term MULOP factor
	{
		$$ = mkop(MULOP, $2, $1, $3);
	}
;

factor
	:ID
	{
		node *tmp;

		tmp = check_exists(cur_scope, $1);
		$$ = mkid(tmp);
		free($1);
	}
	|ID '[' expr ']'
	{
		node *tmp;

		tmp = check_exists(cur_scope, $1);
		$$ = mktree(ARRAY_ACCESS, mkid(tmp), $3);

		free($1);
	}
	|ID '(' expr_list ')'
	{
		node *tmp;

		tmp = check_exists(cur_scope, $1);
		$$ = mktree(FCALL, mkid(tmp), $3);
		check_call($$);

		free($1);
	}
	|INUM
	{
		$$ = mkinum($1);
	}
	|RNUM
	{
		$$ = mkrnum($1);
	}
	| '(' expr ')'
	{
		$$ = $2;
	}
	|NOT factor
	{
		$$ = mktree(NOT, $2, NULL);
	}
	|ADDOP factor{
		if ($1 != SUB)
			yyerror("SUB NOT CORRECT\n");
		else
			$$ = mktree(SUB, $2, NULL);
	}
;

