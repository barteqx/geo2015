

// Code written by Hans de Nivelle, July 2006.


#ifndef FIRSTORDER_FACTOR_INCLUDED
#define FIRSTORDER_FACTOR_INCLUDED   1


#include <map>
#include <list>
#include <vector>


#include "sequent.h"
#include "bindingstack.h"


namespace firstorder
{

   namespace factor 
   {

      extern unsigned int maxfactorsize;
         // The maximal size of a factored disjunction. 
         // It should not be less than 1. 

      
      formula replace_exists( bindingstack& bs, 
		              std::list< function > :: const_iterator p1,
			      std::list< function > :: const_iterator p2,
			      formula body,
			      sequent& seq,
			      std::list< formula > & definitions );
         // The result meets three requirements:
	 // - If it has form <y> F, then F must be an atom.
	 // - It contains no conjunctions. 
         // - It contains no further existential quantifiers. 

      std::list< formula > distr( bindingstack& bs, formula f,
		                  sequent& seq, 
				  std::list< formula > & definitions );
         // Tries to factor conjunction out as much as possible.
	 // The result is a list of formulas that should be interpreted 
	 // as a conjunction. The formulas in the list not contain 
	 // conjunction anymore. The bindingstack records the variables
	 // that are bound around f. 

      void convert_to_cuden( sequent& seq ); 
         // In the result, there are no conjunctions left,
	 // and existential quantifiers are over atoms only. 

   } 

} 


#endif


