

// Written by Hans de Nivelle. Revised April 2007.


// An application is a false_instance decorated with
// some additional statistics.


#ifndef APPLICATION_INCLUDED
#define APPLICATION_INCLUDED   1


#include "false_instance.h"
#include "partial.h"
#include "rulesystem.h"
#include "model.h" 


struct application
{

   false_instance f;

   unsigned int rule_nr;
   unsigned int branching_degree;
      // For a disjunctive rule, the branching degree is
      // the number of conclusions.
      // For an existential rule, the branching degree is
      // the number of elements in the current model + 1.
   
   application( const false_instance& f,
		unsigned int rule_nr, 
		const model& m ) 
   : f(f),
     rule_nr( rule_nr )
   {
      if( f. r. isexistential( ))
         branching_degree = m. nr_elements( ) + 1;
      else
         branching_degree = f. r. nr_conclusions( ); 
   }


   unsigned int getweight( const model& m ) const;
      // Compute the total weight of the matching.
      // It is the sum of the weights of the atom in the range 
      // of the matching, where each atom receives its weight
      // by adding the weights of its parents. 
   unsigned int getlevel( const model& m ) const;
      // Compute the level of the matching. 
   unsigned int getsupatoms( const model& m ) const;
      // Compute the supremum of the matched atoms.
      // (The model is actually not needed to compute this) 

   // The following functions compare applications. Some of them 
   // only look at the rules. Others may look at the application as well. 
   // The comparison functions return  
   //   -1 if left is lighter, 
   //    0 if left and right are considered equal, 
   //    1 if right is lighter. 
   // It is assumed that the lighter application is preferred. 

   static int comparerulesnrpremisses( const application& a1,
		                       const application& a2 );
      // Compare the rules using their number of premisses.
   
   static int comparerulesweight( const application& a1,
		                  const application& a2 );
      // Compare the rules using their weights. 

   static int comparerulesnrvariables( const application& a1,
		                       const application& a2 );
      // Here, we have -1 if a1 has more variables than a2.
      //                0 if numbers are the same.
      //                1 if a1 has less variables than a2.
      // The idea is that more general lemmas should be more preferred.
      // Hence, we treat them as 'lighter'.

   static int comparerandomly( const application& a1,
		               const application& a2 );
      // Decide in a pseudorandom fashion which one is more preferred. 
      // Currently, we look only at the rule numbers.

   static int comparerulesnr( const application& a1,
		              const application& a2 );
      // Prefer the smaller rule number. 

   static int comparematchingsweight( const model& m,
		                      const application& a1,
		                      const application& a2 );
      // Compare the matchings using the weights. The weight is the sum
      // of the weights of the matched atoms. (These are stored in
      // the model) 

   static int comparematchingslevel( const model& m,
		                     const application& a1,
		                     const application& a2 );
      // Compare the matchings using the levels. The level is the
      // smallest natural number that is greater than the levels
      // of the matched atoms. 

   static int comparematchingssupatoms( const model& m,
		                        const application& a1,
		                        const application& a2 );
      // Compare the matchings using the supremum of the numbers of the
      // matched atoms. We actually don't need the model. 

   static int comparematchingssupelements( const model& m,
                                           const application& a1,
                                           const application& a2 );

   static int comparematchingsbranchingdegree( const application& a1,
		                               const application& a2 );
      // Compare the matchings using the branching degrees.
   
   static int compareforward( const model& m,
		              const application& a1,
		              const application& a2 );

   static int compareclosing( const model& m,
		              const application& a1,
		              const application& a2 );

      // These functions make the final decision. 
      // -1 means that a1 is preferred over a2. 

   static bool conclusionistrue( const model& m, const rule& r,
		                 groundsubst& s );
      // true if s makes the conclusion of r true in model m.

   static partial< application > 
      bestforward( const rulesystem& rs, const model& m );
         // Finds the best possible application of a forward reasoning
	 // rule. 


   static partial< application > 
      findclosinglemma( const rulesystem& rs, const model& m, bool best );
         // Returns a closing lemma in case there exists one. 
         // In case best = true, findclosinglemma will try to look
	 // for the best closing lemma. This means: 
         // Smallest, or with smallest number 
	 // of disequalities, smallest number of variables, etc. 

   static partial< application > 
      makeclosurewith( const model& m, const rule& lem, unsigned int nr ); 
         // Construct a closing application from lem if this is possible.
         // The instantiation needs not be the best possible. 


private: 
   void improve( const model& m, partial< application > & best );
      // Try complete the matching in our false_instance, using
      // the rule in the false_instance. If a complete matching exists, 
      // then check if it is better than best. If it is, then
      // replace best. This rules works for lemmas and for forward rules.
      // It either uses compareclosing( ) or compareforward( ). 
 
   void improve( const model& m, 
                 rule::prem_iterator p1,
		 rule::prem_iterator p2,
		 partial< application > & best ); 

public: 
   static void test_swll_match( const model& m, const rulesystem& rs );

};


std::ostream& operator << ( std::ostream& , const application& );


#endif

