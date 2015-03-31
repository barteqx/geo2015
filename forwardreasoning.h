
// Written by Hans de Nivelle, November 2005.
// Modified February 2007,
// added equality split, existential split and
// disjunctive split.


#ifndef FORWARDREASONING_INCLUDED
#define FORWARDREASONING_INCLUDED  1



#include <vector>
#include <map>


#include "variable.h"
#include "false_instance.h"
#include "rulesystem.h"


namespace forwardreasoning
{
 
   bool ext_res_possible( const false_instance& existential,
			  const false_instance& lemma,
			  unsigned int branch_border );
      // In the lemma, the rule and the 
      // matching prem_match must be filled in. 

   false_instance existential_resolvent( 
           std::vector< predicate > & predicates_base,
	   std::vector< variable > & arguments_base,
           const false_instance& existential,
           const false_instance& lemma,
	   const model& m, 
	   unsigned int branch_border );  

      // In the lemma and the rule prem_match must
      // be filled in. prem_match is used for determining which
      // premisses should resolve, and for computing the substitution. 
      // In the result, prem_match and the ground substitution
      // will be filled in.


   // We have only binary at this moment:
   //
   // The lemma and the disjunction should have
   // their prem_match filled in.
   // In the result, both prem_match and the ground substitution
   // will be filled in.

   false_instance disjunction_resolvent(
           std::vector< predicate > & predicates_base,
	   std::vector< variable > & arguments_base,
	   const false_instance& disjunction,
	   unsigned int alternative, 
	   const false_instance& lemma, 
	   const model& m, 
	   unsigned int branch_border );



   // sortlemma first deletes create atoms #X for which X occurs
   // in another non-disequality atom.
   // After that it sorts the lemma in such a way
   // that at each point the relevant variables are minimized.
   // This is done heuristically.
   // lookahead determines how far sortlemma looks ahead
   // when deciding how to sort the literals. 

   rule sortlemma( std::vector< predicate > & predicates_base,
                   std::vector< variable > & arguments_base,
                   const rule& lemma,
                   unsigned int lookahead );

   // Return smallest subset of orig that is subsumed by
   // orig. 

   rule condensation(
      std::vector< predicate > & predicates_base,
      std::vector< variable > & arguments_base,
      const rule& orig ); 




   // The varatom must contain exactly one variable V which
   // does not occur in the lemma. It should be not a special atom. 
   // r1 must be an empty rule over predicates_base1, arguments_base1.
   // It will be filled with lemma -> # V /\ va.
   // r2 must be an empty rule over predicates_base2, arguments_base2.
   // It will be filled with lemma /\ va -> False.

   void existential_split( const rule& lemma, 
		           varatom va, 
			   rule& r1, 
		           std::vector< predicate > & predicates_base1,
			   std::vector< variable > & arguments_base1,
			   rule& r2,
			   std::vector< predicate > & predicates_base2,
			   std::vector< variable > & arguments_base2 );


   // disj can also be a lemma. Assume that it has form
   // P -> B1 \/ ... \/ Bq.
   // Then r1 will have form  P -> B1 \/ ... \/ Bq \/ va,
   // and r2 will have form P /\ va -> False. 
   // In case, va contains variables that do not occur in P,
   // create atoms will be inserted in r1, so that the result
   // is guaranteed to be well-formed. 

   void disjunctive_split( const rule& disj,
		           varatom va,
			   rule& r1, 
			   std::vector< predicate > & predicates_base1,
			   std::vector< variable > & arguments_base1,
			   rule& r2,
			   std::vector< predicate > & predicates_base2,
			   std::vector< variable > & arguments_base2 ); 
		           

   // If lemma has form P -> False,
   // then r1 will become P /\ v1 != v2 -> False.
   // and r2 will become P [ v1 := v2 ] -> False. 
   // It may happen that create atoms in r2 have become redundant,
   // so it is probably a good idea to apply sortlemma
   // on r2. It is essential that r1 is not renamed.
   // (only the disequality is added) 

   void disequality_split( const rule& lemma,
	                   variable v1, variable v2,
			   rule& r1, 
			   std::vector< predicate > & predicates_base1,
			   std::vector< variable > & arguments_base1,
			   rule& r2,
			   std::vector< predicate > & predicates_base2,
			   std::vector< variable > & arguments_base2 );


   
   bool decreasefunctionality( const rule& r,
		               varatom conc,
			       unsigned int func ); 
      
   bool decreasefunctionality( const rule& r,
		    std::map< predicate, unsigned int > & occurrences ); 

   void setfunctionalities( const rulesystem& rs );
      // This algorithm needs to be improved. 

   void testing( ); 

   void testsplits( );

}


#endif


