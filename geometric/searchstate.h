
// Written by Hans de Nivelle, January/February 2015.

#ifndef GEOMETRIC_SEARCHSTATE_INCLUDED
#define GEOMETRIC_SEARCHSTATE_INCLUDED   1

#include "interpretation.h"
#include "timepoints.h"
#include "rulesystem.h"
#include "atom_hasheq.h"
#include "best_false_instances.h" 
#include "deduction.h"


namespace geometric
{

   struct searchstate
   {

      permutationgenerator generator;
         // Permutation generator determines in which order 
         // branches are explored during backtracking.

      rulesystem rs; 

      age starttime;  

      interpretation intp;

      searchstate( const permutationgenerator& gen,
                   const rulesystem& rs, 
                   const std::vector< groundatom > & startatoms )
         : generator{ gen },
           rs{ rs },
           intp{ startatoms } 
      {
      } 

     
      struct timeout 
      {
         unsigned int t;

         explicit timeout( unsigned int t ) : t{t} { } 
      };


      struct instance_checker
      {
         best_false_instances & best;
            // Best false instances that we hope to find.
 
         const searchstate& state;
         const rule& r;         
         
         instance_checker( best_false_instances& best, 
                           const searchstate& state,  
                           const rule& r )  
            : best( best ),
              state( state ), 
              r( r ) 
         { }

         void operator( ) ( const subst<element> & ss );
      };


      
      best_false_instances findfalseinstances( unsigned int nr,
                                               bool onlylemmas ) const;
         // Get the best false instances. 
         // The maximum number that we keep is determined by nr. 
         // If onlylemmas is true, we only look for false lemmas. 

   
      void construct_disjunctive_resolvents( 
         const matched< disjunctive > & disj,
         const std::vector< size_t > & extended, 
         std::vector< false_instance > & chosen,
         const std::vector< best_false_instances > & choices,  
         const rule& mainparent,
         std::initializer_list< rule > firstsideparents,
         best_false_instances& best ) const;


      // The function tries all extensions of disj. It checks
      // for which extensions there are conflict lemmas. It collects
      // a maximum of maxclosing per conflict lemma.
      // After that, it backtracks through all possible combinations of 
      // closing lemmas to collect the best lemmas that can be 
      // obtained by resolution using disj and the closing lemmas. 
      // At most one conclusion of disj may have no closing lemma.
      // In that case, a Horn clause is generated.

      void learn_from_disjunctive( 
         const matched< disjunctive > & disj, 
         unsigned int maxclosing, 
         const rule& mainparent,
         std::initializer_list< rule > firstsideparents,
         best_false_instances& best );
 
      void learn_from_existential( const false_instance& ex,
                                   unsigned int maxclosing, 
                                   best_false_instances& best );
         // Whenever we need closing lemmas, we collect maxclosing
         // closing lemmas. Number shouldn't be too high. 

      void unwindlemmas( best_false_instances& confl );
         // confl should contain only lemmas. At this moment,
         // we have no plan what we do with Horn clauses! 
         // We unwind until either:  
         // 1. We have no extension left, which implies that we are back 
         //    at the initial set of atoms. In that case, confl
         //    contains lemmas that conflict the initial set
         //    of atoms. 
         // 2. We have an extension that is obtained from a guess,
         //    and that is required by all of the lemmas in confl.


      best_false_instances nextguess( );
         // There must be a guesser, and there must be atom extensions 
         // based on this guesser. If there is a next guess, we 
         // retract the current guess, and enter the next guess. 
         // If there is no next guess, we construct new conflict lemmas, 
         // and return them. 

      best_false_instances recheck( best_false_instances inst ) const;
         // Check if the false instances in inst are still false,
         // and check their number of choices. This is done by copying 
         // and reinserting, in order to make sure that result is in
         // proper order. 

      bool makelemma( false_instance& ff ) const;
         // If disj is disjunctive, and has one or zero choices,
         // we turn it into a lemma.
         // If it is a lemma with a conclusion, and it has zero choices,
         // we turn it into a lemma without conclusion.
         // We return true, if we changed ff. 

      bool search( unsigned int maxtime );
         // Try to find a model, maxtime is the maximal time in seconds.
         // If we need more, we throw a timeout. 
         // Return true if we find a model. If we don't find a model,
         // we find a proof, unless timeout. 
   };

   std::ostream& operator << ( std::ostream& out, const searchstate& ss );

}

#endif

