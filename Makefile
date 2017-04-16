CC = gcc
OPTS = -std=c99

all: lisp

lisp: lisp.h lisp.c y.tab.c 
	$(CC) $(OPTS) y.tab.c lisp.c -o $@

y.tab.c: parser.y
	yacc $<

clean:
	rm -f lisp
	rm -f y.tab.c
