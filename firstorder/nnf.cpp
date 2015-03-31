

// Code written by Hans de Nivelle, July 2006.


#include "nnf.h"
#include "replacement.h"
#include <cstdlib>


namespace firstorder
{


   formula nnf::make_nnf( int polarity, formula f )
   {
      ASSERT( polarity == -1 || polarity == 1 );

      switch( f. getoperator( ))
      {
      case fol_false:
         if( polarity == -1 )
	    return formula::make_true( );
	 else
            return formula::make_false( );

      case fol_true:
	 if( polarity == -1 )
            return formula::make_false( );
	 else
            return formula::make_true( );

      case fol_not:
         return make_nnf( - polarity, * f. subformulas_begin( ));

      case fol_and:
      case fol_or:
         {
            std::list< formula > sub;
	    for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               sub. push_back( make_nnf( polarity, *p ));
            }
	    if( polarity == -1 )
            {
               if( f. getoperator( ) == fol_and )
                  return formula::make_or( sub );
	       else
                  return formula::make_and( sub );
            }
	    else
            {
               if( f. getoperator( ) == fol_and )
                  return formula::make_and( sub );
	       else
                  return formula::make_or( sub );
            }
         }

      case fol_implies:
         {
            std::list< formula > :: const_iterator p = f. subformulas_begin( );

	    std::list< formula > sub; 
	    sub. push_back( make_nnf( - polarity, * ( p ++ )));
	    sub. push_back( make_nnf( polarity, * ( p ++ )));

            if( polarity == -1 )
               return formula::make_and( sub );
            else
               return formula::make_or( sub );
         }
      case fol_equiv:
         {
            std::list< formula > :: const_iterator p = f. subformulas_begin( );

	    formula sub1pos = make_nnf( 1, *p );
	    formula sub1neg = make_nnf( -1, *p );

	    ++ p;

	    formula sub2pos = make_nnf( 1, *p );
	    formula sub2neg = make_nnf( -1, *p ); 

	    std::list< formula > subsub1;
	    std::list< formula > subsub2;
	    std::list< formula > sub; 

	    if( polarity == -1 )
            {
               subsub1. push_back( sub1pos );
	       subsub1. push_back( sub2neg );
	
	       sub. push_back( formula::make_and( subsub1 ));
		 
	       subsub2. push_back( sub1neg );
	       subsub2. push_back( sub2pos );

	       sub. push_back( formula::make_and( subsub2 ));

	       return formula::make_or( sub );
            }
	    else
            {
               subsub1. push_back( sub1neg );
	       subsub1. push_back( sub2pos );

	       sub. push_back( formula::make_or( subsub1 ));

	       subsub2. push_back( sub1pos );
	       subsub2. push_back( sub2neg );

	       sub. push_back( formula::make_or( subsub2 ));
		 
               return formula::make_and( sub );
            }
         }
      case fol_forall:
      case fol_exists:
	 {
            formula body = make_nnf( polarity, * f. subformulas_begin( ));

	    std::list< function > vars;
	    for( std::list< function > :: const_iterator
                    p = f. variables_begin( );
		    p != f. variables_end( );
		    ++ p )
            {
               vars. push_back( *p );
            }
	    if( polarity == -1 )
            {
               if( f. getoperator( ) == fol_forall )
                  return formula::make_exists( vars, body );
	       else
                  return formula::make_forall( vars, body );
            }
	    else
            {
               if( f. getoperator( ) == fol_forall )
                  return formula::make_forall( vars, body );
	       else
                  return formula::make_exists( vars, body );
            }
         }

      case fol_equals:
      case fol_atom:
         if( polarity == -1 )
            return formula::make_not(f);
	 else
            return f;
      }
      ASSERT( false ); exit(0);
   }
      



   unsigned int nnf::max_equivalences = 0;
  

   namespace 
   {
   
      bool istrivial( formula f )
      {
         while( f. getoperator( ) == fol_not )
            f = *f. subformulas_begin( );

	 if( f. getoperator( ) == fol_false ||
             f. getoperator( ) == fol_true ||
	     f. getoperator( ) == fol_atom ||
	     f. getoperator( ) == fol_equals )
         {
            return true;
         }
	 else
            return false;
      }
   }  
   
   
   formula nnf::replace_equiv( bindingstack& bs, 
		               formula f,
			       unsigned int nr_equivalences, 
			       sequent& seq,
			       std::list< formula > & definitions )
   {
      switch( f. getoperator( ))
      {
      case fol_true:
      case fol_false:
      case fol_equals:
      case fol_atom: 
         return f;

      case fol_not:
      case fol_and:
      case fol_or:
      case fol_implies:
         {
            std::list< formula > sub;
            for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               sub. push_back( 
                  replace_equiv( bs, *p, nr_equivalences, seq, definitions ));
            }
            
	    if( f. getoperator( ) == fol_not )
               return formula::make_not( sub. front( ));

	    if( f. getoperator( ) == fol_and )
               return formula::make_and( sub );

	    if( f. getoperator( ) == fol_or )
               return formula::make_or( sub );

	    if( f. getoperator( ) == fol_implies )
            {
               formula sub1 = sub. front( );
	       sub. pop_front( );
	       formula sub2 = sub. front( );

	       return formula::make_implies( sub1, sub2 );
            }
         }
      case fol_equiv:
         {
            unsigned int nr_equivalences1 = nr_equivalences + 1;
	    if( nr_equivalences >= max_equivalences )
               nr_equivalences1 = 0;
                  // We will replace the subformulas, so the
		  // counter resets.

            std::list< formula > sub;
	    for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               sub. push_back(
                 replace_equiv( bs, *p, nr_equivalences1, seq, definitions ));
            }

            if( nr_equivalences >= max_equivalences )
            {

               for( std::list< formula > :: iterator
                       p = sub. begin( ); p != sub. end( ); ++ p )
               {
                  // If *p is trivial, it is not replaced.

                  if( ! istrivial( *p )) 
                  {
                     // It's replacement time!

                     std::map< function, bool > varmap =
                                 replacement::freevariables( bs, *p );
	             std::list< function > varlist = 
		                 replacement::converttolist( varmap );

	             formula at = replacement::make_atom( varlist, seq );
	             definitions. push_front( 
				  replacement::full_definition( at, *p ));
		     *p = at;
                  }
               }
            }

	    formula sub1 = sub. front( );
	    sub. pop_front( );
	    formula sub2 = sub. front( );
	    return formula::make_equiv( sub1, sub2 ); 
         }

      case fol_forall:
      case fol_exists:
         {
            std::list< function > varlist;
	    for( std::list< function > :: const_iterator
                    v = f. variables_begin( );
		    v != f. variables_end( );
		    ++ v )
            {
               varlist. push_back(*v);
	       bs. bind(*v);
            }

	    formula bodyresult =
		       replace_equiv( bs, * f. subformulas_begin( ), 
				      nr_equivalences, seq, definitions );

	    for( std::list< function > :: const_iterator
                    v = f. variables_begin( );
		    v != f. variables_end( );
		    ++ v )
            {
               bs. release(*v);  
            }

	    if( f. getoperator( ) == fol_forall )
               return formula::make_forall( varlist, bodyresult );
	    else
               return formula::make_exists( varlist, bodyresult );
         }
      default:
         ASSERT( false ); exit(0);
      }
   }




   void nnf::convert_to_nnf( sequent& seq )
   {
      bindingstack bs;

      std::list< formula > result;

      for( std::list< formula > :: iterator
              p = seq. formulas. begin( );
	      p != seq. formulas. end( );
	      ++ p )
      {
	 std::list< formula > definitions;

         formula repl = replace_equiv( bs, *p, 0, seq, definitions );
         ASSERT( bs. isempty( ) );

	 repl = make_nnf( 1, repl );
         result. push_back( repl );

	 while( definitions. size( ))
         {
            formula f = definitions. front( );
            definitions. pop_front( );

	    f = make_nnf( 1, f ); 
	    result. push_back(f); 
         }
      }

      seq. formulas = result;
   }


} 


