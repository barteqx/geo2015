

%functions 
 
   f1/ 2, f3/ 2.

   f4 / 1.

   c2/0, c5/0, c6/0, c7/0, c8/0, c9/0, c10/0, c11/0.


%firstorder

/*  clause_4, axiom. */
   [X1, X2, X3]  f1(f1(X1,f1(f1(f1(X1,X2),X3),f1(X2,c2))),f1(c2,c2)) = X3.

/* clause_6, axiom.  */
   [X1] c2 = f1(X1,f4(X1)).

/* clause_5, axiom.  */
   [X1,X2] f1(X1,X2) = f3(f4(X1),f4(X2)). 

/* clause_1, axiom.  */



/* clause_12, conjecture. */

   ! f3(f3(c9,c10),c11) = f3(c9,f3(c10,c11)) \/ 
   ! f3(f3(f4(c7),c7),c8 ) = c8 \/ 
   ! f3(f4(c5),c5) = f3(f4(c6),c6). 


%end


