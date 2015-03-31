
// Written by Hans de Nivelle, 01.06.2014.

#ifndef GEOMETRIC_TRUTHVAL_INCLUDED
#define GEOMETRIC_TRUTHVAL_INCLUDED  1

#include <stdexcept>

namespace geometric
{

   // truthval represents a set of truth values:
 
   enum class truthval : unsigned char 
   {
      empty = 0, 
      F = 1,
      E = 2,
      T = 4,
      FE = 3, 
      FT = 5, 
      ET = 6, 
      FET = 7 
   };

   inline std::ostream& operator << ( std::ostream& out, truthval t )
   {
      switch(t)
      {
      case truthval::empty:
         out << "(empty)"; return out; 
      case truthval::F: 
         out << "F"; return out; 
      case truthval::E:
         out << "E"; return out; 
      case truthval::T:
         out << "T"; return out; 
      case truthval::FE:
         out << "FE"; return out;
      case truthval::FT:
         out << "FT"; return out;
      case truthval::ET:
         out << "ET"; return out;
      case truthval::FET:
         out << "FET"; return out; 
      default:
         throw std::runtime_error( "fall through with truthval" ); 
      }
   }

   inline truthval operator | ( truthval t1, truthval t2 ) 
   {
      return static_cast< truthval > ( static_cast< unsigned char > ( t1 ) |
                                       static_cast< unsigned char > ( t2 ) );
   }

   inline truthval operator & ( truthval t1, truthval t2 )
   {
      return static_cast< truthval > ( static_cast< unsigned char > ( t1 ) &
                                       static_cast< unsigned char > ( t2 ));
   }

   inline void operator |= ( truthval& t1, truthval t2 )
   {
      t1 = ( t1 | t2 );
   }

   inline void operator &= ( truthval& t1, truthval t2 )
   {
      t1 = ( t1 & t2 );
   }

}

#endif 

