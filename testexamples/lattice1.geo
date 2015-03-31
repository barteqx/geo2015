
%predicates 
   a/1, b/1, c/1.
   leq/2.
   meet/3, join/3.

%rulesystem
   # V0 -> leq(V0,V0). 

   leq(V0,V1) /\ leq(V1,V2) -> leq(V0,V2). 

   leq(V0,V1) /\ leq(V1,V0) /\ V0 != V1 -> . 

   meet(V0,V1,V2) -> leq(V2,V0). 

   meet(V0,V1,V2) -> leq(V2,V1). 

   meet(V0,V1,V2) /\ leq(V3,V0) /\ leq(V3,V1) -> leq(V3,V2). 

   join(V0,V1,V2) -> leq(V0,V2). 

   join(V0,V1,V2) -> leq(V1,V2). 

   join(V0,V1,V2) /\ leq(V0,V3) /\ leq(V1,V3) -> leq(V2,V3). 

   # V0 /\ # V1 -> # V2 /\ meet(V0,V1,V2). 

   # V0 /\ # V1 -> # V2 /\ join(V0,V1,V2). 

    -> # V0 /\ a(V0). 

    -> # V0 /\ b(V0). 

    -> # V0 /\ c(V0). 

   a(V0) /\ b(V1) /\ c(V2) /\ 
      meet(V0,V1,V3) /\ meet(V1,V2,V4) /\ meet(V3,V2,V5) /\ meet(V0,V4,V5) 
         -> . 
%end.


