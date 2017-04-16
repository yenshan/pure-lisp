# pure-lisp
This is a experimental small implementation of John McCarthy's LISP interpreter in C.
No arithmetic function, no GC, no symbol table, no error handling...

This is not a practical programming language.
I just want to show the simpleness of LISP, and for one who want to know how to implement programming language.
For this reason, this program is intent to be as simple and as readable as possible.

## How to build
You can build this program on any UNIX like Operation System. Checkout from github and type following command.

$ make

Executable file ``lisp`` will be created.

## Test
This program only read lisp source code from stdin.
To interpret ``ff.lisp``, type following command .

```
$ cat ff.lisp | ./lisp
a
```

You can see the result of 'a'.

### Sample LISP code

ff.lisp
```lisp
((LABEL ff (LAMBDA (x)
			(COND
			 ((ATOM x) x)
			 ((QUOTE T) (ff (CAR x))))))
 (QUOTE ((a b) (c d))))
```


## reference
- John McCarthy, [Recursive Functions of Symbolic Expressions and their Computation by Machine (Part I).](http://www-formal.stanford.edu/jmc/recursive.html)
- Paul Graham, [The Roots of Lisp](http://www.paulgraham.com/rootsoflisp.html)
