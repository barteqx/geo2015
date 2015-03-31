

/* From Logica voor Informatici. */
/* Ex. 5 on page 202.            */

%predicates
   V/1.

%functions
   subst / 2,    
   appl / 2.

%firstorder

   // Standard characterization of subst:

   [ x1, x2, v ] V(x1) /\ V(x2) /\ V(v) ->
      ( v != x1 ) -> appl( subst( x1, x2 ), v ) = v.
   [ x1, x2, v ] V( x1 ) /\ V( x2 ) /\ V( v ) ->
      ( v = x1 ) -> appl( subst( x1, x2 ), v ) = x2.


   ! [ x1, x2, y1, y2 ] V( x1 ) /\ V( x2 ) /\ V( y1 ) /\ V( y2 ) ->
      < z1, z2 > V( z1 ) /\ V( z2 ) -> 
         [ v ] V(v) ->
	    appl( subst(  appl( subst( x1, x2 ), y1 ),
	                  appl( subst( x1, x2 ), y2 )),
               appl( subst( x1, x2 ), v )) =
            
	    appl( subst( z1, z2 ), appl( subst( y1, y2 ), v )).


