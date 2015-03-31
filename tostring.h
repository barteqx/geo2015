
#ifndef TOSTRING_INCLUDED
#define TOSTRING_INCLUDED  1

#include <sstream>

template< class X >
std::string tostring( const X& x )
{
   std::ostringstream out;
   out << x;
   return out. str( );
}

#endif 

