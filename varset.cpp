
// Code written by Hans de Nivelle, Jan 2006.


#include "varset.h"



bool varset::contains( variable v ) const
{
   for( std::vector< variable > :: const_iterator it = repr. begin( );
        it != repr. end( );
	++ it )
   {
      if( *it == v )
         return true;
   }
   return false;
}


unsigned int varset::size( ) const
{
   return repr. size( );
}


void varset::clear( )
{
   repr. clear( );
}


void varset::insert( variable v )
{
   for( std::vector< variable > :: const_iterator it = repr. begin( );
        it != repr. end( );
	++ it )
   {
      if( *it == v )
         return;
   }
   repr. push_back(v);
}


void varset::remove( variable v )
{
   for( std::vector< variable > :: iterator it = repr. begin( );
        it != repr. end( );
        ++ it )
   {
      if( *it == v )
      {
         repr. erase( it );
         return;
      }
   }
}


variable varset::supremum( ) const
{
   variable sup;

   for( std::vector< variable > :: const_iterator it = repr. begin( );
        it != repr. end( );
	++ it )
   {
      variable v = *it;
      ++ v;
      if( v > sup )
         sup = v;
   }
   return sup;
}


varset operator | ( const varset& v1, const varset& v2 ) 
{
   varset res = v1;
   for( std::vector< variable > :: const_iterator it = v2. repr. begin( );
        it != v2. repr. end( );
	++ it )
   {
      res. insert( *it );
   }
   return res;
}


varset operator & ( const varset& v1, const varset& v2 )
{
   varset res;

   for( std::vector< variable > :: const_iterator it1 = v1. repr. begin( );
        it1 != v1. repr. end( );
	++ it1 )
   {
      for( std::vector< variable > :: const_iterator it2 = v2. repr. begin( );
           it2 != v2. repr. end( );
	   ++ it2 )
      {
         if( *it1 == *it2 )
            res. insert( *it1 );
      }
   }
   return res;
}


varset operator - ( const varset& v1, const varset& v2 )
{
   varset res;

   for( std::vector< variable > :: const_iterator it = v1. repr. begin( );
        it != v1. repr. end( );
	++ it )
   {
      if( ! v2. contains( *it ))
         res. insert( *it );
   }
   return res;
}


std::ostream& operator << ( std::ostream& stream,
		            const varset& vs )
{
   stream << "{";

   for( auto it = vs. vars_cbegin( ); it != vs. vars_cend( ); ++ it )
   {
      if( it != vs. vars_cbegin( ))
         stream << ",";
      stream << *it;
   }
   stream << "}"; 
   return stream; 
}


