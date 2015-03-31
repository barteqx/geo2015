

// Written by Hans de Nivelle, July 2006.


#ifndef FIRSTORDER_SEQUENT_INCLUDED
#define	FIRSTORDER_SEQUENT_INCLUDED    1


#include <list>
#include <map>

#include "formula.h"


namespace firstorder
{

   class sequent
   {
   public: 
      std::list< formula > formulas;
         // Our philosophy is that the formula-list is an open 
	 // datastructure. This means of course that the user 
	 // can modify the formulas without updating
	 // usedpredicates, usedfunctions,
	 // so be careful. 
         
   private:
      std::map< predicate, bool > usedpredicates; 
         // usedpredicates contains all predicates that occur 
	 // in the formula. 
      std::map< function, bool > usedfunctions; 
         // usedfunctions contains all functions that occur
	 // in the formulua. These include the quantified variables,
	 // which are considered 0-arity functions.

      unsigned int formulapredicate_cnt;
      unsigned int termvariable_cnt; 
         // These counters are used for the generation of fresh 
	 // predicates, functions. 


   public: 
      void collectusedidentifiers( );
         // If you forget to collect the used identifiers,
	 // then the generation of fresh names will not
	 // work correctly. 

      predicate freshsubformulapredicate( unsigned int arity ); 
         // Produces a fresh predicate for representing a subformula.
         // It will have form P#. 

      predicate freshfunctionpredicate( function f );
         // Produces a predicate that can represent function f 
         // in the translation. It will have form f_. 

      function freshtermvariable( ); 
         // Produces a fresh termvariable. It will have form
         // alpha#. 


      friend std::ostream& operator << ( std::ostream& stream,
		                         const sequent& s );

      sequent( )
         : formulapredicate_cnt(0),
           termvariable_cnt(0)
      { } 

   private:
      void addidentifiers( formula f );
      void addidentifiers( term t );
   };

} 


#endif

