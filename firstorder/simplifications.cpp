

// Code written by Hans de Nivelle,
// July 2006.


#include "simplifications.h"
#include <cstdlib>


namespace firstorder
{

   formula simplifications::flatten( formula f )
   {
      
      switch( f. getoperator( ))
      {
      case fol_false:
      case fol_true:
      case fol_equals:
      case fol_atom:
         return f;

      case fol_and:
      case fol_or:
      case fol_equiv: 
         {
            std::list< std::list< formula > :: const_iterator > p1;
	    std::list< std::list< formula > :: const_iterator > p2;

            p1. push_front( f. subformulas_begin( ));
	    p2. push_front( f. subformulas_end( ));

	    std::list< formula > resultsubforms;

	    while( p1. size( ))
            {
               if( p1. front( ) == p2. front( ))
               {
                  p1. pop_front( );
	          p2. pop_front( );
               }
	       else
               {
                  formula g = * p1. front( );
		  ( p1. front( ) ) ++ ;

		  if( g. getoperator( ) == f. getoperator( ))
                  {
                     p1. push_front( g. subformulas_begin( ));
		     p2. push_front( g. subformulas_end( ));
                  }
		  else
	          {
                     g = flatten(g); 
                     resultsubforms. push_back(g);
                  }
               }
            }

	    if( f. getoperator( ) == fol_and )
               return formula::make_and( resultsubforms );
	   
	    if( f. getoperator( ) == fol_or ) 
               return formula::make_or( resultsubforms );

	    if( f. getoperator( ) == fol_equiv )
            {
               ASSERT( resultsubforms. size( ) >= 2 );

	       while( resultsubforms. size( ) >= 2 )
               {
                  formula g1 = resultsubforms. front( ); 
	          resultsubforms. pop_front( ); 
                  formula g2 = resultsubforms. front( );
		  resultsubforms. pop_front( );

		  resultsubforms. push_front( formula::make_equiv( g1, g2 ));
               }

	       return resultsubforms. front( );
            }
         }

      case fol_implies:
         {
	    std::list< formula > :: const_iterator p = f. subformulas_begin( );
            formula sub1 = flatten( * ( p ++ ) );
	    formula sub2 = flatten( *p );

	    return formula::make_implies( sub1, sub2 );
         }

      case fol_not:
         {
            formula sub = * f. subformulas_begin( );
	    sub = flatten( sub );
	    return formula::make_not( sub );
         }

      case fol_exists:
      case fol_forall:
	 {
            std::list< function > vars;

	    formula g = f; 

	    // The following loop is passed at least once.

	    while( g. getoperator( ) == f. getoperator( ))
            {
               for( std::list< function > :: const_iterator
                       p = g. variables_begin( );
		       p != g. variables_end( );
		       ++ p )
               {
                  vars. push_back( *p );
               }

	       g = * g. subformulas_begin( );
            }

	    g = flatten(g); 

	    if( vars. size( ) == 0 )
               return g;

	    if( f. getoperator( ) == fol_forall )
               return formula::make_forall( vars, g );
            else
               return formula::make_exists( vars, g );
         }

      default:
         ASSERT( false ); exit(0);
      }
   }





