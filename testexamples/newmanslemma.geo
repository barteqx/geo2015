
%predicates 
   A/1,B/1,C/1.
   S/2,E/2,R/2.
   meetS/3,ERS/2,RS/3.


%rulesystem
   -> # V0 /\ A(V0).
   -> # V0 /\ B(V0).
   -> # V0 /\ C(V0).
   B(V0) /\ C(V1) /\ S(V0,V2) /\ S(V1,V2) -> . 
   A(V0) /\ B(V1) -> S(V0,V1).
   A(V0) /\ C(V1) -> S(V0,V1).
   # V0 -> E(V0,V0).
   E(V0,V1) -> E(V1,V0).
   E(V0,V1) -> S(V0,V1).
   R(V0,V1) -> S(V0,V1).
   S(V0,V1) /\ S(V1,V2) -> S(V0,V2).
   R(V0,V1) /\ R(V0,V2) -> # V3 /\ meetS(V1,V2,V3).
   meetS(V0,V1,V2) -> S(V0,V2).
   meetS(V0,V1,V2) -> S(V1,V2).
   A(V5) /\ R(V5,V0) /\ S(V0,V1) /\ S(V0,V2) -> # V3 /\ meetS(V1,V2,V3).
   S(V0,V1) -> E(V0,V1) \/ ERS(V0,V1).
   ERS(V0,V2) -> # V1 /\ RS(V0,V1,V2).
   RS(V0,V1,V2) -> R(V0,V1).
   RS(V0,V1,V2) -> S(V1,V2).




