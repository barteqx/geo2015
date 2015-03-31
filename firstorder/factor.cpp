

// Code written by Hans de Nivelle,
// July 2006.


#include "factor.h"
#include "replacement.h"
#include <cstdlib>


namespace firstorder
{


   unsigned int factor::maxfactorsize = 4; 
      // We must see what is a reasonable value. 
      // It should not be less than 1, because there
      // is always at least one factor. 


   namespace
   {
	     
      // addfactors adds to factorlist the disjunctions that
      // are obtained by factoring ( * p1 .. * p2 ).
      // To each disjunction in the result, initial_disj 
      // is appended in the front. 

      void addfactors( std::list< std::list< formula > > :: const_iterator p1,
		       std::list< std::list< formula > > :: const_iterator p2, 
		       std::list< formula > & initial_disj, 
                       std::list< formula > & factorlist )
      {
         if( p1 == p2 ) 
            factorlist. push_back( formula::make_or( initial_disj ));
         else
         {
            std::list< std::list< formula > > :: const_iterator p1_succ = p1;
	    p1_succ ++ ;

            for( std::list< formula > :: const_iterator
                    q = p1 -> begin( );
		    q != p1 -> end( );
		    ++ q ) 
            {
               initial_disj. push_back( *q );
	       addfactors( p1_succ, p2, initial_disj, factorlist );
	       initial_disj. pop_back( );
            }
         }
      }



      // disj represents a disjunction of conjunctions. We
      // compute the number of disjunctions that factoring would
      // result in. If this number is unacceptibly high, we replace
      // a biggest conjunction by a fresh atom.
      // We keep on doing this, until
      // | Pi(disj) | <= maxfactorsize. 


      void replace_conj( bindingstack& bs,
                         std::list< std::list< formula > > & disj,
                         sequent& seq,
		         std::list< formula > & definitions )
      {

         ASSERT( disj. size( ) != 0 );
         ASSERT( factor::maxfactorsize > 0 );

	 while( true )
         {
            bool replacementneeded = false;

            // We decide whether subformula replacement is necessary:
            // We compare the size of Pi(disj) with maxfactorsize. 

            unsigned int prod = 1;
            for( std::list< std::list< formula > > :: const_iterator
                    p = disj. begin( );
	            p != disj. end( ) && ! replacementneeded;
	            ++ p )
            {
               prod *= ( p -> size( ));
	       if( prod > factor::maxfactorsize ) 
                  replacementneeded = true;
	             // Waiting until the end would be dangerous
		     // because of possible overflow of prod. 
            }

	    if( ! replacementneeded )
               return;

            // *m is going to be a biggest conjunction in disj.
	    // We will replace *m by a fresh atom. 

            std::list< std::list< formula > > :: iterator m = disj. begin( ); 
            for( std::list< std::list< formula > > :: iterator
                    p = disj. begin( );
		    p != disj. end( );
		    ++ p )
            {
               if( p -> size( ) > m -> size( ))
                  m = p;
            }

	    // We collect the free variables of *m: 

	    std::map< function, bool > varmap =
		    replacement::freevariables( bs, *m );
	    std::list< function > varlist =
		    replacement::converttolist( varmap ); 
 
	    // Produce a nice virgin atom:

	    formula at = replacement::make_atom( varlist, seq );

	    // Produce the definitions:

	    for( std::list< formula > :: const_iterator
                    p = m -> begin( );
		    p != m -> end( );
		    ++ p )
            {
               definitions. push_front(
                       replacement::positive_definition( at, *p ));
            }

	    // Replace *m by the single atom at.

	    m -> clear( );
	    m -> push_back(at);

         }
      }

   } 




   // The result has to be factored (i.e. not contain conjunction).
   // If it contains an existential quantifier, the body is an atom.

   formula factor::replace_exists( 
		       bindingstack& bs,
		       std::list< function > :: const_iterator p1,
		       std::list< function > :: const_iterator p2,
		       formula body,
		       sequent& seq, 
		       std::list< formula > & definitions )
   {
      if( p1 == p2 )
      {
         std::list< formula > body_conj = 
		        distr( bs, body, seq, definitions );

         if( body_conj. size( ) == 1 )
            return body_conj. front( );

         std::map< function, bool > varmap =
                         replacement::freevariables( bs, body_conj );

	 std::list< function > varlist =
		         replacement::converttolist( varmap );

	 formula at = replacement::make_atom( varlist, seq );

	 for( std::list< formula > :: const_iterator
                 p = body_conj. begin( );
		 p != body_conj. end( );
		 ++ p )
         {
            definitions. push_front( 
		       replacement::positive_definition( at, *p ));
         }

	 return at; 
          
      }
      else
      {
         function v = * ( p1 ++ );

	 bs. bind( v );

	 formula bodyres = replace_exists( bs, p1, p2, body, seq, 
			                   definitions );

	 // If the body is not an atom, then we replace it 
	 // by a fresh atom. 

	 if(  bodyres. getoperator( ) != fol_atom ) 
         {
            std::map< function, bool > varmap = 
		       replacement::freevariables( bs, bodyres ); 
     
	    for( std::list< function > :: const_iterator
                    q = p1; q != p2; ++ q )
            {
               varmap [ *q ] = false;
	          // Because they are existentially quantified,
	          // they are not free in the result. 
	    }

            varmap [v] = false;
	       // We remove v, 
	    std::list< function > varlist = 
		          replacement::converttolist( varmap ); 
	    varlist. push_back(v);
               // and put it back last, because we want the 
	       // existential quantifier to be last. 
	       // That looks better. 

	    formula at = replacement::make_atom( varlist, seq );
	    definitions. push_front( 
                    replacement::positive_definition( at, bodyres ));

	    bodyres = at; 
         }

	 std::list< function > vvvv;
	 vvvv. push_back(v);

	 bs. release(v); 
	 return formula::make_exists( vvvv, bodyres );
      }
   }




