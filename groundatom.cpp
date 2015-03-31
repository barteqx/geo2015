

// Code written by Hans de Nivelle, May 2005. 


#include "groundatom.h"


std::ostream& operator << ( std::ostream& stream,
		            const groundatom& at )
{
   if( at. p == predicate::create )
   {
      stream << at. p << " " << *at. a1;
      return stream;
   }

   if( at. p == predicate::diseq )
   {
      stream << *at. a1 << " " << at. p << " " << * ( at. a1 + 1 );
      return stream;
   }

   stream << at. p;
   if( at. a2 - at. a1 > 0 )
   {
      stream << "(";
      for( std::vector< element > :: const_iterator e = at. a1; 
	   e != at. a2; 
	   ++ e )
      {
         stream << *e;
	 if( e != at. a2 - 1 )
            stream << ",";
      }
      stream << ")";
   }

   return stream;
}


bool operator == ( const groundatom& g1, const groundatom& g2 )
{
   if( g1.p != g2.p )
      return false;
   else
   {
      std::vector< element > :: const_iterator x1 = g1. a1;
      std::vector< element > :: const_iterator x2 = g2. a1;
      while( x1 != g1. a2 )
      {
         if( *x1 != *x2 )
            return false;

	 ++ x1;
	 ++ x2;
      }
   }
   return true;
}


