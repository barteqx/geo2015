

// Code written by Hans de Nivelle.


#ifndef GROUNDATOM_INCLUDED
#define GROUNDATOM_INCLUDED   1


#include <iostream>
#include <vector>


#include "predicate.h"
#include "element.h"


struct groundatom 
{
   predicate p;
   std::vector< element > :: const_iterator a1; 
   std::vector< element > :: const_iterator a2; 

   // The arity of n should fit to a2 - a1;
   // A groundatom is not an independent object. It
   // remains valid only as long as
   // the iterators a1, a2 are valid, which point into some
   // vector that one should preserve. 

   groundatom( predicate p, std::vector< element > :: const_iterator a1,
		            std::vector< element > :: const_iterator a2 ) 
      : p(p), a1(a1), a2(a2) 
   {
      ASSERT( a2 - a1 == p. getarity( ));
   }

   groundatom( predicate p, const std::vector< element > & args ) 
      : p(p), a1( args. begin( )), a2( args. end( ))
   {
      ASSERT( args. size( ) == p. getarity( )); 
   }

   unsigned int getarity( ) const
   {
      return a2 - a1;
   }

};


bool operator == ( const groundatom& g1, const groundatom& g2 );
inline bool operator != ( const groundatom& g1, const groundatom& g2 )
{
   if( g1 == g2 )
      return false;
   else
      return true;
}


std::ostream& operator << ( std::ostream& stream,
		            const groundatom& at ); 


#endif


