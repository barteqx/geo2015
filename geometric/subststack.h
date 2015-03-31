
// Written by Hans de Nivelle, December 2014.
// A subststack consists of a substitution and a stack of variables
// for backtracking.

#ifndef GEOMETRIC_SUBSTSTACK_INCLUDED
#define GEOMETRIC_SUBSTSTACK_INCLUDED   1

#include <stack>
#include "subst.h"

namespace geometric
{
   template< typename D >
   struct subststack
   {
   
      subst<D> s;
      std::stack<variable> varstack;


      D* lookup( variable v ) { return s. lookup(v); }
      const D* lookup( variable v ) const { return s. clookup(v); }
      const D* clookup( variable v ) const { return s. clookup(v); } 

      bool assign( variable v, const D& d )
      { 
         bool b = s. assign( v, d ); 
         if(b) varstack. push(v);   // If assigned, we push v on stack.
         return b;
      }

      size_t size( ) const { return s. size( ); } 

      void restore( size_t k ) 
      {
         while( s. size( ) > k )
         {
            s. remove( varstack. top( ));
            varstack. pop( );
         }
      }

      bool match( variable v, const D& d );
         // If the value of v is already d, do nothing and return true.
         // If v is undefined, assign v:=d and return true.
         // If the value of v is something else, do nothing and return false.

      template< size_t S > 
      bool match_arguments( const varatom& from, const atom<D,S> & into );
         // Assumes that from and into have the same arity.
         // We use operator != for testing inequality.
         // There are many possible conditions on the truth-values, and
         // one cannot make a matching function for each of them. 
         // Equality is not specially treated.

   };


   template< typename D >
   inline bool subststack<D> :: match( variable v, const D& d ) 
   {
      auto p = clookup(v);
      if(p)
      {
         if( *p != d )
            return false;
         else
            return true; 
      }
      else
      {
         assign( v, d );
         return true;
      }
   }


   // C++ does not believe in Currying: 

   template< typename D > template< size_t S >
   inline bool 
   subststack<D> :: match_arguments( const varatom& from, 
                                     const atom<D,S> & into )
   {
      auto a1 = from. args_cbegin( );
      auto a2 = into. args_cbegin( );

      while( a1 != from. args_cend( ))
      {
         if( !match( *a1, *a2 )) 
            return false; 
         ++ a1; ++ a2;
      }
      return true;
   }


   template< typename D >
   std::ostream& operator << ( std::ostream& out, const subststack<D> & s )
   {
      out << "subststack\n";
      out << "   " << s. s << "\n";
      out << "   stack size = " << s. varstack. size( ) << "\n";
      return out; 
   }

} 
      
#endif

