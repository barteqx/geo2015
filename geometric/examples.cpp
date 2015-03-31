
// A couple of examples:

#include "examples.h"

geometric::rulesystem geometric::simple( signature& sig )
{
   variable v0;
   variable v1 = v0. next( );
   variable v2 = v1. next( );
   variable v3 = v2. next( );
   variable v4 = v3. next( );
   variable v5 = v4. next( );
   variable v6 = v5. next( );
   variable v7 = v6. next( );

   auto A = sig. lookup( { "A" }, 1 );
   auto B = sig. lookup( { "B" }, 1 );
   auto C = sig. lookup( { "C" }, 1 );
   auto D = sig. lookup( { "D" }, 2 );
   auto E = sig. lookup( { "E" }, 1 );

   return { "simple", 
      { existential( { }, v0, varatom( A, truthval::T, { v0 } )),
        disjunctive( { varatom( A, truthval::F, { v0 } ),
                       varatom( B, truthval::T, { v0 } ),
                       varatom( C, truthval::T, { v0 } ) } ),
        lemma( { varatom( A, truthval::F, { v0 } ),
                 varatom( B, truthval::F, { v0 } ) },
                 varatom( C, truthval::T, { v0 } )),
        existential( { varatom( A, truthval::F, { v0 } ),
                       varatom( C, truthval::F, { v0 } ) }, v1, 
                       varatom( D, truthval::T, { v0, v1 } )),
        lemma( { varatom( D, truthval::F, { v0, v1 } ) } ) },
      { lemma( { varatom( A, truthval::F, { v0 } ),
                 varatom( A, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
        lemma( { varatom( D, truthval::F, { v0, v1 } ),
                 varatom( D, truthval::F, { v0, v2 } ),
                 varatom( sig. equality, truthval::T, { v1, v2 } ) } )}};
}




geometric::rulesystem geometric::reflexivity( signature& sig )
{
   variable v0;
   variable v1 = v0. next( );
   variable v2 = v1. next( );
   variable v3 = v2. next( );
   variable v4 = v3. next( );
   variable v5 = v4. next( );
   variable v6 = v5. next( );
   variable v7 = v6. next( );

   auto A = sig. lookup( { "A" }, 1 );
   auto B = sig. lookup( { "B" }, 1 );
   auto C = sig. lookup( { "C" }, 1 );

   auto S = sig. lookup( { "S" }, 2 );

   rulesystem rs( "reflexivity" );

   std::vector<rule> initial = 
   {
      { existential( { }, v0, varatom( A, truthval::T, { v0 } )), 
        partofcode( ) },
      { existential( { }, v0, varatom( B, truthval::T, { v0 } )), 
        partofcode( ) },
      { existential( { }, v0, varatom( C, truthval::T, { v0 } )), 
        partofcode( ) }, 

      { existential( { varatom( sig.create, truthval::F, { v0 } ) },
               v1, varatom( S, truthval::T, { v0, v1 } )), 
           partofcode( ) },
      { lemma( { varatom( A, truthval::F, { v0 } ),
                 varatom( B, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ), 
           partofcode( ) },
      {  lemma( { varatom( B, truthval::F, { v0 } ),
                  varatom( C, truthval::F, { v1 } ),
                  varatom( sig. equality, truthval::T, { v0, v1 } ) } ), 
            partofcode( ) },
      { lemma( { varatom( A, truthval::F, { v0 } ), 
                 varatom( S, truthval::F, { v0, v2 } ),
                 varatom( B, truthval::F, { v1 } ), 
                 varatom( S, truthval::F, { v1, v2 } ) } ), 
            partofcode( ) }
   };

   std::vector<rule> simplifiers = 
   {
      { lemma( { varatom( A, truthval::F, { v0 } ),
                 varatom( A, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
              partofcode( ) },
      { lemma( { varatom( B, truthval::F, { v0 } ),
                 varatom( B, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
              partofcode( ) },

      { lemma( { varatom( C, truthval::F, { v0 } ),
                 varatom( C, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
              partofcode( ) },

      { lemma( { varatom( S, truthval::F, { v0, v1 } ),
                 varatom( S, truthval::F, { v0, v2 } ), 
                 varatom( sig. equality, truthval::T, { v1, v2 } ) } ),
              partofcode( ) } };

   return { "reflexivity", initial, simplifiers };
}


geometric::rulesystem geometric::blz202_5( signature& sig )
{

   // Translation of:

   //   [x,y] E(x) /\ E(y) -> <z> E(z) /\ S(z,x,y).
   //   [x,y] ! E(x) /\ ! E(y) -> <z> E(z) /\ S(z,x,y).
   //   [x,y,v,w] S(v,x,y) /\ S(w,x,y) -> v = w.

   //   ! [x] <y> E(y) /\ [z] S(z,x,x) -> y = z.

   variable v0;
   variable v1 = v0. next( );
   variable v2 = v1. next( );
   variable v3 = v2. next( );
   variable v4 = v3. next( );
   variable v5 = v4. next( );
   variable v6 = v5. next( );
   variable v7 = v6. next( );

   auto pppp0 = sig. lookup( { "pppp0" }, 3 ); 
   auto pppp1 = sig. lookup( { "pppp1" }, 3 );
   auto pppp2 = sig. lookup( { "pppp2" }, 3 );
   auto pppp3 = sig. lookup( { "pppp3" }, 1 );
   auto pppp4 = sig. lookup( { "pppp4" }, 2 );

   auto E = sig. lookup( { "E" }, 1 );
   auto S = sig. lookup( { "S" }, 3 ); 

   rulesystem rs( "blz202(5)", {
      { existential( { varatom( E, truthval::F, { v0 } ),
                       varatom( E, truthval::F, { v1 } ) }, v2, 
                       varatom( pppp0, truthval::T, { v1, v0, v2 } )),
           partofcode( ) },

      { disjunctive( { varatom( pppp0, truthval::F, { v0, v1, v2 } ),
                       varatom( S, truthval::T, { v2, v0, v1 } ) } ), 
           partofcode( ) },
   
      { disjunctive( { varatom( pppp0, truthval::F, { v0, v1, v2 } ),
                       varatom( E, truthval::T, { v2 } ) } ), 
           partofcode( ) },

      { disjunctive( { varatom( sig. create, truthval::F, { v0 } ),
                       varatom( sig. create, truthval::F, { v1 } ),
                       varatom( E, truthval::T, { v0 } ),
                       varatom( E, truthval::T, { v1 } ),
                       varatom( pppp4, truthval::T, { v1, v0 } ) } ), 
           partofcode( ) },

      { existential( { varatom( pppp4, truthval::F, { v0, v1 } ) },
                     v2, 
                     varatom( pppp1, truthval::T, { v0, v1, v2 } )), 
           partofcode( ) },
   
      { disjunctive( { varatom( pppp1, truthval::F, { v0, v1, v2 } ), 
                       varatom( S, truthval::T, { v2, v0, v1 } ) } ), 
           partofcode( ) }, 

      { disjunctive( { varatom( pppp1, truthval::F, { v0, v1, v2 } ),
                       varatom( E, truthval::T, { v2 } ) } ), 
           partofcode( ) },

      { lemma( { varatom( S, truthval::F, { v0, v3, v2 } ),
                 varatom( S, truthval::F, { v1, v3, v2 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ), 
           partofcode( ) },

      { existential( { }, v0, varatom( pppp3, truthval::T, { v0 } )),  
           partofcode( ) },

      { existential( { varatom( pppp3, truthval::F, { v0 } ),
                       varatom( E, truthval::F, { v1 } ) },
                     v2, 
                     varatom( pppp2, truthval::T, { v0, v1, v2 } )), 
           partofcode( ) },

      { lemma( { varatom( pppp2, truthval::F, { v0, v2, v2 } ) } ), 
           partofcode( ) },

      { disjunctive( { varatom( pppp2, truthval::F, { v0, v1, v2 } ), 
                       varatom( S, truthval::T, { v2, v0, v0 } ) } ), 
           partofcode( ) }} );

   return rs; 
}

geometric::rulesystem geometric::demod( signature& sig )
{
   variable v0;
   variable v1 = v0. next( );
   variable v2 = v1. next( );
   variable v3 = v2. next( );
   variable v4 = v3. next( );
   variable v5 = v4. next( );
   variable v6 = v5. next( );
   variable v7 = v6. next( );
   variable v8 = v7. next( );
   variable v9 = v8. next( );
   variable v10 = v9. next( );
   variable v11 = v10. next( );
   variable v12 = v11. next( );
   variable v13 = v12. next( );
   variable v14 = v13. next( );
   variable v15 = v14. next( );
   variable v16 = v15. next( );
   variable v17 = v16. next( );
   variable v18 = v17. next( );
   variable v19 = v18. next( );
   variable v20 = v19. next( );

   auto P_f1 =  sig. lookup( { "P", "f1" }, 3 );
   auto P_c2 =  sig. lookup( { "P", "c2" }, 1 );
   auto P_f4 =  sig. lookup( { "P", "f4" }, 2 );
   auto P_f3 =  sig. lookup( { "P", "f3" }, 3 );
   auto P_c9 =  sig. lookup( { "P", "c9" }, 1 );
   auto P_c10 = sig. lookup( { "P", "c10" }, 1 );
   auto P_c11 = sig. lookup( { "P", "c11" }, 1 );
   auto P_c7 =  sig. lookup( { "P", "c7" }, 1 );
   auto P_c8 =  sig. lookup( { "P", "c8" }, 1 );
   auto P_c5 =  sig. lookup( { "P", "c5" }, 1 );
   auto P_c6 =  sig. lookup( { "P", "c6" }, 1 );

   std::vector<rule> initial =
   {
      { lemma(
          { varatom( P_c2, truthval::F, { v0 } ),
            varatom( P_f1, truthval::F, { v0, v0, v1 } ),
            varatom( P_f1, truthval::F, { v2, v3, v5 } ),
            varatom( P_f1, truthval::F, { v5, v4, v6 } ),
            varatom( P_f1, truthval::F, { v3, v0, v7 } ),
            varatom( P_f1, truthval::F, { v6, v7, v8 } ),
            varatom( P_f1, truthval::F, { v2, v8, v9 } ),
            varatom( P_f1, truthval::F, { v9, v1, v10 } ),
            varatom( sig. equality, truthval::T, { v10, v4 } ) } ),
               partofcode( ) },
     { lemma( 
          { varatom( P_c2, truthval::F, { v0 } ),
            varatom( P_f1, truthval::F, { v0, v0, v1 } ),
            varatom( P_f4, truthval::F, { v2, v3 } ),
            varatom( P_f1, truthval::F, { v2, v3, v4 } ),
            varatom( sig. equality, truthval::T, { v0, v4 } ) } ),
               partofcode( ) },

     { lemma( 
          { varatom( P_c2, truthval::F, { v0 } ), 
            varatom( P_f1, truthval::F, { v0, v0, v1 } ), 
            varatom( P_f1, truthval::F, { v2, v3, v4 } ),
            varatom( P_f4, truthval::F, { v2, v5 } ),
            varatom( P_f4, truthval::F, { v3, v6 } ),
            varatom( P_f3, truthval::F, { v5, v6, v7 } ),
            varatom( sig. equality, truthval::T, { v4, v7 } ) } ),
               partofcode( ) },

      { lemma( 
         { varatom( P_c2, truthval::F, { v0 } ),
           varatom( P_f1, truthval::F, { v0, v0, v1 } ),
           varatom( P_c9, truthval::F, { v2 } ),
           varatom( P_c10, truthval::F, { v3 } ),
           varatom( P_f3, truthval::F, { v2, v3, v4 } ),
           varatom( P_c11, truthval::F, { v5 } ),
           varatom( P_f3, truthval::F, { v4, v5, v8 } ),
           varatom( P_f3, truthval::F, { v3, v5, v7 } ),
           varatom( P_f3, truthval::F, { v2, v7, v8 } ),
           varatom( P_c7, truthval::F, { v9 } ),
           varatom( P_f4, truthval::F, { v9, v10 } ),
           varatom( P_f3, truthval::F, { v10, v9, v11 } ),
           varatom( P_c8, truthval::F, { v12 } ),
           varatom( P_f3, truthval::F, { v11, v12, v12 } ),
           varatom( P_c5, truthval::F, { v14 } ),
           varatom( P_f4, truthval::F, { v14, v15 } ),
           varatom( P_f3, truthval::F, { v15, v14, v19 } ),
           varatom( P_c6, truthval::F, { v17 } ),
           varatom( P_f4, truthval::F, { v17, v18 } ),
           varatom( P_f3, truthval::F, { v18, v17, v19 } ) } ), 
        partofcode( ) }, 

   { existential(
      { varatom( sig. create, truthval::F, { v0 } ),
        varatom( sig. create, truthval::F, { v1 } ) }, 
      v2, varatom( P_f1, truthval::T, { v0, v1, v2 } )), partofcode( ) },

   { existential( 
      { varatom( sig. create, truthval::F, { v0 } ),
        varatom( sig. create, truthval::F, { v1 } ) }, 
      v2, varatom( P_f3, truthval::T, { v0, v1, v2 } )), partofcode( ) },

   { existential( 
      { varatom( sig. create, truthval::F, { v0 } ) },
      v1, varatom( P_f4, truthval::T, { v0, v1 } )), partofcode( ) },

   { existential( 
      { },
      v0, varatom( P_c2, truthval::T, { v0 } )), partofcode( ) },

   { existential( 
      { }, 
      v0, varatom( P_c5, truthval::T, { v0 } )), partofcode( ) }, 

   { existential( 
      { }, 
      v0, varatom( P_c6, truthval::T, { v0 } )), partofcode( ) },

   { existential(
      { }, 
      v0, varatom( P_c7, truthval::T, { v0 } )), partofcode( ) }, 

   { existential( 
      { }, 
      v0, varatom( P_c8, truthval::T, { v0 } )), partofcode( ) }, 

   { existential( 
      { }, 
      v0, varatom( P_c9, truthval::T, { v0 } )), partofcode( ) },

   { existential( 
      { }, 
      v0, varatom( P_c10, truthval::T, { v0 } )), partofcode( ) },

   { existential( 
      { },
      v0, varatom( P_c11, truthval::T, { v0 } )), partofcode( ) } 
   };

   std::vector<rule> simplifiers =
   {
      { lemma( { varatom( P_f1, truthval::F, { v0, v1, v2 } ),
               varatom( P_f1, truthval::F, { v0, v1, v5 } ),
               varatom( sig. equality, truthval::T, { v2, v5 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_c2, truthval::F, { v0 } ),
                 varatom( P_c2, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_f4, truthval::F, { v0, v1 } ),
                 varatom( P_f4, truthval::F, { v0, v3 } ),
                 varatom( sig. equality, truthval::T, { v1, v3 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_f3, truthval::F, { v0, v1, v2 } ),
                 varatom( P_f3, truthval::F, { v0, v1, v5 } ),
                 varatom( sig. equality, truthval::T, { v2, v5 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_c9, truthval::F, { v0 } ),
                 varatom( P_c9, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_c10, truthval::F, { v0 } ),
                 varatom( P_c10, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_c11, truthval::F, { v0 } ),
                 varatom( P_c11, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_c7, truthval::F, { v0 } ),
                 varatom( P_c7, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_c8, truthval::F, { v0 } ),
                 varatom( P_c8, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_c5, truthval::F, { v0 } ),
                 varatom( P_c5, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
        partofcode( ) },
      { lemma( { varatom( P_c6, truthval::F, { v0 } ),
                 varatom( P_c6, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ),
        partofcode( ) } 
   };

   return { "demod", initial, simplifiers };
}


geometric::rulesystem geometric::substitution( signature& sig )
{
   variable v0;
   variable v1 = v0. next( );
   variable v2 = v1. next( );
   variable v3 = v2. next( );
   variable v4 = v3. next( );
   variable v5 = v4. next( );
   variable v6 = v5. next( );
   variable v7 = v6. next( );
   variable v8 = v7. next( );
   variable v9 = v8. next( );
   variable v10 = v9. next( );
   variable v11 = v10. next( );
   variable v12 = v11. next( );
   variable v13 = v12. next( );
   variable v14 = v13. next( );

   auto V = sig. lookup( { "V" }, 1 );
   auto P_subst =  sig. lookup( { "P", "subst" }, 3 );
   auto P_appl =  sig. lookup( { "P", "appl" }, 3 );

   auto pppp0 = sig. lookup( { "pppp0" }, 5 );
   auto pppp1 = sig. lookup( { "pppp1" }, 5 );
   auto pppp2 = sig. lookup( { "pppp2" }, 4 );
   auto pppp3 = sig. lookup( { "pppp3" }, 2 );
   auto pppp4 = sig. lookup( { "pppp4" }, 1 );

   std::vector<rule> initial =
   {
      { disjunctive( { varatom( V, truthval::F, { v0 } ),
                       varatom( V, truthval::F, { v1 } ),
                       varatom( P_subst, truthval::F, { v1, v2, v3 } ),
                       varatom( P_appl, truthval::F, { v3, v0, v4 } ),
                       varatom( V, truthval::F, { v2 } ),
                       varatom( sig. equality, truthval::T, { v0, v1 } ),
                       varatom( sig. equality, truthval::T, { v4, v0 } ) } )
      },
      { disjunctive( { varatom( V, truthval::F, { v1 } ),
                       varatom( P_subst, truthval::F, { v1, v2, v3 } ),
                       varatom( P_appl, truthval::F, { v3, v1, v4 } ), 
                       varatom( V, truthval::F, { v2 } ),
                       varatom( sig. equality, truthval::T, { v4, v2 } )
                     } )
      },
      { existential( { },
                     v0, varatom( pppp4, truthval::T, { v0 } )) },
      { existential( { varatom( pppp4, truthval::F, { v0 } ) },
                     v1, varatom( pppp3, truthval::T, { v1, v0 } )) },
      { disjunctive( { varatom( pppp4, truthval::F, { v0 } ),
                       varatom( V, truthval::T, { v0 } ) } ) },
      { existential( { varatom( pppp3, truthval::F, { v0, v1 } ),
                       varatom( P_subst, truthval::F, { v0, v1, v2 } ) },
                     v3, varatom( pppp2, truthval::T, { v3, v0, v1, v2 } )) },
      { disjunctive( { varatom( pppp3, truthval::F, { v0, v1 } ),
                       varatom( P_subst, truthval::F, { v0, v1, v2 } ),
                       varatom( V, truthval::T, { v0 } ) } ) },
      { existential( { varatom( pppp2, truthval::F, { v0, v1, v2, v3 } ) },
                     v4, varatom( pppp1, truthval::T, { v4, v0, v1, v2, v3 } ))
      },
      { disjunctive( { varatom( pppp2, truthval::F, { v0, v1, v2, v3 } ),
                       varatom( V, truthval::T, { v0 } ) } ) },
      { existential( { varatom( pppp1, truthval::F, { v0, v1, v2, v3, v4 } ),
                       varatom( P_subst, truthval::F, { v0, v1, v5 } ),
                       varatom( P_appl, truthval::F, { v5, v2, v6 } ),
                       varatom( P_appl, truthval::F, { v5, v3, v7 } ),
                       varatom( P_subst, truthval::F, { v6, v7, v8 } ),
                       varatom( P_subst, truthval::F, { v10, v9, v11 } ) },
                     v12, 
                     varatom( pppp0, truthval::T, { v12, v5, v8, v11, v4 } ))},
      { disjunctive( { varatom( pppp1, truthval::F, { v0,v1,v2,v3,v4} ),
                       varatom( P_subst, truthval::F, { v0,v1,v5 } ),
                       varatom( P_appl, truthval::F, { v5, v2, v6 } ),
                       varatom( P_appl, truthval::F, { v5, v3, v7 } ),
                       varatom( P_subst, truthval::F, { v6,v7,v8 } ),
                       varatom( P_subst, truthval::F, { v10, v9, v11 } ),
                       varatom( V, truthval::T, { v10 } ) } ) },
      { disjunctive( { varatom( pppp1, truthval::F, { v0, v1, v2, v3, v4 } ),
                       varatom( P_subst, truthval::F, { v0, v1, v5 } ),
                       varatom( P_appl, truthval::F, { v5, v2, v6 } ),
                       varatom( P_appl, truthval::F, { v5, v3, v7 } ),
                       varatom( P_subst, truthval::F, { v6, v7, v8 } ),
                       varatom( sig. create, truthval::F, { v9 } ),
                       varatom( V, truthval::T, { v9 } ) } ) },
      { disjunctive( { varatom( pppp1, truthval::F, { v0,v1,v2,v3,v4 } ),
                       varatom( P_subst, truthval::F, { v0, v1, v5 } ),
                       varatom( P_subst, truthval::F, { v5, v2, v6 } ),
                       varatom( P_appl, truthval::F, { v5, v3, v7 } ),
                       varatom( P_subst, truthval::F, { v6, v7, v8 } ),
                       varatom( V, truthval::T, { v0 } ) } ) },
      { disjunctive( { varatom( pppp0, truthval::F, { v0,v1,v2,v3,v4 } ),
                       varatom( P_appl, truthval::F, { v1, v0, v5 } ),
                       varatom( P_appl, truthval::F, { v2, v5, v8 } ),
                       varatom( P_appl, truthval::F, { v4, v0, v7 } ),
                       varatom( P_appl, truthval::F, { v3, v7, v8 } ) } ) },
      { disjunctive( { varatom( pppp0, truthval::F, { v0,v1,v2,v3,v4 } ),
                       varatom( P_appl, truthval::F, { v1, v0, v5 } ),
                       varatom( P_appl, truthval::F, { v2, v5, v6 } ),
                       varatom( P_appl, truthval::F, { v4, v0, v7 } ),
                       varatom( P_appl, truthval::F, { v3, v7, v8 } ),
                       varatom( V, truthval::T, { v0 } ) } ) },
      { existential( { varatom( sig. create, truthval::F, { v0 } ),
                       varatom( sig. create, truthval::F, { v1 } ) },
                       v2,
                       varatom( P_subst, truthval::T, { v0, v1, v2 } )) },
      { existential( { varatom( sig. create, truthval::F, { v0 } ),
                       varatom( sig. create, truthval::F, { v1 } ) },
                       v2,
                       varatom( P_appl, truthval::T, { v0, v1, v2 } )) }
   };

   std::vector<rule> simplifiers = 
   {
      { lemma( { varatom( P_appl, truthval::F, { v0, v1, v2 } ),
                 varatom( P_appl, truthval::F, { v0, v1, v5 } ),
                 varatom( sig. equality, truthval::T, { v2, v5 } ) } ) },
      { lemma( { varatom( P_subst, truthval::F, { v0, v1, v2 } ),
                 varatom( P_subst, truthval::F, { v0, v1, v5 } ),
                 varatom( sig. equality, truthval::T, { v2, v5 } ) } ) },
      { lemma( { varatom( pppp0, truthval::F, { v0,v1,v2,v3,v4 } ),
                 varatom( pppp0, truthval::F, { v5,v1,v2,v3,v4 } ),
                 varatom( sig. equality, truthval::T, { v0, v5 } ) } ) },
      { lemma( { varatom( pppp1, truthval::F, { v0,v1,v2,v3,v4 } ),
                 varatom( pppp1, truthval::F, { v5,v1,v2,v3,v4 } ),
                 varatom( sig. equality, truthval::T, { v0, v5 } ) } ) },
      { lemma( { varatom( pppp1, truthval::F, { v0,v1,v2,v3,v4 } ),
                 varatom( pppp1, truthval::F, { v5,v6,v2,v3,v4 } ),
                 varatom( sig. equality, truthval::T, { v1, v6 } ) } ) },
      { lemma( { varatom( pppp1, truthval::F, { v0,v1,v2,v3,v4 } ),
                 varatom( pppp1, truthval::F, { v5,v6,v7,v3,v9 } ),
                 varatom( sig. equality, truthval::T, { v2, v7 } ) } ) },
      { lemma( { varatom( pppp1, truthval::F, { v0,v1,v2,v3,v4 } ),
                 varatom( pppp1, truthval::F, { v5,v6,v7,v8,v9 } ),
                 varatom( sig. equality, truthval::T, { v3, v8 } ) } ) },
      { lemma( { varatom( pppp1, truthval::F, { v0,v1,v2,v3,v4 } ),
                 varatom( pppp1, truthval::F, { v5,v6,v2,v3,v9 } ),
                 varatom( sig. equality, truthval::T, { v4, v9 } ) } ) },
      { lemma( { varatom( pppp2, truthval::F, { v0, v1, v2, v3 } ),
                 varatom( pppp2, truthval::F, { v4, v1, v2, v3 } ),
                 varatom( sig. equality, truthval::T, { v0, v4 } ) } ) },
      { lemma( { varatom( pppp2, truthval::F, { v0, v1, v2, v3 } ),
                 varatom( pppp2, truthval::F, { v4, v5, v2, v7 } ),
                 varatom( sig. equality, truthval::T, { v1, v5 } ) } ) },
      { lemma( { varatom( pppp2, truthval::F, { v0, v1, v2, v3 } ),
                 varatom( pppp2, truthval::F, { v4, v5, v6, v7 } ),
                 varatom( sig. equality, truthval::T, { v2, v6 } ) } ) },
      { lemma( { varatom( pppp2, truthval::F, { v0, v1, v2, v3 } ),
                 varatom( pppp2, truthval::F, { v4, v1, v2, v7 } ),
                 varatom( sig. equality, truthval::T, { v3, v7 } ) } ) },
      { lemma( { varatom( pppp3, truthval::F, { v0, v1 } ),
                 varatom( pppp3, truthval::F, { v2, v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v2 } ) } ) },
      { lemma( { varatom( pppp3, truthval::F, { v0, v1 } ),
                 varatom( pppp3, truthval::F, { v2, v3 } ),
                 varatom( sig. equality, truthval::T, { v1, v3 } ) } ) },
      { lemma( { varatom( pppp4, truthval::F, { v0 } ),
                 varatom( pppp4, truthval::F, { v1 } ),
                 varatom( sig. equality, truthval::T, { v0, v1 } ) } ) }
   };

   return { "subst", initial, simplifiers }; 

}