   // The result of distr is a list of formulas which should be interpreted
   // as a conjunction.
   // The bindingstack records the surrounding bound variables of f.
   // The sequent is needed for the possible generation of fresh names.
   // In case subformulas are replaced, the definitions
   // are inserted into definitions.

   std::list< formula > factor::distr( bindingstack& bs, formula f,
				       sequent& seq, 
				       std::list< formula > & definitions ) 
   {
      switch( f. getoperator( ))
      {
      case fol_false:
      case fol_atom:
      case fol_equals:
         {
            std::list< formula > res;
	       
            res. push_back(f);
	    return res;
         }

      case fol_true:
	 {
            // For TRUE, we return the empty list.

            std::list< formula > res;
	    return res;
         }

      case fol_not:
         {
            formula sub = * f. subformulas_begin( );

	    if( sub. getoperator( ) != fol_atom && 
                sub. getoperator( ) != fol_equals )
            {
               std::cout << "formula is not in NNF\n";
	       ASSERT( false ); exit(0);
            }
            
	    std::list< formula > result;
	    result. push_back(f);
	    return result;
         }

      case fol_and:
         {
            std::list< formula > res;

	    // We compute the distributions of the subformulas,
	    // and append them to res. 

            for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               std::list< formula > sub = distr( bs, *p, seq, definitions );

               for( std::list< formula > :: const_iterator
                       q = sub. begin( );
		       q != sub. end( );
		       ++ q )
               {
                  res. push_back(*q);
               }
            }
               
	    return res;
         }

      case fol_or:
	 {
            std::list< std::list< formula > > subconjunctions;

	    for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               subconjunctions. push_back( distr( bs, *p, seq, definitions ));
            }

	    if( subconjunctions. size( ) == 0 )
            {
               std::list< formula > result; 
               result. push_back( formula::make_false( ) );
	       return result;
            } 

	    replace_conj( bs, subconjunctions, seq, definitions );
	       // If necessary, this function replaces subconjunctions
	       // by fresh atoms. 

	    std::list< formula > disj; 

	    std::list< formula > result; 
	    addfactors( subconjunctions. begin( ), 
			subconjunctions. end( ), disj, result );
	    ASSERT( disj. size( ) == 0 ); 

	    return result; 
         }

      case fol_forall:
         {
            std::list< function > varlist;
	    for( std::list< function > :: const_iterator
                    v = f. variables_begin( );
		    v != f. variables_end( );
		    ++ v )
            {
               varlist. push_back( *v );
	       bs. bind( *v ); 
            }

	    std::list< formula > bodyresult = 
		   distr( bs, * f. subformulas_begin( ), seq, definitions );

	    for( std::list< function > :: const_iterator
                    v = f. variables_begin( );
		    v != f. variables_end( );
		    ++ v )
            {
               bs. release( *v );
            }

	    std::list< formula > result;
	    for( std::list< formula > :: const_iterator
                    p = bodyresult. begin( );
		    p != bodyresult. end( );
		    ++ p )
            {
               result. push_back( formula::make_forall( varlist, *p ));
            }

	    return result;
         }

      case fol_exists:
         {
            if( f. variables_begin( ) == f. variables_end( ))
               return distr( bs, *f. subformulas_begin( ), seq, definitions );
	    else
            {
               std::list< formula > result; 
	       result. push_back( 
		    replace_exists( bs, 
			     f. variables_begin( ), f. variables_end( ), 
			     * f. subformulas_begin( ), 
			     seq, definitions )); 
	       return result;
            }
         }

      default:
	 std::cout << "formula is not in NNF\n";
	 ASSERT( false ); exit(0); 
      }
   }



   void factor::convert_to_cuden( sequent& seq )
   {
      bindingstack bs;

      std::list< formula > result;

      for( std::list< formula > :: iterator
              p = seq. formulas. begin( );
	      p != seq. formulas. end( );
	      ++ p )
      {
	 std::list< formula > definitions;

	 std::list< formula > fact = distr( bs, *p, seq, definitions );
	 ASSERT( bs. isempty( ) );

	 while( fact. size( ))
         {
            result. push_back( fact. front( ));
	    fact. pop_front( );
         }

	 while( definitions. size( ))
         {
            result. push_back( definitions. front( ));
	    definitions. pop_front( );
	 }
      }

      seq. formulas = result;
   }


} 


