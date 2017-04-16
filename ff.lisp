((LABEL ff (LAMBDA (x) 
			(COND 
			 ((ATOM x) x)
			 ((QUOTE T) (ff (CAR x))))))
 (QUOTE ((a b) (c d))))
