%{

#include "pc.h"
#include "y.tab.h"
#include "sem_check.h"

/*
TODO:
- Add checkid() to counter mkid()
*/

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
	sub-prog declarations
	compound_statemnt
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
		for(tmp = var_list; tmp != NULL; tmp = tmp->next) {
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
		$$ = $ARRAY - $6;
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

sub-prog_declarations
	:sub-prog_declarations sub-prog_declaration ';'
	{
	}
	|/*empty*/
;
sub-prog_declaration
	:sub-prog_head
	 declarations
	 sub-prog_declarations
	 compound_statument
	{
	}
;

sub-prog_head
	:FUNC ID arguments ':' standard_type ';'
	{
	}
	|PROC ID arguments ';'
	{
	}
;

arguments
	:id_list ':' type
	{
	}
	|param_list ';' id_list ':' type
	{
	}
;

compound_statement
	:BEG optional_statments END
	{
		$$ = $2;
	}
;

optional_statements
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
	: var ASSIGNOP expression
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
	|IF expression THEN statement ELSE statement
	{
		$$ = mktree(IF, $2, mktree(THEN, $4, $6));
	}
	|WHILE expression DO statement
	{
		$$ = mktree(WHILE, $2, $4)
	}
;

var
	:ID
	{
		$$ = mkid($1)
	}
	|ID '[' expression ']'
	{
		$$ = mktree(ARRAY_ACCESS, mkid($1), $3);
	}
;

proc_statement
	:ID
	{
		$$ = mkid($1);
	}
	|ID '(' expression_list ')'
	{
		$$ = mktree(PCALL, mkid($1), $3);
	}
;

expression_list
	:expression
	{
		$$ = $1;
	}
	|expression_list ',' expression
	{
		$$ = mktree(LIST, $1, $3);
	}
;

expression
	:simple_expression
	{
		$$ = $1;
	}
	|simple_expression RELOP simple_expression
	{
		$$ = mkop(RELOP, $2, $1, $3);
	}
;

term
	:factor
	{
		$$ = mktree(LIST, $1, $3);
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
	|ID '[' expression ']'
	{
		$$ = mktree(ARRAY_ACCESS, mkid($1), $3)
	}
	|ID '(' expression_list ')'
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
	| '(' expression ')'
	{
		$$ = $2;
	}
	|NOT factor
	{
		$$ = mktree(NOT, $2, NULL);
	}
;

%%
