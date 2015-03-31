
// Code written by Hans de Nivelle, April 2006.

#ifndef SWLL_SEARCHSTATE_INCLUDED
#define SWLL_SEARCHSTATE_INCLUDED   1

#include "occurrencematrix.h"
#include "vardata.h"
#include "lemmasystem.h"
#include "unsignedsubst.h"

#include "../rule.h"
#include "../model.h"
#include "../groundsubst.h"
#include "../varmatching.h"
#include "../partial.h"


namespace swll
{

   class searchstate 
   {

      unsignedsubst solution; 
         // The (possible) solution.

      std::vector< unsigned int > iiii;
      std::vector< variable > vvvv;


      struct clause 
      {
         unsigned int v1, v2;
	    // The variables. These are indices into iiii. 

	 unsigned int i1, i2;
	    // The groups of unsigned integers into which they can 
	    // be matched. If we have (v2-v1) variables, and
	    // n atoms in this clause, then i2 - i1 = n(v2-v1).

	 clause( unsigned int v1, unsigned int v2,
                 unsigned int i1, unsigned int i2 )
            : v1(v1), v2(v2),
              i1(i1), i2(i2)
         {
         }

      };


      std::vector< clause > clauses;

      occurrencematrix occ;
         // Stores for each assignment the list of clauses+offsets in which 
	 // the literal appears. The clauses are stored as
	 // indices in clauses. 


       vardata vd;
          // Attaches to each variable the list of its occurrences,
	  // the set of possible values and a timestamp.


       lemmasystem lemmas;
          // Contains generated lemmas.
       std::vector< lemma::atom > lemma_container; 
          // The container for the atoms of the lemmas. 


   public: 
       searchstate( );
       bool setproblem( const rule& r, const model& m );
          // Initialize from a matching problem. This function 
	  // returns false when the problem is immediately 
	  // unsatisfiable. 

       groundsubst makegroundsubst( ) const;
          // Convert the solution into a groundsubstitution in 
	  // case the problem was initialized from a matching problem
	  // and a solution was found. 

       bool setproblem( const rule& from, const rule& into,
		        partial< variable > avoid );
          // Initialize from a subsumption problem. This function
	  // returns false if the problem is immediately unsatisfiable. 
          // In case avoid is defined, the variable avoid of into will be
	  // avoided. This is useful for strict self-subsumption.

       bool setproblemwithoutdiseq( const rule& from,
		                    const rule& into );
          // Initialize from a subsumption problem. Disequality
	  // atoms are not matched. 

       varmatching makevarmatching( ) const;
          // Convert the solution into a varmatching in
	  // case the problem was initialized from a matching problem
	  // and a solution was found.

       bool interreduce( );
          // Applies an interreduction and return false if the 
          // problem turns out unsatisfiable during this reduction. 

       bool findmodel( );
          // Returns true if it can find a model.

       bool searchsolution( );

       bool verifysolution( ) const; 
      

       friend std::ostream& operator << ( std::ostream& , const searchstate& );

   private:
       partial< lemma > forward( unsigned int solutionbase,
	  std::vector< occurrencematrix::within_clause > :: const_iterator p1,
	  std::vector< occurrencematrix::within_clause > :: const_iterator p2,
	  unsigned int clausenr,
	  unsigned int varnr );

       partial<lemma> forward( unsigned int solutionbase, 
		               unsigned int clausenr,
 			       unsigned int varnr ); 

       partial<lemma> guessclause( unsigned int clausenr,
			           unsigned int varnr );

       partial<lemma> guessvar( unsigned int clausenr,
				unsigned int varnr );


      void addvars( varatom va );
         // Append the variables of va to vvvv.  

        
      void set_occ_vd( );
         // Fill in the occurrence matrix (indicating where each
	 // possible assignment occurs) 
	 // and the vardata attaching to each variable 
	 // the clauses containing it, and the list of its possible
	 // values. 

      void updatepossiblevalues( variable v,
		                 unsigned int updateofneighbour,
				 unsigned int& nrupdatesmade );
         // Recompute the set of possible values for v taking into 
	 // account an update of a neighbour that took place on
	 // time updateofneighbour.

      bool clauseistrue( unsigned int nr ) const;
         // True if the clause with nr is true. 


      static bool avoids( varatom at, const partial< variable > & v );
         // True if the varatom at avoids the possibly undefined
	 // variable v. If v is undefined, avoids( ) is trivially true.
	 // Otherwise the atom should not contain the variable. 

   };


   std::ostream& operator << ( std::ostream& , const searchstate& );

} 


#endif


