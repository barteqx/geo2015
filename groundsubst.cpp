

// Code written by Hans de Nivelle, May 2005.


#include "groundsubst.h"


const element* groundsubst::lookup( variable v ) const
{

   for( std::vector< assignment > :: const_iterator p = repr. begin( );
           p < repr. end( );
	   p ++ )
   {
      if( ( p -> v ) == v )
      {
	 return & ( p -> e );
      }
   }
   return 0;
}


groundatom groundsubst::apply_on( varatom va ) const
{
   static std::vector< element > base;
   base. clear( );

   for( std::vector< variable > :: const_iterator p = va. a1;
        p != va. a2;
	++ p )
   {
      const element* e = lookup( *p );
      ASSERT(e); 
      base. push_back( *e );
   }
   return groundatom( va. p, base );
}


void groundsubst::assign( variable v, element e )
{
   ASSERT( lookup(v) == 0 );
   repr. push_back( groundsubst::assignment( v, e ));
}


variable groundsubst::sup_variables( ) const
{
   variable sup;

   for( std::vector< groundsubst::assignment > :: const_iterator 
	   p = repr. begin( );
        p != repr. end( );
	++ p )
   {
      variable v = ( p -> v );
      ++ v;

      if( v > sup )
         sup = v;
   }
   return sup;
}


element groundsubst::sup_elements( ) const
{
   element sup;

   for( std::vector< groundsubst::assignment > :: const_iterator 
	   p = repr. begin( );
           p != repr. end( );
	   ++ p )
   {
      element e = ( p -> e );
      ++ e;

      if( e > sup )
         sup = e;
   }
   return sup;
}


varset groundsubst::getdomain( ) const
{
   varset res;
   for( std::vector< assignment > :: const_iterator
           p = repr. begin( );
	   p != repr. end( );
	   ++ p )
   {
      res. insert( p -> v );
   }
   return res;
}


varset groundsubst::getaddedvars( unsigned int x ) const
{
   varset res;
   ASSERT( x <= size( ));
   
   for( std::vector< assignment > :: const_iterator
           p = repr. begin( ) + x;
	   p != repr. end( );
	   ++ p )
   {
      res. insert( p -> v );
   }
   return res;
}


bool groundsubst::match( varatom va, groundatom at )
{
   if( va. p != at. p ) 
      return false;
   
   std::vector< variable > :: const_iterator p = va. a1;
   std::vector< element > :: const_iterator q = at. a1;

   while( p != va. a2 )
   {
      const element* e = lookup( *p );
      if(e)
      {
         if( *e != *q )
         {
	    return false;
         }
      }
      else
         assign( *p, *q );

      ++ p;
      ++ q;
   }
   return true;
}


bool groundsubst::consistentwith( const groundsubst& other ) const 
{
   for( std::vector< assignment > :: const_iterator
           p = repr. begin( );
	   p != repr. end( );
	   ++ p )
   {
      const element* e = other. lookup( p -> v );
      if( e && *e != ( p -> e ))
         return false;
   }
   return true;
}


groundsubst operator + ( const groundsubst& g1, 
		         const groundsubst& g2 )
{
   groundsubst res = g1;

   for( std::vector< groundsubst::assignment > :: const_iterator
           p = g2. repr. begin( );
	   p != g2. repr. end( );
	   ++ p )
   {
      const element* e = g1. lookup( p -> v );
      ASSERT( !e || *e == ( p -> e ));
      if( !e )
      {
         res. repr. push_back( groundsubst::assignment( p -> v, p -> e ));
      }
   }
   return res;
}

   
groundsubst groundsubst::restrict_to( const varset& s ) const
{
   groundsubst res;

   for( std::vector< groundsubst::assignment > :: const_iterator 
           p = repr. begin( );
	   p != repr. end( );
	   ++ p )
   {
      if( s. contains( p -> v ))
         res. assign( p -> v, p -> e );
   }

   return res;
}


std::ostream& operator << ( std::ostream& stream,
		            const groundsubst& s )
{
   stream << "GroundSubstitution( ";
   for( std::vector< groundsubst::assignment > :: const_iterator 
	   p = s. repr. begin( );
           p !=  s. repr. end( );
	   ++ p )
   {
      if( p != s. repr. begin( ))
         stream << ",  "; 
      stream << ( p -> v ) << " := " << ( p -> e );
   }
   stream << " )";
   return stream;
}


const groundsubst::assignment* 
groundsubst::nextassignment( variable v ) const
{
   const assignment* res = 0;

   for( std::vector< groundsubst::assignment > :: const_iterator 
           p = repr. begin( );
	   p != repr. end( );
	   ++ p )
   {
      if( ( p -> v ) >= v ) 
      {
         if( res == 0 )
         {
            res = & (*p);
         }
         else
         {
            if( ( p -> v ) < ( res -> v ))
               res = & (*p);
         }
      }
   }

   return res;
}


// The order is defined as follows:
// Find smallest variable v, where s1[v] differs from s2[v].
// Then we put
//    e0 < e1 < e2 ........ < undefined.


int groundsubst::compare( const groundsubst& s1, const groundsubst& s2 )
{
   variable v;

   ASSERT( s1. size( ) == s2. size( ));
   for( unsigned int i = 0; i < s1. size( ); ++ i )
   {
      ASSERT( s1. repr [i]. v == s2. repr [i]. v );

      if( s1. repr [i]. e < s2. repr [i]. e )
         return -1;
      if( s1. repr [i]. e > s2. repr [i]. e )
         return 1;

   }
   return 0;


   while( true )
   {
      const assignment* a1 = s1. nextassignment( v );
      const assignment* a2 = s2. nextassignment( v );

      if( a1 != 0 && a2 == 0 )
         return -1;
      if( a1 == 0 && a2 != 0 )
         return 1;
      if( a1 == 0 && a2 == 0 )
	 return 0;

      if( ( a1 -> v ) < ( a2 -> v ))
         return -1;
      if( ( a1 -> v ) > ( a2 -> v ))
         return 1;

      if( ( a1 -> e ) < ( a2 -> e ))
         return -1;
      if( ( a1 -> e ) > ( a2 -> e ))
         return 1;

      v = ( a1 -> v );
      ++ v;
   }
}


