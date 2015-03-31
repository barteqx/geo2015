

// Code written by Hans de Nivelle, July 2006.



#ifndef FIRSTORDER_ANTISKOLEMIZATION_INCLUDED
#define FIRSTORDER_ANTISKOLEMIZATION_INCLUDED   1


#include <map>
#include <list>
#include <vector>


#include "function.h"
#include "formula.h"
#include "sequent.h"


namespace firstorder
{

   namespace antiskolemization 
   {

      struct functionreplacement
      {
         std::map< function, predicate > repr;

	 bool contains( function f ) const;

	 void ensure( function f, sequent& seq );
	    // If there is no [f] yet, then one
	    // will be created. The sequent seq is
	    // needed in order to ensure that [f] is 
	    // fresh, in case [f] is created. 
	    // The sequent seq cannot be const, because the new
	    // name will be added as a name that is in use.

         predicate lookup( function f ) const;


         static formula seriality_axiom( predicate p );
            // Constructs a seriality axiom for predicate p. 

	 void addseriality_axioms( sequent& seq ) const;
	    // Add the serial axioms to seq. 

      };


      std::ostream& operator << ( std::ostream& stream,
		                  const functionreplacement& f );  


      struct levelvar
      {
         int level;
	    // -1: does not depend on a bound variable,
	    // 0: depends on the first bound variable,
	    // 1: depends on the second bound variable,
	    // 2: depends on the third bound variable, etc.

         function var;

         levelvar( int level,
                   function var )
            : level( level ),
	      var( var )
	                                                                                 {
	 }

      };



      functionreplacement functionlist( sequent& seq ); 
         // The functionreplacement assigns to each function symbol in s 
	 // a new predicate symbol. If the function symbol has
	 // arity a, then the predicate symbol receives arity
	 // a + 1. 



      // A term replacement assigns fresh variables to terms. 
      // Quantified variables are assigned to themselves.

      class termreplacement
      {
      private:
         std::map< term, std::list< levelvar > > vars;
            // Assigns to terms a list (with descending levels)
	    // of the variables that are assigned at a certain level.

         std::vector< std::list< term > > stack;
	    // Contains for each level a list of terms that
	    // are defined at that level.

         std::list< term > toplevel;
	    // The terms that are defined on level -1.


      public: 
	 term lookup( term t, sequent& seq );
	    // Lookup the variable for t at the present level.
	    // If required, an entry is created. 


	 void addquantifiedvar( function v );
	    // Assign v to itself at the highest level. This
	    // has to be done with quantified variables, so
	    // that terms containing this variable will receive
	    // the correct level. 

	 void addlevel( );
	    // Add a level.

	 void removelastlevel( );
	    // Remove the last level.

         formula defining_atom( const functionreplacement& r,
			        sequent& seq,
				term t );

	 formula add_defining_atoms( const functionreplacement& repl,
			             sequent& seq,
				     formula f );
	    // Replace f by 
	    // [x1,...xn] D(x1), ... D(xn) -> f, where
	    // D(x1)...D(xn) are the definitions of the last level. 

	 std::list< term > :: const_iterator lastlevel_begin( ) const;
	 std::list< term > :: const_iterator lastlevel_end( ) const;
	 std::list< term > :: const_iterator toplevel_begin( ) const;
	 std::list< term > :: const_iterator toplevel_end( ) const;


	 friend std::ostream& operator << ( std::ostream&,
			                    const termreplacement& ); 
      };

      std::ostream& operator << ( std::ostream& , 
		                  const termreplacement& );



      void antiskolemize( sequent& seq );
         // Antiskolemize the formulas in seq. They must
	 // be in negation normal form.

   } 

} 


#endif


