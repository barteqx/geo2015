
/* Uit logica voor informatici, blz. 202.   */

%predicates
   N/1, S/2.
%firstorder
  [x,y] N(x) /\ S(x,y) -> N(y).
  [x,y] S(x,y) -> ! ( x = y ).
  [x] N(x) -> <y> S(x,y).
  ! [x] N(x) -> <y> N(y) /\ ! ( x = y ).

