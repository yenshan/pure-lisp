#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lisp.h"

static S *ATOM, *EQ, *CAR, *CDR, *CONS, *QUOTE, *COND, *LABEL, *LAMBDA;
static S *T, *F, *NIL;

void init(void) {
	ATOM = Symbol("ATOM");
	EQ = Symbol("EQ");
	CAR = Symbol("CAR");
	CDR = Symbol("CDR");
	CONS = Symbol("CONS");
 	QUOTE = Symbol("QUOTE");
	COND = Symbol("COND");
	LABEL = Symbol("LABEL");
	LAMBDA = Symbol("LAMBDA");
	T = Symbol("T");
	F = Symbol("F");
	NIL = Nil();
}

S *Elem(SType type) {
	S *e = malloc(sizeof(S));
	memset(e, 0, sizeof(S));
	e->type = type;
	return e;
}

S *Symbol(const char *s) {
	S *e = Elem(T_SYMBOL);
	e->l = strdup(s);
	return e;
}

S *Nil() {
	return Symbol("NIL");
}

void p(S *e) {
	if (e == NULL)
		return;

	if(atom)
		printf("%s", (char*)e->l);
	else {
		printf("(");
		p( e->l );
		printf(".");
		p( e->r );
		printf(")");
	}
}

void putsS(S *e) {
	p(e); printf("\n");
}

//
// S-Functions
//
bool atom(S *e) {
	if (e == NULL)
		return false;
	return (e->type != T_LIST);
}

bool eq(S *x, S *y) {
	if (atom(x) && atom(y)) {
		if (x->type == T_SYMBOL && y->type == T_SYMBOL)
			return (strcmp(x->l, y->l)==0);
	} 
	return false;
}

S *car(S *e) {
	if (e == NULL || atom(e)) {
		return NULL;
	}
	return e->l;
}

S *cdr(S *e) {
	if (e == NULL || atom(e)) {
		return NULL;
	}
	return e->r;
}

S *cons(S *l, S *r) {
	S *e = Elem(T_LIST);
	e->l = l;
	e->r = r;
	return e;
}

bool null(S *e) {
	return eq(e, NIL);
}

S *caar(S *e) {
	return car(car(e));
}

S *cadr(S *e) {
	return car(cdr(e));
}

S *cadar(S *e) {
	return car(cdr(car(e)));
}

S *caddr(S *e) {
	return car(cdr(cdr(e)));
}

S *caddar(S *e) {
	return car(cdr(cdr(car(e))));
}

S *ff(S *e) {
	if (atom(e)) 
		return e;
	else
		return ff(car(e));
}

S *subst(S *x, S *y, S *z) {
	if (atom(z)) {
		if (eq(z,y))
			return x;
		else
			return z;
	} else {
		return cons(subst(x,y,car(z)), subst(x,y,cdr(z)));
	}
}

bool equal(S *x, S *y) {
	if (atom(x) && atom(y))
		return eq(x,y);
	else if (!atom(x) && !atom(y))
		return (equal(car(x), car(y)) && equal(cdr(x), cdr(y)));
	else
		return false;
}

S *append(S *x, S *y) {
	if(null(x)) 
		return y;
	if(null(y)) 
		return x;
	if (atom(x) || atom(y))
		return NULL; // undefined
	return cons(car(x), append(cdr(x),y));
}

S *assoc(S *x, S *y) {
	if (x == NULL || y == NULL) {
		return NULL;
	}
	if (null(x))
		return x;
	if (null(y)) {
		printf("undefined: %s\n", x->l); 
		return NIL;
	}
	if (eq(caar(y), x))
		return cadar(y);
	return assoc(x, cdr(y));
}

S *list(S* e1, S *e2) {
	return cons(e1, cons(e2, NIL));
}

S *pair(S* x, S *y) {
	if (null(x) && null(y))
			return NIL;
	else if (!atom(x) && !atom(y)) 
		return cons(list(car(x), car(y)),
			 pair(cdr(x), cdr(y)));
	else
		return NIL;
}

//
// eval
//
S *evcon(S*, S*);
S *evlis(S*, S*);

S *eval(S *e, S *a) {
	if (atom(e))
		return assoc(e, a);
	else if (atom(car(e))) {
		if (eq(car(e), QUOTE)) {
			return cadr(e);
		} else if (eq(car(e), ATOM)) {
			return atom(eval(cadr(e), a))? T : F;
		} else if (eq(car(e), EQ)) {
			return eq( eval(cadr(e),a),
					   eval(caddr(e),a))? T : F;
		} else if (eq(car(e), COND)) {
			return evcon(cdr(e),a);	
		} else if (eq(car(e), CAR)) {
			return car(eval(cadr(e),a));
		} else if (eq(car(e), CDR)) {
			return cdr(eval(cadr(e),a));
		} else if (eq(car(e), CONS)) {
			return cons(eval(cadr(e),a), eval(caddr(e),a));
		} else if (eq(car(e), NIL)) {
			return NIL;
		} else {
			return eval(
		//			cons(assoc(car(e),a), evlis(cdr(e),a)),  // McCarthy's paper
					cons(assoc(car(e),a), cdr(e)), // Paul Graham's implementation
				   	a);  
		}
	} else if (eq(caar(e), LABEL)) {
		return eval(cons(caddar(e), cdr(e)), 
			        cons(list(cadar(e), car(e)), a));
	} else if (eq(caar(e), LAMBDA)) {
		return eval(caddar(e), 
				    append( pair(cadar(e), evlis(cdr(e),a)), a)
					);
	}
	printf("eval ERROR: "); putsS(e);
	exit(1);	
}

S *evcon(S *c, S *a) {
	if (eq(eval(caar(c),a), T))
		return eval(cadar(c),a);
	else
		return evcon(cdr(c),a);
}

S* evlis(S *m, S *a) {
	if (null(m))
		return NIL;
	else
		return cons(eval(car(m),a), evlis(cdr(m),a));
}

