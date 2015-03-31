

// Code written by Hans de Nivelle, November 2005. 


#include "normalizer.h"


std::ostream& operator << ( std::ostream& stream,
		            const normalizer& n )
{
   stream << "Normalizer: \n\n";
   for( std::vector< normalizer::assignment > :: const_iterator 
	   p = n. repr. begin( );
           p !=  n. repr. end( );
	   ++ p )
   {
      stream << "(" << ( p -> from_lab ) << "," << ( p -> from_var ) << ")";
      stream << " := ";
      stream << ( p -> var );
      stream << "\n"; 
   }
   stream << "\n";
   stream << "the next free variable is " << n. nextvar << "\n"; 
   return stream;
}




