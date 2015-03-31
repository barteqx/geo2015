
// Written by Hans de Nivelle, June 2014.

#include "timepoints.h" 

std::ostream& geometric::operator << ( std::ostream& stream,
                                       const timepoints& t )
{
   stream << '{';
   for( auto p = t. moments. begin( ); p != t. moments. end( ); ++ p )
   {
      if( p != t. moments. begin( ))
         stream << ',';

      stream << *p;
   }
   stream << '}';
   return stream;
}

 
