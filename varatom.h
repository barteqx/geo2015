
// Code written by Hans de Nivelle, May 2005. 


#ifndef VARATOM_INCLUDED
#define VARATOM_INCLUDED   1


#include <iostream>
#include <vector>


#include "predicate.h"
#include "variable.h"


struct varatom 
{
   predicate p;
   std::vector< variable > :: const_iterator a1; 
   std::vector< variable > :: const_iterator a2; 

   // A variable atom is an atom the arguments of 
   // which are variables. 
   // The arity of n should fit to a2 - a1.
   // One should keep in mind that the varatom is not an independent
   // object. The varatom will remain valid only as long as
   // the iterators a1, a2 remain valid. 


   varatom( predicate p, const std::vector< variable > :: const_iterator a1, 
		         const std::vector< variable > :: const_iterator a2 )
      :p(p), a1(a1), a2(a2)
   {
   }

   varatom( predicate p, const std::vector< variable > & args ) 
      : p(p), a1( args. begin( )), a2( args. end( ))
   {
      ASSERT( args. size( ) == p. getarity( ));
   }

   unsigned int getarity( ) const
   {
      return a2 - a1;
   }

};


bool operator == ( const varatom& v1, const varatom& v2 );
inline bool operator != ( const varatom& v1, const varatom& v2 )
{
   if( v1 == v2 )
      return false;
   else
      return true;
}


std::ostream& operator << ( std::ostream& , const varatom& );


#endif


