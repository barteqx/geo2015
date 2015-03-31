

/* From Logica voor Informatici. */
/* Ex. 5 on page 202.            */

%predicates
   E/1, S/3. 


%firstorder 
 [x,y] E(x) /\ E(y) -> <z> E(z) /\ S(z,x,y).
 [x,y] ! E(x) /\ ! E(y) -> <z> E(z) /\ S(z,x,y).
 [x,y,v,w] S(v,x,y) /\ S(w,x,y) -> v = w.


 ! [x] <y> E(y) /\ [z] S(z,x,x) -> y = z.


