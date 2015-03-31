
// Written by Hans de Nivelle, Jan-Feb 2015.
// Part of Geo III.

#ifndef GEOMETRIC_DEDUCTION_INCLUDED
#define GEOMETRIC_DEDUCTION_INCLUDED  1

#include "rule.h"
#include "false_instance.h"
#include "matched.h"

namespace geometric
{

   // We first need a couple of inference rules:
   // Horn clause resolution, existential resolution, and 
   // disjunction resolution. They are described in the paper. 

   // A horn_res step consists of a sequence of Horn lemmas that 
   // in the end resolve with a closing lemma.

   struct horn_res : infbase
   {
      std::vector<rule> horn;
      rule clos;

      void flatten( );
         // If close is also an application of Horn res, we merge
         // the two inferences.

      horn_res( const std::vector<rule> & horn, const rule& clos )
         : horn{ horn },
           clos{ clos }
      { 
         flatten( ); 
      }

      horn_res( std::vector<rule> && horn, const rule& clos )
         : horn{ std::move( horn ) },
           clos{ clos }
      { 
         flatten( ); 
      }

      horn_res* clone( ) const override { return new horn_res( *this ); }
      std::vector< rule > getparents( ) const override;
      void print( std::ostream& ) const override;
   };


   struct exdisj_res : infbase
   {
      rule start;                // Either disj or existential. 
      std::vector<rule> lemmas; 

      void flatten( );
         // If start has form exdisj_res( E, L ), then we flatten. 

      exdisj_res( const rule& start, const std::vector<rule> & lemmas )
         : start{ start }, 
           lemmas{ lemmas }
      { 
         flatten( ); 
      }

      exdisj_res( const rule& start, std::vector<rule> && lemmas )
         : start{ start },
           lemmas{ std::move( lemmas ) }
      { 
         flatten( ); 
      }

      exdisj_res* clone( ) const override { return new exdisj_res( *this ); } 
      std::vector< rule > getparents( ) const override;
      void print( std::ostream& out ) const override;
   };


   // Return at[ v1 ==> v2 ].

   varatom replace( varatom at, variable v1, variable v2 );


   bool groundconflict( const groundatom& at1, const groundatom& at2 );
      // True if at1 and at2 are in ground conflict.

   bool iscreate( element e, const groundatom& at );
      // True if at has from #_{F} e.

   bool isequalityfrom( element e, const groundatom& at );
      // True if at has form e = e', where e' is not e. 
   bool isequalityinto( element e, const groundatom& at );
      // True if at has form e' = e, where e' is not e.

   size_t find( size_t val, const std::vector<size_t> & vect );
      // Return index on which val occurs in vector, if it does.
      // Otherwise vect. size( ).

   void transfer( const subst<element> & orig,
                  label lab, 
                  const unifier & unif,
                  normalizer& norm,
                  const set<variable> & varsinresult, 
                  subst<element> & result ); 
      // For every assignment V := t, we compute the instantiation
      // W := t. If W occurs in varsinresult, we put the assignment in
      // result.
      // THIS IS AN UNSATISFACTORY SOLUTION! MAYBE ONE SHOULD
      // RECOMPUTE.
 

   // skolem_constant is the instance of the existential variable
   // in inst_exists.

   matched<disjunctive>
   ex_resolve( const matched<existential> & exists, 
               element skolem_constant, 
               const false_instance& inst_lemma );

   // extensions is a vector of indices of active atoms of inst_disj.
   // (These are the active atoms with which the interpretation can
   //  be extended. The others cause immediate conflict.) 
   // For all possible extensions, except for the last, a lemma must be 
   // present in inst_lemmas. The result is a conflict lemma or a Horn
   // lemma. 
   // Active atoms that do not occur in extensions are moved to the 
   // premiss of the lemma. 

   matched<lemma> 
   disj_resolve( const matched<disjunctive> & disj, 
      const std::vector<size_t> & extensions, 
      const std::vector< false_instance > & inst_lemmas ); 

   matched<lemma>
   horn_resolve( const matched<lemma> & horn,
                 const false_instance& inst_confl );
      // Resolve a Horn clause (lemma) with a conflict lemma.  

   lemma makeconflictlemma( const lemma& lem );
   lemma makeconflictlemma( const disjunctive& disj );
   lemma makehornlemma( const disjunctive& disj, size_t j );

   struct merging
   {
      variable v1;
      variable v2;

      merging( variable v1, variable v2 )
         : v1{v1}, v2{v2}
      { }
   };

   std::ostream& operator << ( std::ostream& , merging );



   struct mergings : infbase
   {
      rule parent;
      std::vector<merging> m;

      mergings( const rule& parent,
                const std::vector<merging> & m )
         : parent{ parent },
           m{m}
      { }

      mergings( const rule& parent,
                std::vector<merging> && m )
         : parent{ parent },
           m{ std::move(m) }
      { }

      mergings* clone( ) const override { return new mergings( *this ); }
      std::vector<rule> getparents( ) const override { return { parent }; }
      void print( std::ostream& ) const override;
   };



   matched<lemma> merge( const matched<lemma> & lem,
                         const std::vector<merging> & eq );

   matched<disjunctive> merge( const matched<disjunctive> & disj,
                               const std::vector<merging> & eq );

}

#endif

