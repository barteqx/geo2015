
// This is not the complete, but a part of the demod.geo problem.

%predicates
 
   f1/ 3, f3/ 3.
   f4 / 2.
   c2/1, c5/1, c6/1, c7/1, c8/1, c9/1, c10/1, c11/1.


%rulesystem

   # V0 /\ # V1 -> # V2 /\ f1(V0,V1,V2).

   # V0 /\ # V1 -> # V2 /\ f3(V0,V1,V2).

   # V0 -> # V1 /\ f4(V0,V1).

      -> # V0 /\ c2(V0).

      -> # V0 /\ c5(V0). 

      -> # V0 /\ c6(V0). 

      -> # V0 /\ c7(V0).

      -> # V0 /\ c8(V0).

      -> # V0 /\ c9(V0).

      -> # V0 /\ c10(V0).

      -> # V0 /\ c11(V0).

   f1(V0,V1,V3) /\ f1(V3,V2,V7) /\ f1(V1,V4,V6) /\ f1(V7,V6,V8) /\ 
   f1(V0,V8,V9) /\ c2(V4) /\ f1(V4,V4,V5) /\ f1(V9,V5,V10) /\ 
         V10 != V2 -> . 

   c2(V3) /\ f4(V0,V1) /\ f1(V0,V1,V2) /\ V3 != V2 -> . 

   f1(V0,V1,V2) /\ f4(V0,V3) /\ f4(V1,V4) /\ f3(V3,V4,V5) /\ V2 != V5 -> . 

   c9(V0) /\ c10(V1) /\ c11(V2) /\ f3(V0,V1,V3) /\ f3(V3,V2,V5) /\ 
   f3(V1,V2,V4) /\ f3(V0,V4,V5) -> . 



%end


