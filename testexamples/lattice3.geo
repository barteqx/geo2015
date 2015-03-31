
%functions 
   meet/2,
   join/2,
   a/0,
   b/0,
   c/0.

%firstorder 

   [x] meet(x,x) = x.
   [x,y] meet(x,y) = meet(y,x).
   [x,y,z] meet(x,meet(y,z)) = meet( meet(x,y), z ).

   meet(a,b) = a.
   meet(b,c) = b.

   ! meet(a,c) = a.




