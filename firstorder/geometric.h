

// Code written by Hans de Nivelle, July 2006.


#ifndef FIRSTORDER_GEOMETRIC_DEFINED
#define FIRSTORDER_GEOMETRIC_DEFINED   1


// Makes the final conversion from factored formulas
// (called CUDEN-normal form in the paper) to geometric formulas.

// The conditions on the formula f are:
// - f does not contain /\ or TRUE.
// - Existential quantifiers in f must have form <v> F, where v is an atom.
// - f is in negation normal form. 
// - f contains no function symbols. All variables in f are bound.
//
// It appears that FALSE is allowed.


#include <vector>
#include <map>


#include "formula.h"
#include "sequent.h"
#include "bindingstack.h"
#include "../rulesystem.h"


namespace firstorder
{

   namespace geometric
   {

      unsigned int nr_positive_atoms( formula f );
         // f must be in negation normal form.
	 // Equalities (whether positive or negative)
	 // are not counted.

      formula replace_existential( bindingstack& bs, formula f, 
		                   sequent& seq, 
		                   std::list< formula > & definitions );

         // Replace the existential quantifiers by fresh atoms.
	 // This is necessary in case there other positive atoms
	 // in f. 




      // A pseudo_atom is essentially a varatom.
      // The essential difference is that it is an independent object,
      // so that manipulation is more convenient. 


      struct pseudo_atom 
      {
         predicate p;
	 std::list< variable > args;

	 explicit pseudo_atom( predicate p );
	    // The arguments have to be added later by direct
	    // insertion into the list args. 

	 unsigned int getarity( ) const;
         bool check( ) const;
	    // Checks that the arity of p fits to args. size( ).

	 void subst( variable v1, variable v2 );
	    // Replace v1 by v2 in this pseudo_atom.

	 bool subsumes( const pseudo_atom& at ) const;
	    // True if *this and at are identical.
	    //      if *this has form V != W and at has form W != V,
	    //      if *this has form p( .., V, .. ) and at has form # V. 

      };

      bool operator == ( const pseudo_atom& at1, const pseudo_atom& at2 );
      inline 
      bool operator != ( const pseudo_atom& at1, const pseudo_atom& at2 )
      {
         if( at1 == at2 )
            return false;
	 else
            return true;
      }


      std::ostream& operator << ( std::ostream& , const pseudo_atom& va );


      // A pseudo rule is like a rule, but it lives as independent
      // object. 


      struct pseudo_rule 
      {
         std::list< pseudo_atom > prem;
	 std::list< pseudo_atom > conc;

	 bool check( ) const;
	    // Check that the pseudo_rule is well-formed.

	 bool isexistential( ) const;
	    // A pseudo_rule is existential if the conclusion has two atoms,
	    // and the first one has form # V. 

	 bool istautology( ) const;
	    // This is the case if there is an equality V != V among
	    // the premisses, or there is an atom in the 
	    // premisses which also occurs in the conclusion.

	 void simplify( );
            // Removes identical atoms,
	    // and atoms of form # V for which V occurs in a non-equality
	    // atom.

         void insert_to( rulesystem& rs ) const;
	    // Insert this rule into the rulesystem rs.

      };

      std::ostream& operator << ( std::ostream& , const pseudo_rule& r );



      void addtherules( sequent& seq, rulesystem& rs );
         // Convert the formulas in seq, and insert them into rs.
         // The formulas in seq should be factored 
	 // (in CUDEN normal form) 
         // The sequent is not const, because it is still possible
	 // that new names are introduced. 

   } 

} 


#endif


