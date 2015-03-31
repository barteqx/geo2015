

// Code written by Hans de Nivelle, July 2006.



#ifndef FIRSTORDER_NNF_INCLUDED
#define FIRSTORDER_NNF_INCLUDED   1


#include <map>
#include <list>


#include "function.h"
#include "formula.h"
#include "sequent.h"
#include "bindingstack.h"


namespace firstorder
{

   // A formula is in nnf if it does not contain -> and <-> and 
   // negation is applied only on atoms or equalities.
   // (also not on truth constants)

   namespace nnf 
   {
      
      formula make_nnf( int polarity, formula f );
         // Returns the nnf of f. The nnf is obtained by
	 // naive factoring which may cause exponential
	 // blow up in case of nested <->'s.
	 // The polarity should be -1 or 1. 

      void make_nnf( sequent& s );
         // Transform all formulas in s into nnf. 

      extern unsigned int max_equivalences; 
         // If an equivalence is encountered, and it is in the scope of 
	 // max_equivalences other equivalences, then its 
	 // non-trival arguments are replaced. 
         // max_equivalences = 0 means that the non-atomic arguments of every
	 // equivalence are replaced.
	 // max_equivalences = 1 means that only in
	 //   A <-> ... ( B <-> C ), the B and C are replaced, etc.


      formula replace_equiv( bindingstack& bs,
		             formula f,
		             unsigned int nr_equivalences,
		             sequent& seq,
		             std::list< formula > & definitions );


      void convert_to_nnf( sequent& seq );
         // Nested equivalences are replaced, the result is converted
	 // to nnf. 

   } 

} 


#endif