   formula simplifications::truth_constants( formula f )
   {
      switch( f. getoperator( ))
      {
           
      case fol_false:
      case fol_true:
      case fol_equals:
      case fol_atom:
         return f;

      case fol_and:
         {
            std::list< formula > sub;

            for( std::list< formula > :: const_iterator
		    p = f. subformulas_begin( );
	            p != f. subformulas_end( ); 
		    ++ p )
            {
               formula g = truth_constants( *p );
	       if( g. getoperator( ) == fol_false )
                  return g;
	       if( g. getoperator( ) != fol_true )
                  sub. push_back( g );
            }
            if( sub. size( ) == 0 )
               return formula::make_true( );
	    if( sub. size( ) == 1 )
               return sub. front( );

	    return formula::make_and( sub );
         }

      case fol_or:
         {
            std::list< formula > sub;

            for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               formula g = truth_constants( *p );
	       if( g. getoperator( ) == fol_true )
                  return g;

	       if( g. getoperator( ) != fol_false )
                  sub. push_back( g );
            }

	    if( sub. size( ) == 0 )
               return formula::make_false( );
	    if( sub. size( ) == 1 ) 
               return sub. front( );

            return formula::make_or( sub );
         }

      case fol_implies:
      case fol_equiv: 
         {
            std::list< formula > :: const_iterator p = f. subformulas_begin( );

	    formula sub1 = truth_constants( * ( p ++ ));
	    formula sub2 = truth_constants( *p );

            if( f. getoperator( ) == fol_implies )
            {
               if( sub2. getoperator( ) == fol_true )
                  return sub2;
	       if( sub1. getoperator( ) == fol_true )
                  return sub2;
	       if( sub1. getoperator( ) == fol_false )
                  return formula::make_true( );
                  
	       return formula::make_implies( sub1, sub2 );
            }
            else
            {
               if( sub1. getoperator( ) == fol_true )
                  return sub2;

               if( sub2. getoperator( ) == fol_true )
                  return sub1;

	       if( sub1. getoperator( ) == fol_false && 
                   sub2. getoperator( ) == fol_false ) 
               {
                  return formula::make_true( );
               }

	       if( sub1. getoperator( ) == fol_false )
                  return formula::make_not( sub2 );

	       if( sub2. getoperator( ) == fol_false )
                  return formula::make_not( sub1 );

	       return formula::make_equiv( sub1, sub2 );
            }
         }

      case fol_not: 
         {
            formula sub = truth_constants( * f. subformulas_begin( ) ); 

	    if( sub. getoperator( ) == fol_false )
               return formula::make_true( );
	    if( sub. getoperator( ) == fol_true )
               return formula::make_false( );

	    return formula::make_not( sub );
         }

      case fol_forall:
      case fol_exists:
         {
            std::list< function > vars;
	    for( std::list< function > :: const_iterator 
                    p = f. variables_begin( );
		    p != f. variables_end( );
		    ++ p )
            {
               vars. push_back( *p );
            }

	    formula sub = truth_constants( * f. subformulas_begin( ) ); 

            // We do not propagate truth-constants through quantifiers,
	    // because that is non-constructive.

	    if( f. getoperator( ) == fol_forall )
               return formula::make_forall( vars, sub );
	    else
               return formula::make_exists( vars, sub );
         }
      default:
	 ASSERT( false ); exit(0);
      }
   }

   

   bool simplifications::iscontained( function v, term t )
   {
      if( v == t. getfunction( ))
         return true;

      for( std::list< term > :: const_iterator 
              p = t. subterms_begin( );
	      p != t. subterms_end( );
	      ++ p )
      {
         if( iscontained( v, *p ))
            return true;
      }
      return false;
   }

        
   

   bool simplifications::isfree( function v, formula f )
   {
      switch( f. getoperator( ))
      {
      case fol_false:
      case fol_true:
      case fol_not:
      case fol_or:
      case fol_and:
      case fol_implies:
      case fol_equiv: 
         for( std::list< formula > :: const_iterator
                 p = f. subformulas_begin( );
		 p != f. subformulas_end( );
		 ++ p )
         {
            if( isfree( v, *p ))
               return true;
         }
	 return false;

      case fol_forall:
      case fol_exists:
	 {
            for( std::list< function > :: const_iterator
                    p = f. variables_begin( );
		    p != f. variables_end( );
		    ++ p )
            {
               if( *p == v )
                  return false;
            }

	    formula body = * f. subformulas_begin( );
	    return isfree( v, body );
         }

      case fol_atom:
      case fol_equals:
         {
            for( std::list< term > :: const_iterator
                    p = f. subterms_begin( );
		    p != f. subterms_end( );
		    ++ p )
            {
               if( iscontained( v, *p ))
                  return true;
            }
	    return false; 
         }
      default:
	 ASSERT( false ); exit(0);
      }
   }


   formula simplifications::absorb( formula f )
   {
      ASSERT( f. getoperator( ) == fol_forall || 
              f. getoperator( ) == fol_exists );
     
      std::list< function > vars;
      for( std::list< function > :: const_iterator
              p = f. variables_begin( );
	      p != f. variables_end( );
	      ++ p )
      {
         vars. push_back(*p);
      }

      formula body = * f. subformulas_begin( ); 

   restart: 
      switch( body. getoperator( ))
      {
      case fol_not:
         {
            formula body2 = * body. subformulas_begin( );

	    switch( body2. getoperator( ))
            {
            case fol_or:
            case fol_and:
               {
                  std::list< formula > res;
		  for( std::list< formula > :: const_iterator
                          p = body2. subformulas_begin( );
			  p != body2. subformulas_end( );
			  ++ p )
                  {
                     res. push_back( formula::make_not( *p ));
                  }
                  if( body2. getoperator( ) == fol_or )
                     body = formula::make_and( res );
		  else
                     body = formula::make_or( res );
		  goto restart;
               }
            case fol_implies:
	       {
                  std::list< formula > res;
		  std::list< formula > :: const_iterator 
			      p = body2. subformulas_begin( );
		  res. push_back( *p );
		  ++ p;
		  res. push_back( formula::make_not( *p ));
		  body = formula::make_and( res );
		  goto restart;
               }
            case fol_not:
	       {
                  body = * body2. subformulas_begin( );
		  goto restart;
               }
            case fol_forall:
            case fol_exists:
               {
                  std::list< function > vars2;
		  for( std::list< function > :: const_iterator
                          p = body2. variables_begin( );
			  p != body2. variables_end( );
			  ++ p )
                  {
                     vars2. push_back( *p );
                  }

		  if( body2. getoperator( ) == fol_forall )
                     body = formula::make_exists( vars2, 
			 formula::make_not( * body2. subformulas_begin( )));
		  else
                     body = formula::make_forall( vars2,
                         formula::make_not( * body2. subformulas_begin( )));

	          goto restart;
               }
	    }
         }
	 break; 
      case fol_implies:
	 {
            std::list< formula > res;
	    std::list< formula > :: const_iterator 
                            p = body. subformulas_begin( );
            res. push_back( formula::make_not( *p ));
	    ++ p;
	    res. push_back( *p );
	    body = formula::make_or( res );
	    goto restart;
         }
      case fol_forall:
      case fol_exists:
	 {
            if( f. getoperator( ) == body. getoperator( ) )
            {
               for( std::list< function > :: const_iterator
                       p = body. variables_begin( );
		       p != body. variables_end( );
		       ++ p )
               {
                  vars. push_back( *p );
               }
	       body = * body. subformulas_begin( );

	       goto restart;
            }
         }
      }

      // We build the result and quit.

      if( f. getoperator( ) == fol_forall )
         return formula::make_forall( vars, body );
      else
         return formula::make_exists( vars, body );
   }



   formula simplifications::remove_unused( formula f )
   {
      ASSERT( f. getoperator( ) == fol_forall ||
              f. getoperator( ) == fol_exists );

      std::list< function > vars;
      for( std::list< function > :: const_iterator
              p = f. variables_begin( );
	      p != f. variables_end( );
	      ++ p )
      {
         vars. push_back(*p);
      }

      formula body = * f. subformulas_begin( );


      // Only non-repeated variables that are free in the body
      // are copied to vars2: 

      std::list< function > vars2;
      for( std::list< function > :: const_iterator
              p = vars. begin( ); p != vars. end( ); ++ p )
      {
         for( std::list< function > :: const_iterator 
		 q = vars. begin( ); q != p; ++ q )
         {
            if( *p == *q )
                goto skip_p;
         }

	 if( ! isfree( *p, body ))
            goto skip_p;

	 vars2. push_back(*p);
      skip_p:
	 ;
      }

      // If no variables are left, we simply return the body.

      if( vars2. size( ))
      {
         if( f. getoperator( ) == fol_forall )
            return formula::make_forall( vars2, body );
	 else
            return formula::make_exists( vars2, body );
      }
      else
         return body;
   }

 
   namespace
   {
      formula build( fol_oper quant, 
                     const std::list< function > & vars,
		     fol_oper mainop,
		     const std::list< formula > & body )
      {
         ASSERT( quant == fol_forall || quant == fol_exists );
	 ASSERT( mainop == fol_and || mainop == fol_or );

	 if( body. size( ) == 0 )
         {
            if( mainop == fol_and )
               return formula::make_true( );
	    else
               return formula::make_false( );
         }

	 std::list< formula > res;

	 if( body. size( ) == 1 )
            res. push_back( body. front( ));
	 else
         {
	    if( mainop == fol_and )
               res. push_back( formula::make_and( body ));
            else
	       res. push_back( formula::make_or( body ));
         }

         if( vars. size( ) == 0 )
            return res. front( );

         if( quant == fol_forall )
            return formula::make_forall( vars, res. front( )); 
         else
            return formula::make_exists( vars, res. front( ));
      }
   }



   formula simplifications::distrusedunused( formula f )
   {
      ASSERT( f. getoperator( ) == fol_forall ||
              f. getoperator( ) == fol_exists );

      std::list< function > vars;
      for( std::list< function > :: const_iterator
              p = f. variables_begin( );
	      p != f. variables_end( );
	      ++ p )
      {
         vars. push_back( *p );
      }

      formula body = * f. subformulas_begin( );
      if( body. getoperator( ) != fol_and &&
          body. getoperator( ) != fol_or )
      {
         return f;  
      }

      std::list< formula > body_args; 
      for( std::list< formula > :: const_iterator
              p = body. subformulas_begin( );
	      p != body. subformulas_end( );
	      ++ p )
      {
         body_args. push_back( *p );
      }

      while( vars. size( ))
      {
         unsigned int leastuse = body_args. size( ) + 1; 
	 std::list< function > :: iterator leastused = vars. begin( );

	 for( std::list< function > :: iterator
                 p = vars. begin( ); p != vars. end( ); ++ p )
         {
            unsigned int use = 0;
            for( std::list< formula > :: const_iterator
                    q = body_args. begin( );
		    q != body_args. end( );
		    ++ q )
            {
               if( isfree( *p, *q ))
                  ++ use; 
            }
	    if( use < leastuse )
            {
               leastuse = use;
	       leastused = p;
            }
         }

	 // If all variables are used everywhere, we quit:

	 if( leastuse == body_args. size( ))
            return build( f. getoperator( ), vars,
                          body. getoperator( ), body_args ); 

	 // Split body_args into the arguments that contain leastused,
	 // and those that do not contain leastused. 

	 std::list< formula > body_containing_args;
	 std::list< formula > body_notcontaining_args; 

         while( body_args. size( ))
         {
            if( isfree( *leastused, body_args. front( )))
               body_containing_args. push_back( body_args. front( ));
	    else
               body_notcontaining_args. push_back( body_args. front( ));
            body_args. pop_front( ); 
         }

	 // Also split the variables:

	 std::list< function > vvvv; 
	 vvvv. push_back( *leastused );
	 vars. erase( leastused );

	 body_args = body_notcontaining_args; 
         body_args. push_back( 
			 build( f. getoperator( ), vvvv,
			        body. getoperator( ), body_containing_args ));

      }
      return build( f. getoperator( ), vars,
	            body. getoperator( ), body_args );
   }



   formula simplifications::distrquantifier( formula f )
   {
      ASSERT( f. getoperator( ) == fol_forall ||
              f. getoperator( ) == fol_exists );

      std::list< function > vars;
      for( std::list< function > :: const_iterator
               p = f. variables_begin( );
	       p != f. variables_end( );
	       ++ p )
      {
         vars. push_back( *p );
      }

      formula body = * f. subformulas_begin( );
      if( ( f. getoperator( ) == fol_forall &&
            body. getoperator( ) == fol_and ) ||
          ( f. getoperator( ) == fol_exists && 
	    body. getoperator( ) == fol_or )) 
      {
         std::list< formula > body_args;
	 for( std::list< formula > :: const_iterator
		 p = body. subformulas_begin( );
		 p != body. subformulas_end( );
		 ++ p )
         {
            if( f. getoperator( ) == fol_forall ) 
               body_args. push_back( formula::make_forall( vars, *p ));
	    else
               body_args. push_back( formula::make_exists( vars, *p ));
         }

	 if( body. getoperator( ) == fol_and )
            return formula::make_and( body_args ); 
         else
            return formula::make_or( body_args );
      }

      return f;
   }



   namespace 
   {

      formula toplevel_antiprenex( formula f )
      {
         ASSERT( f. getoperator( ) == fol_forall ||
                 f. getoperator( ) == fol_exists ); 
        
         f = simplifications::absorb(f);

	 if( f. getoperator( ) != fol_forall &&
             f. getoperator( ) != fol_exists )
         {
            return simplifications::antiprenex(f);
         }

	 f = simplifications::remove_unused(f);

	 if( f. getoperator( ) != fol_forall &&
             f. getoperator( ) != fol_exists )
         {
            return simplifications::antiprenex(f);
         }

	 formula sub = * f. subformulas_begin( );
         if( sub. getoperator( ) == fol_forall ||
             sub. getoperator( ) == fol_exists )
         {
            sub = toplevel_antiprenex( sub );

	    std::list< function > vars; 
	    for( std::list< function > :: const_iterator
                    p = f. variables_begin( );
		    p != f. variables_end( );
		    ++ p )
            {
               vars. push_back( *p );
            }

	    if( f. getoperator( ) == fol_forall )
               f = formula::make_forall( vars, sub );
	    else
               f = formula::make_exists( vars, sub );
         }

	 // I don't know if this step is any good at all.

         if( f. getoperator( ) != fol_forall &&
             f. getoperator( ) != fol_exists )
         {
            return simplifications::antiprenex(f);
         }

         f = simplifications::distrusedunused(f);
	 if( f. getoperator( ) != fol_forall &&
             f. getoperator( ) != fol_exists )
         {
            return simplifications::antiprenex(f);
         }

	 return f; 
      }
   } 



   formula simplifications::antiprenex( formula f )
   {
      switch( f. getoperator( ))
      {
      case fol_false:
      case fol_true:
      case fol_atom:
      case fol_equals:
         return f; 

      case fol_not:
         return formula::make_not( antiprenex( *f. subformulas_begin( ))); 

      case fol_implies:
      case fol_equiv:
         {
            std::list< formula > :: const_iterator 
               p = f. subformulas_begin( );

	    formula sub1 = antiprenex( *( p ++ ) );
	    formula sub2 = antiprenex( *p );
	    
	    if( f. getoperator( ) == fol_implies )
               return formula::make_implies( sub1, sub2 );
	    else
               return formula::make_equiv( sub1, sub2 );
         }

      case fol_and:
      case fol_or:
         {
            std::list< formula > sub;

            for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               sub. push_back( antiprenex( *p ));
            }
            
	    if( f. getoperator( ) == fol_and )
               return formula::make_and( sub );
	    else
               return formula::make_or( sub );
         }

      case fol_forall:
      case fol_exists:
         {
            f = toplevel_antiprenex(f); 

            if( f. getoperator( ) != fol_forall && 
                f. getoperator( ) != fol_exists )
            {
               return flatten(f);
            }

	    std::list< function > vars;
	    for( std::list< function > :: const_iterator
                    p = f. variables_begin( );
		    p != f. variables_end( );
		    ++ p )
            {
               vars. push_back(*p);
            }

	    formula body = * f. subformulas_begin( );
            body = antiprenex( body );
	    if( f. getoperator( ) == fol_forall )
               return formula::make_forall( vars, body );
	    else
               return formula::make_exists( vars, body );
         }
      default:
	 ASSERT( false ); exit(0);

      }
   }


   void simplifications::flatten( sequent& seq )
   {
      for( std::list< formula > :: iterator
              p = seq. formulas. begin( );
	      p != seq. formulas. end( );
	      ++ p )
      {
         *p = flatten( *p );
      }
   }


   void simplifications::fully_simplify( sequent& seq )
   {
      for( std::list< formula > :: iterator 
              p = seq. formulas. begin( );
	      p != seq. formulas. end( );
	      ++ p )
      {
         *p = flatten( *p ); 
	 *p = truth_constants( *p ); 
         *p = antiprenex( *p );
	 *p = flatten( *p ); 
      }
   }


} 


