

// Code written by Hans de Nivelle, June 2006.


#ifndef FIRSTORDER_FORMULA_INCLUDED
#define FIRSTORDER_FORMULA_INCLUDED    1


#include "../predicate.h"
#include "term.h"



namespace firstorder
{

   enum fol_oper
   {
      fol_false, fol_true,

      fol_not,

      fol_and, fol_or,
      fol_implies, fol_equiv,

      fol_forall, fol_exists,

      fol_equals, fol_atom
   }; 
	     

   class formula
   {

      struct formula_node
      {
         unsigned int reference_count;

         fol_oper op; 

         std::list< function > variables;
            // The quantified variables in case of fol_forall, fol_exists.

	 std::list< predicate > pred;
	    // Empty list in all cases, except for fol_atom.
	    // Then we have here a single predicate with which the
	    // atom is built.

         std::list< term > subterms;
            // Two subterms in case of fol_equals.
	    // An arbitrary number of subterms in case of fol_atom.
            // Otherwise empty.

         std::list< formula > subformulas;
            // 0 subformulas in case of fol_false, fol_true, fol_equals, 
	    //                          fol_atom.
	    // 1 subformula in case of fol_not, fol_forall, fol_exists.
	    // 2 subformulas in case of fol_implies, fol_equiv.
	    // Any number of subformulas distinct from 0 
	    // in case of fol_and, fol_or.


	 formula_node( fol_oper op,
		       const std::list< function > & variables,
		       const std::list< predicate > & pred, 
		       const std::list< term > & subterms,
		       const std::list< formula > & subformulas ) 
            : op( op ),
              variables( variables ),
	      pred( pred ), 
	      subterms( subterms ),
	      subformulas( subformulas )
         { }

      };


      formula_node* repr;

   private:
      formula( fol_oper op,
               const std::list< function > & variables,
	       const std::list< predicate > & pred,
	       const std::list< term > & subterms,
	       const std::list< formula > & subformulas )
         : repr( new formula_node( op, variables, pred,
				       subterms, subformulas ))
      {
         ( repr -> reference_count ) = 1;
      }


   public:
      formula( const formula& other );
      void operator = ( formula other );
      ~formula( ); 


   public: 
      static formula make_false( );
      static formula make_true( );
      static formula make_not( const formula& f );
      static formula make_and( const std::list< formula > & subforms );
      static formula make_or( const std::list< formula > & subforms );

      static formula make_implies( const formula& f1, const formula& f2 );
      static formula make_equiv( const formula& f1, const formula& f2 );

      static formula make_forall( const std::list< function > & variables, 
		                  const formula& f );
      static formula make_exists( const std::list< function > & variables, 
		                  const formula& f );

      static formula make_equals( const term& t1, const term& t2 );
      static formula make_atom( const predicate& p, 
		                const std::list< term > & subterms );


      fol_oper getoperator( ) const;
      
      std::list< function > :: const_iterator variables_begin( ) const; 
      std::list< function > :: const_iterator variables_end( ) const;
         // The quantified variables in case of fol_exists, fol_forall.

      std::list< term > :: const_iterator subterms_begin( ) const;
      std::list< term > :: const_iterator subterms_end( ) const;
         // The subterms in case of fol_equals, or fol_atom.

      std::list< formula > :: const_iterator subformulas_begin( ) const;
      std::list< formula > :: const_iterator subformulas_end( ) const;
         // The subformulas in case of fol_false, fol_true,
	 // fol_not, fol_and, fol_or, fol_implies, fol_equiv,
	 // fol_forall, fol_exists. 

      predicate getpredicate( ) const;
         // Gets the predicate in case of a non-equality atom. 


      friend std::ostream& operator << ( std::ostream& , const formula& f ); 

   private:
      static bool zero_arity_only( const std::list< function > & ); 
   };


}


#endif



