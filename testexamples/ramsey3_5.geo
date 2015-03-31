

%predicates  
   A1/1, A2/1, A3/1, A4/1, A5/1, A6/1.
   R1/2, R2/2, R3/2, R4/2, R5/2. 
   E/2. 
   R/2, G/2.

%rulesystem

   -> # V0 /\ A1(V0).

   A1(V0) -> # V1 /\ R1(V0,V1).
   R1(V0,V1) -> E(V0,V1).
   R1(V0,V1) -> A2(V1).

   A2(V0) -> # V1 /\ R2(V0,V1).
   R2(V0,V1) -> E(V0,V1).
   R2(V0,V1) -> A3(V1).

   A3(V0) -> # V1 /\ R3(V0,V1).
   R3(V0,V1) -> E(V0,V1).
   R3(V0,V1) -> A4(V1).

   A4(V0) -> # V1 /\ R4(V0,V1).
   R4(V0,V1) -> E(V0,V1).
   R4(V0,V1) -> A5(V1).

   A5(V0) -> # V1 /\ R5(V0,V1).
   R5(V0,V1) -> E(V0,V1).
   R5(V0,V1) -> A6(V1).
  
   E(V0,V1) /\ E(V1,V2) -> E(V0,V2).

   E(V0,V1) -> R(V0,V1) \/ G(V0,V1).
   R(V0,V1) /\ R(V0,V2) /\ R(V1,V2) -> .
   G(V0,V1) /\ G(V0,V2) /\ G(V1,V2) -> .

// axiom no_overlap(X,Y) : (r(X,Y),g(X,Y) => false).





