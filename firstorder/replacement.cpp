

// Code written by Hans de Nivelle, July 2006.


#include "replacement.h"


namespace firstorder
{

   namespace
   {

      void addfreevariables( 
         const bindingstack & boundoutside,
	 const bindingstack & boundinside,
	 term t, 
	 std::map< function, bool > & res ) 
      {
         function f = t. getfunction( );

	 if( f. getarity( ) == 0 && 
	     boundoutside. isbound(f) && 
	     ! boundinside. isbound(f) )
         {
            res [f] = true;  
         }

	 for( std::list< term > :: const_iterator 
                 p = t. subterms_begin( );
		 p != t. subterms_end( );
		 ++ p )
         {
            addfreevariables( boundoutside, boundinside, *p, res );
         }
      }


            
      void addfreevariables( 
         const bindingstack& boundoutside,
	 bindingstack& boundinside,
	 formula f, 
	 std::map< function, bool > & res )
      {
         
         switch( f. getoperator( ))
         {
         case fol_false:
         case fol_true:
         case fol_not:
         case fol_and:
         case fol_or:
         case fol_implies:
         case fol_equiv:
            for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               addfreevariables( boundoutside, boundinside, *p, res );
	    }
	    return; 

         case fol_forall:
         case fol_exists:
	    for( std::list< function > :: const_iterator
                    p = f. variables_begin( );
		    p != f. variables_end( );
		    ++ p )
            {
               boundinside. bind( *p );
            }

            addfreevariables( boundoutside, boundinside,
			      * f. subformulas_begin( ), res );

	    for( std::list< function > :: const_iterator
                    p = f. variables_begin( );
		    p != f. variables_end( );
		    ++ p )
            {
               boundinside. release( *p );
            }
            return;

         case fol_equals:
         case fol_atom:
            for( std::list< term > :: const_iterator
                    p = f. subterms_begin( );
		    p != f. subterms_end( );
		    ++ p )
            {
               addfreevariables( boundoutside, boundinside, *p, res );
	    }
            return;
         }

      }

   } 


   std::map< function, bool > 
   replacement::freevariables( const bindingstack& bound, formula f )
   {
      std::map< function, bool > res; 
      bindingstack boundinside;
      addfreevariables( bound, boundinside, f, res );
      return res; 
   }


   std::map< function, bool > 
   replacement::freevariables( const bindingstack& bound, 
		               const std::list< formula > & fl )
   {
      std::map< function, bool > res;
      bindingstack boundinside;

      for( std::list< formula > :: const_iterator
              p = fl. begin( );
	      p != fl. end( );
	      ++ p )
      {
         addfreevariables( bound, boundinside, *p, res );
      }
      return res; 
   }


     
   formula replacement::make_atom( const std::list< function > & vars,
		                   sequent& seq )
   {
      predicate p = seq. freshsubformulapredicate( vars. size( ));

      std::list< term > empty;

      std::list< term > args;
      for( std::list< function > :: const_iterator
               v = vars. begin( );
	       v != vars. end( );
	       ++ v )
      {
         args. push_back( term( *v, empty ));
      }

      return formula::make_atom( p, args );
   }


   formula replacement::positive_definition( formula atom, formula f )
   {
      // Collect the list of variables:

      std::list< function > vars;
      for( std::list< term > :: const_iterator 
              p = atom. subterms_begin( );
	      p != atom. subterms_end( );
	      ++ p )
      {
         ASSERT( p -> getfunction( ). getarity( ) == 0 );
	 vars. push_back( p -> getfunction( ));
      }
     
      std::list< formula > def;
      def. push_back( formula::make_not( atom ));
      def. push_back( f );
      return formula::make_forall( vars, formula::make_or( def ));
   }


   formula replacement::full_definition( formula atom, formula f )
   {
      // Collect the list of variables:

      std::list< function > vars;
      for( std::list< term > :: const_iterator
              p = atom. subterms_begin( );
              p != atom. subterms_end( );
              ++ p )
      {
         ASSERT( p -> getfunction( ). getarity( ) == 0 );
	 vars. push_back( p -> getfunction( ));
      }

      return formula::make_forall( vars, formula::make_equiv( atom, f ));
   }


   std::list< function >
   replacement::converttolist( const std::map< function, bool > & varlist )
   {
      std::list< function > result;
      for( std::map< function, bool > :: const_iterator
              p = varlist. begin( );
	      p != varlist. end( );
	      ++ p )
      {
         if( p -> second ) 
            result. push_back( p -> first );
      }
      return result;
   }

} 


