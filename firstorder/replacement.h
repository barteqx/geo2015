

// Code written by Hans de Nivelle, July 2006.


// Functions that are related to subformula replacement.


#ifndef FIRSTORDER_REPLACEMENT_INCLUDED 
#define FIRSTORDER_REPLACEMENT_INCLUDED   1


#include <map>
#include <list>
#include <vector>


#include "function.h"
#include "formula.h"
#include "sequent.h"
#include "bindingstack.h"


namespace firstorder
{

   namespace replacement 
   {
      
      std::map< function, bool > freevariables( const bindingstack& bound, 
		                                formula f );

      std::map< function, bool > freevariables( 
		                      const bindingstack& bound,
		                      const std::list< formula > & fl );
         // Collect the free variables of f or fl. 

      formula make_atom( const std::list< function > & vars, sequent& seq );
        // Creates a fresh atom p(vars). 

      formula positive_definition( formula atom, formula f );
         // Constructs  [x1...xn] ! p(x1....xn) \/ f.

      formula full_definition( formula atom, formula f );
         // Constructs [x1....xn]  p(x1...xn) <-> f.


      std::list< function > 
      converttolist( const std::map< function, bool > & );
         // When collecting the variables, it is much more efficient to 
	 // construct an std::map< function, bool > . However,
	 // when generating an atom, you rather want an 
	 // std::list< function > , so that you can control the
	 // order of the variables in the atom. 

   } 

} 


#endif


