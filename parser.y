%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lisp.h"

int yylex(void);
void yyerror(char*);
void store(S *);
%}
%union {
	char *symbol;
	S *expr;
}
%token <symbol> SYMBOL
%type <expr> program s_exp list elem atom
%%
program:/*nothing*/					
		| s_exp						{ $$ = $1; store($1);}
		| atom						{ $$ = $1; store($1);}
		| program '\n'				{ return 0; }
		;
s_exp:	'(' list ')'				{ $$ = $2; 		}	
		;
list:	elem						{ $$ = cons($1, Nil()); }
 		| elem list					{ $$ = cons($1, $2); }
 		| elem '\n' list			{ $$ = cons($1, $3); }
		;
elem:	/*nothing*/					{ $$ = Nil(); }
 		| atom						{ $$ = $1; } 
		| s_exp						{ $$ = $1; }
		;
atom:	SYMBOL						{ $$ = Symbol($1); }
		;
%%
char *progname; 	/* for error message */
int lineno = 1;

S* result;

S *parse_stdin() {
	yyparse();
	return result;
}

void store(S *e) {
	result = e;
}

int yylex(void) {
	int c;

	while ((c=getchar()) == ' ' || c == '\t')
		; 
	if (c==EOF)
		return 0;

	char buf[256], *pbuf = buf;
	if (isalpha(c)) {
		do {
			*pbuf++ =c;
			c = getchar();
		} while(isalnum(c));
		ungetc(c, stdin);
		*pbuf = '\0';
		yylval.symbol = strdup(buf);
		return SYMBOL;
	}

	if (c=='\n')
		lineno++;

	return c;
}

void yyerror(char *s) {
	fprintf(stderr, "%s: %s", progname, s);
	fprintf(stderr, " near line %d\n", lineno);
}

int main(int argc, char **argv) {
	init();
	putsS(eval(parse_stdin(), Nil()));
	return 0;
}
