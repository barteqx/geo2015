
// Transformation from an intance of Post Correspondence
// Problem to existence of finite model. I expect that
// geo will not prove this because of depth first search. 

%functions 

%predicates
   lambda /1,
   p /2,

   a/2, b/2, c/2,
 
%firstorder
   <x> p(x,x) /\ lambda(x).

   [x,y] ( p(x,y) /\ ( x != y \/ lambda(x))) ->
       ( < x1, y1, y2 > a(x,x1) /\ a(y,y1) /\ b(y1,y2) /\ p(y1,y2) ) \/
       ( < x1, x2, y1, y2 > b(x,x1) /\ c(x1,x2) /\  
                            b(y,y1) /\ c(y1,y2) /\ p(y1,y2) ) \/ 
       ( < x1, x2, y1 > b(x,x1) /\ a(x1,x2) /\ a(y,y1) /\ p(x2,y1) ).

   [x1,x2] a(x1,x2) /\ lambda(x2) -> FALSE.
   [x1,x2] b(x1,x2) /\ lambda(x2) -> FALSE.
   [x1,x2] c(x1,x2) /\ lambda(x2) -> FALSE.


   [x,y,z] a(x,z) /\ b(y,z) -> FALSE.
   [x,y,z] a(x,z) /\ c(y,z) -> FALSE.
   [x,y,z] b(x,z) /\ c(y,z) -> FALSE.


   [ x1,x2, y1, y2  ] a(x1,x2) /\ a(y1,y2) /\ x2 = y2 -> x1 = y1.
   [ x1,x2, y1, y2  ] b(x1,x2) /\ b(y1,y2) /\ x2 = y2 -> x1 = y1.
   [ x1,x2, y1, y2  ] c(x1,x2) /\ c(y1,y2) /\ x2 = y2 -> x1 = y1.


   [ x1,x2,x3,x4,x5, x6 ] 
              x1 = x2 \/ x1 = x3 \/ x1 = x4 \/ x1 = x5 \/ x1 = x6 \/
                         x2 = x3 \/ x2 = x4 \/ x2 = x5 \/ x2 = x6 \/ 
                                    x3 = x4 \/ x3 = x5 \/ x3 = x6 \/ 
                                               x4 = x5 \/ x4 = x6 \/
                                                          x5 = x6. 


