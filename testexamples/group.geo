

%functions 
   one/0.
   inverse/1.
   prod/2.

%firstorder
   [x] prod(x,one) = x.
   [x] prod(one,x) = x.
   [x,y,z] prod( prod(x,y), z ) = prod( x, prod(y,z)).
   [x] prod( x, inverse(x)) = one.
   [x] prod( inverse(x), x ) = one.

   [x] prod(x,x) = one.

   ! [x,y] prod(x,y) = prod(y,x).


