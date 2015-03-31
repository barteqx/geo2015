
// Code written by Hans de Nivelle, November 2005.
// Modified in May 2006.


#ifndef SEARCH_INCLUDED
#define SEARCH_INCLUDED   1

#include <list>

#include "model.h"
#include "rulesystem.h"
#include "application.h"
#include "partial.h"


namespace search
{

   static unsigned int show_closures = 1000; 
       // A value of N means that once in N times, the 
       // model and the rulesystem are shown. 
       // 1000 is a resonable value. 


   std::list< unsigned int > extendstomodel( model& m, rulesystem& rs );
      // Try to extend m into a complete model of rs. 
      // There are two possible outcomes: 
      // - We manage to obtain a closing lemma for m. It will be added 
      //   to rs, and we return a list of indices in rs of closing lemmas. 
      // - We manage to extend m to a complete model. In that case, 
      //   we return the empty list, and m is a model of rs. 



   std::list< unsigned int > 
                 extenddisjunction(
		             application& disj, 
			     model& m,
			     unsigned int alt,
                             unsigned int altcnt, 
			     unsigned int nr_conclusions, 
			     rulesystem& rs,
                             const std::list< unsigned int > & preferred );
      // disj should be an application of a disjunctive rule.
      // It must be the case that 
      //          0 <= alt, altcnt < nr_conclusions. 
      // We first make the rhs of disj true by adding the alt-th atom of 
      // its rhs to m. After that, we try to extend the result into
      // a complete model. If we succeed, we return the empty list,
      // otherwise we return a list of closing lemmas. 
      // It is possible that we go through the rhs in a permuted order. 
      // altcnt simply counts from left to right, alt is the atom
      // in the rhs that is actually tried.  
      // preferred contains a list of rules that is tried first,
      // because in many cases, closing lemmas can be reused.
      //    (but probably not with disjunctions) 


   std::list< unsigned int >
                 extendexistential(
		             application& existential, 
			     model& m,
			     unsigned int alt,
                             unsigned int altcnt, 
                             unsigned int m_nr_elements, 
			     rulesystem& rs,
                             const std::list< unsigned int > & preferred );

      // existential should be an application of an existential rule.
      // It should be the case that 0 <= alt, altcnt <= m_nr_elements.
      // We make the rhs of existential true true by substituting the alt-th  
      // element of m for the existential quantifier. 
      // altcnt simply counts the attempts from left to right, and 
      // alt is the element that is actually tried. 
      // The elements can be tried in a permuted order. 
      // We return either a non-empty list of closing lemmas, 
      // or the empty list, which means that a model has been found. 
      // preferred contains a list of rules that is tried first, 
      // because in most cases, closing lemmas can be reused. 


   void mergewith( std::list< unsigned int > & list1, 
                   const std::list< unsigned int > & list2 );
      // Add list2 into list1, not adding repeated elements. 


   void printlist( std::ostream& stream, 
                   const std::list< unsigned int > & list );

   std::list< unsigned int > selectclosing( 
                    const std::list< unsigned int > & closing, 
                    const model& m,
                    const rulesystem& rs );
      // closing is a set of indices of rules of rs. 
      // We return the subset of closing that is a closing lemma for
      // m. 
 
} 


#endif


