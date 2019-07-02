%{

#include "pc.h"
#include "y.tab.h"
#include "sem_check.h"

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

%token <opval> ADDOP
%token <opval> MULOP
%token <opval> RELOP

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
%token ACCESS

%token LIST

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
	}
	|id_list ',' ID
	{
	}
;

var_declarations
	:var_declarations VAR id_list ':' type ';'
	{
	}
	|/*empty*/
;

type
	:standard_type
	{
	}
	|ARRAY '[' INUM DOTS INUM ']' OF standard_type
	{
	}
;

standard_type
	:INT
	{
	}
	|REAL
	{
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
	}
;

optional_statements
	: statement_list
	{
	}
	|/*empty*/
	{
	}
;

statement_list
	:statement
	{
	}
	|statement_list ';' statement
	{
	}
;
statement
	: var ASSIGNOP expression
	{
	}
	|proc_statement
	{
	}
	|compound_statement
	{
	}
	|IF expression THEN statement ELSE statement
	{
	}
	|WHILE expression DO statement
	{
	}
;

var
	:ID
	{
	}
	|ID '[' expression ']'
	{
	}
;

proc_statement
	:ID
	{
	}
	|ID '(' expression_list ')'
	{
	}
;

expression_list
	:expression
	{
	}
	|expression_list ',' expression
	{
	}
;

expression
	:simple_expression
	{
	}
	|simple_expression RELOP simple_expression
	{
	}
;

term
	:factor
	{
	}
	|term MULOP factor
	{
	}
;

factor
	:ID
	{
	}
	|ID '[' expression ']'
	{
	}
	|ID '(' expression_list ')'
	{
	}
	|INUM
	{
	}
	|RNUM
	{
	}
	| '(' expression ')'
	{
	}
	|NOT factor
	{
	}
;

%%
