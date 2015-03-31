

// Written by Hans de Nivelle, July 2006.

#include "sequent.h"
#include <cstdlib>


namespace firstorder
{
  
   void sequent::addidentifiers( term t )
   {
      function f = t. getfunction( );
      usedfunctions [f] = true;

      for( std::list< term > :: const_iterator 
              p = t. subterms_begin( );
	      p != t. subterms_end( );
	      ++ p )
      {
         addidentifiers( *p );
      }
   }

   
   void sequent::addidentifiers( formula f )
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
      case fol_exists:
      case fol_forall:

         if( f. getoperator( ) == fol_exists ||
             f. getoperator( ) == fol_forall )
         {
	    for( std::list< function > :: const_iterator
                    p = f. variables_begin( );
		    p != f. variables_end( );
		    ++ p )
            {
               usedfunctions [ *p ] = true;
            }
         }

         for( std::list< formula > :: const_iterator
                 p = f. subformulas_begin( );
		 p != f. subformulas_end( );
		 ++ p )
         {
            addidentifiers( *p );
         }
	 return;

      case fol_equals:
      case fol_atom:
         if( f. getoperator( ) == fol_atom )
         {
            usedpredicates[ f. getpredicate( ) ] = true; 
         }

	 for( std::list< term > :: const_iterator
                 p = f. subterms_begin( );
		 p != f. subterms_end( );
		 ++ p )
         {
            addidentifiers( *p );
         }
	 return; 
      default:
         ASSERT( false ); exit(0); 
      }
   }


   void sequent::collectusedidentifiers( )
   {
   
      usedpredicates. clear( );
      usedfunctions. clear( );

      for( std::list< formula > :: const_iterator
              p = formulas. begin( );
	      p != formulas. end( );
	      ++ p )
      {
         addidentifiers( *p );               
      }
   }


   namespace
   {
   
      std::string converttostring( unsigned int x )
      {
         if( x >= 10 )
	 {
	    char c = ( x % 10 ) + '0';
	    x = x / 10;
	    std::string res = converttostring(x);
	    res += c;
	    return res;
         }
         else
         {
	    std::string res;
	    char c = x + '0';
	    res += c;
	    return res;
         }
      }

   } 


   predicate sequent::freshsubformulapredicate( unsigned int arity ) 
   {
   
      std::string pp = std::string( "pppp" );
      predicate p = 
	 predicate( 
	       pp + converttostring( formulapredicate_cnt ++ ), 
	       arity );

      while( usedpredicates [ p ] ) 
      {
	 p = predicate( 
		  pp + converttostring( formulapredicate_cnt ++ ), arity );
      }

      usedpredicates [p] = true;
         // We assume that the user has the intention of using p. 

      return p;
   }


   predicate sequent::freshfunctionpredicate( function f ) 
   {
      std::string s = std::string( "P_" ) + f. getstring( );
      unsigned int ar = f. getarity( );

      predicate p = predicate( s, ar + 1 );
      if( ! usedpredicates [p] )
      {
         usedpredicates [p] = true;
	 return p;
      }

      unsigned int cntr = 1;
      s = std::string( "P_" ) + f. getstring( ) + 
	           "_" + converttostring( cntr ++ );
      p = predicate( s, ar + 1 ); 
      while( usedpredicates [p] )
      {
         s = std::string( "P_" ) + f. getstring( ) + 
                "_" + converttostring( cntr ++ );
         p = predicate( s, ar + 1 ); 
      }
      usedpredicates [p] = true;
         // We assume that the predicate will be used. 
      return p;
   }


   function sequent::freshtermvariable( ) 
   {
      std::string aa = "alpha";
      function f = function(
		        aa + converttostring( termvariable_cnt ++ ), 0 ); 
    
      while( usedfunctions [f] )
      {
         f = function(
                 aa + converttostring( termvariable_cnt ++ ), 0 );
      }

      usedfunctions [f] = true;
         // We assume that the variable will be used. 
      return f;
   }



   std::ostream& operator << ( std::ostream& stream,
		               const sequent& s )
   {
      unsigned int formcntr = 0;
      for( std::list< formula > :: const_iterator
              p = s. formulas. begin( );
	      p != s. formulas. end( );
	      ++ p, ++ formcntr )
      {
         stream << formcntr << " :  ";
	 stream << *p << "." << "\n";
      }
      stream << "\n\n";

#if 0
      stream << "Used Predicate Symbols:\n"; 
      for( std::map< predicate, bool > :: const_iterator
              p = s. usedpredicates. begin( );
	      p != s. usedpredicates. end( );
	      ++ p )
      {
         if( p != s. usedpredicates. begin( ))
            stream << ", "; 
         stream << ( p -> first ) << "/" << ( p -> first. getarity( )); 
      }
      stream << "\n";

      stream << "Used Function Symbols:\n"; 
      for( std::map< function, bool > :: const_iterator
              p = s. usedfunctions. begin( );
	      p != s. usedfunctions. end( );
	      ++ p )
      {
         if( p != s. usedfunctions. begin( ))
            stream << ", ";
	 stream << ( p -> first ) << "/" << ( p -> first. getarity( ));
      }
      stream << "\n\n";
#endif       
      return stream; 
   }

} 




