
// Written by Hans de Nivelle, Jan 2015.

#include "best_false_instances.h"

void geometric::best_false_instances::insert( false_instance&& f ) 
{
   auto p = lst. begin( );
   while( p != lst. end( ) && ! ( p -> isworsethan(f) ))
   {
      ++ p;
   }

   if( p == lst. end( ))
   {
      if( lst. size( ) < max )  
         lst. push_back( std::move(f));
   }
   else
   {
      lst. insert( p, std::move(f));  
      if( lst. size( ) > max )
         lst. pop_back( );
   }
}


std::ostream& geometric::operator << ( std::ostream& out,
                                       const best_false_instances& best )
{
   out << "Best:\n";
   
   unsigned int nr = 0; 
   for( const auto& f : best )
   {
      out << (nr++) << " : " << f;
   }

   return out;
}


