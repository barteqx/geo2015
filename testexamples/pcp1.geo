
// Transformation from an intance of Post Correspondence
// Problem to unsatisfiability.

%functions 
   lambda / 0, 
   a/ 1, b/1, c/1,
   zero/0, s/ 1.

%predicates
   p /3.

%firstorder
   p( lambda, lambda, zero ).

   [x,y,n] p(x,y,n) -> p(a(x), b(a(y)), s(n)).
   [x,y,n] p(x,y,n) -> p(c(b(x)), c(b(y)), s(n)).
   [x,y,n] p(x,y,n) -> p(a(b(x)), a(y), s(n)).

   [ x, n ] !  p(x,x,s(n)). 


