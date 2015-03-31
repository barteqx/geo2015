

// Written by Hans de Nivelle, February 2007.
// Rewritten April 2007. Added backward simplification.


#ifndef SIMPLIFICATION_INCLUDED
#define SIMPLIFICATION_INCLUDED   1


#include "partial.h"
#include "rulesystem.h"
#include "varmatching.h" 
#include "varset.h"


namespace simplification
{

   partial< varmatching >
   findselfsubsumption( const rule& self_rule );
      // Returns a varmatching from self_rule into self_rule,
      // s.t. at least one variable of self_rule does not occur in
      // the range.

   partial< varmatching > 
   findsubsumption( const rule& from_rule, const rule& into_rule );
      // Returns a subsuming varmatching in case there exists one. 

   partial< varmatching >
   findsubsumptionwithoutdiseq( const rule& from_rule, const rule& into_rule );
      // Find a matching from from_rule into into_rule, that does not
      // match disequalities. In case no such matching exists, the
      // undefined object is returned. Although inequalities are not
      // matched, it is required that the variables in a 
      // disequality are assigned to distinct variables.
      // (Otherwise, one would construct a trivial simplification) 

   bool check_subsumption( const varmatching& base,
                  const rule& from_rule, const rule& into_rule );



   void addtorulesystem( 
	    std::list< rule > & rules,
	    std::list< std::vector< rulesystem::binary_step > > & derivations,
	    std::vector< predicate > & predicates_base,
	    std::vector< variable > & arguments_base,
	    rulesystem& rs,
	    unsigned int closure_counter );

      // Add rules to rs. Before the rules are added, a lot of 
      // simplifications are done on them. Also, rules from rs may be 
      // backward simplified.

         

   void test_swll_subsumption( const rulesystem& rs );
      // Test whether the first rule in rs subsumes the second
      // rule in rs. 


   void testsimplification( rulesystem& rs ); 

} 



#endif


