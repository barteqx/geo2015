

// Code written by Hans de Nivelle, February 2006. 


#include "varmatching.h"


const variable* varmatching::lookup( variable v ) const
{

   for( std::vector< assignment > :: const_iterator 
	   p = repr. begin( );
           p != repr. end( );
	   p ++ )
   {
      if( ( p -> v1 ) == v )
      {
	 return & ( p -> v2 );
      }
   }
   return 0;
}


varatom varmatching::apply_on( varatom va ) const
{
   static std::vector< variable > base;
   base. clear( );

   for( std::vector< variable > :: const_iterator 
	   p = va. a1;
           p != va. a2;
	   ++ p )
   {
      const variable* v = lookup( *p );
      ASSERT(v); 
      base. push_back( *v );
   }
   return varatom( va. p, base );
}


void varmatching::assign( variable v1, variable v2 )
{
   ASSERT( lookup(v1) == 0 );
   repr. push_back( assignment( v1, v2 ));
}


varset varmatching::getdomain( ) const
{
   varset res;

   for( std::vector< varmatching::assignment > :: const_iterator
           p = repr. begin( );
	   p != repr. end( );
	   ++ p )
   {
      res. insert( p -> v1 );
   }
   return res;
}


varset varmatching::getrange( ) const
{
   varset res;

   for( std::vector< varmatching::assignment > :: const_iterator
           p = repr. begin( );
	   p != repr. end( );
	   ++ p )
   {
      res. insert( p -> v2 );
   }
   return res;
}


varset varmatching::getaddedvars( unsigned int x ) const 
{
   varset res;

   ASSERT( x <= size( )); 

   for( std::vector< varmatching::assignment > :: const_iterator
           p = repr. begin( ) + x;
	   p != repr. end( );
	   ++ p )
   {
      res. insert( p -> v1 );
   }
   return res;
}


bool varmatching::match( varatom va1, varatom va2 )
{
   if( va1. p != va2. p ) 
      return false;
   
   std::vector< variable > :: const_iterator p = va1. a1;
   std::vector< variable > :: const_iterator q = va2. a1;

   while( p != va1. a2 )
   {
      const variable* v = lookup( *p );
      if(v)
      {
         if( *v != *q )
	    return false;
      }
      else
         repr. push_back( assignment( *p, *q ));

      ++ p;
      ++ q;
   }
   return true;
}


bool varmatching::consistentwith( const varmatching& other ) const
{
   for( std::vector< assignment > :: const_iterator 
           p = repr. begin( );
	   p != repr. end( );
	   ++ p )
   {
      const variable* v = other. lookup( p  -> v1 );  
      if( v && *v != ( p -> v2 ))
         return false; 
   }

   return true; 
}


varmatching operator + ( const varmatching& m1,
		         const varmatching& m2 ) 
{
   varmatching res = m1;

   for( std::vector< varmatching::assignment > :: const_iterator
           p = m2. repr. begin( );
	   p != m2. repr. end( );
	   ++ p )
   {
      const variable* v = m1. lookup( p -> v1 );
      ASSERT( !v || *v == ( p -> v2 ));
      if( !v )
      {
         res. repr. push_back( 
		     varmatching::assignment( p -> v1, p -> v2 ));
      }
   }
   return res; 
}


std::ostream& operator << ( std::ostream& stream,
		            const varmatching& s )
{
   stream << "VariableMatching( ";
   for( std::vector< varmatching::assignment > :: const_iterator 
	   p = s. repr. begin( );
           p !=  s. repr. end( );
	   ++ p )
   {
      if( p != s. repr. begin( ))
         stream << ",  "; 
      stream << ( p -> v1 ) << " := " << ( p -> v2 );
   }
   stream << " )";
   return stream;
}


