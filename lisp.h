#ifndef LISP_H
#define LISP_H
#include <stdbool.h>

typedef enum {
	T_LIST = 0,
	T_SYMBOL
} SType;

typedef struct S_Pair {
	SType type;
	void *l;
	void *r;
} S;


S *Symbol(const char*);
S *Nil();
void init(void);
bool null(S*);
bool atom(S*);
bool eq(S*, S*);
S *car(S*);
S *cdr(S*);
S *cons(S*, S*);
S *caar(S*);
S *cadr(S*);
S *cadar(S*);
S *caddr(S*);
S *caddar(S*);

void putsS(S*);

S *assoc(S*, S*);
S *list(S*, S*);
S *pair(S*, S*);

S *ff(S*);
S *subst(S*, S*, S*);
bool equal(S*, S*);
S *append(S*, S*);

S *eval(S*, S*);

#endif
