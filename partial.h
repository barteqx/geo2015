
// Written by Hans de Nivelle,
// November 2005.


// A partial object is an object that does or does not exist.


#ifndef PARTIAL_INCLUDED
#define PARTIAL_INCLUDED   1

#include <iostream> 
#include <vector>

#include "assert.h"


template< class X > class partial
{

private:
   std::vector< X > repr;

public:
   partial( const X& val )
   {
      repr. push_back( val );
   }

   partial( )
   {
   }


   bool isdefined( ) const
   {
      return repr. size( ) == 1;
   }

   bool isundefined( ) const
   {
      return repr. size( ) == 0;
   }

   void clear( )
   {
      repr. clear( );
   }

   void operator = ( const partial< X > & other )
   {
      repr = other. repr;
   }

   void operator = ( const X& other )
   {
      if( repr. size( ) == 0 )
         repr. push_back( other );
      else
         repr [0] = other;
   }


   const X& contents( ) const
   {
      ASSERT( repr. size( ));
      return repr [0];
   }

   X& contents( ) 
   {
      ASSERT( repr. size( ));
      return repr [0];
   }

};


template< class X >
std::ostream& operator << ( std::ostream& stream, const partial<X> & p )
{
   if( p. isundefined( ))
      stream << "UNDEFINED";
   else
      stream << p. contents( );

   return stream;
}


#endif

