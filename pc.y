%{
#include <stdlib.h>
#include <stddef.h>

#include "node.h"
#include "scope.h"
#include "tree.h"
#include "y.tab.h"
#include "pc.h"
#include "sem_check.h"

/*
TODO:
- Add checkid() to counter mkid()
*/

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
%type <tval> expr_list

%type <tval> statement
%type <tval> statement_list
%type <tval> compound_statement
%type <tval> opt_statements
%type <tval> proc_statement

%type <tval> var
%type <ival> type
%type <ival> standard_type

%%

program
	:PROG ID '(' id_list ')' ';'
	var_declarations
	sub_prog_declarations
	compound_statement
	'.'
	{
	}
;

id_list
	:ID
	{
		$$ = mkid($1);
	}
	|id_list ',' ID
	{
		$$ = mktree(LIST, $1, mkid($3));
	}
;

var_declarations
	:var_declarations VAR id_list ':' type ';'
	{
		ptree *tmp;
		for(tmp = $3; tmp; tmp = tmp->l) {
			tmp->type = $5;
		}
	}
	|/*empty*/
;

type
	:standard_type
	{
		$$ = $1;
	}
	|ARRAY '[' INUM DOTS INUM ']' OF standard_type
	{
		$$ = ARRAY - $8;
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
	}
;

sub_prog_head
	:FUNC ID arguments ':' standard_type ';'
	{
	}
	|PROC ID arguments ';'
	{
	}
;

arguments
	:'(' param_list ')'
	{
	}
	|/*empty*/
;

param_list
	:id_list ':' type
	{
	}
	|param_list ';' id_list ':' type
	{
	}
;

compound_statement
	:BEG opt_statements END
	{
		$$ = $2;
		print_tree($$);
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
		/*TODO design tree structure for FOR loops*/
		$$ = NULL;
	}
;

TD: TO | DT;

var
	:ID
	{
		$$ = mkid($1);
	}
	|ID '[' expr ']'
	{
		$$ = mktree(ARRAY_ACCESS, mkid($1), $3);
	}
;

proc_statement
	:ID
	{
		$$ = mkid($1);
	}
	|ID '(' expr_list ')'
	{
		$$ = mktree(PCALL, mkid($1), $3);
	}
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
		$$ = mkid($1);
	}
	|ID '[' expr ']'
	{
		$$ = mktree(ARRAY_ACCESS, mkid($1), $3);
	}
	|ID '(' expr_list ')'
	{
		$$ = mktree(FCALL, mkid($1), $3);
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
;

