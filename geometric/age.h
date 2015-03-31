
#ifndef GEOMETRIC_AGE_INCLUDED
#define GEOMETRIC_AGE_INCLUDED 1

#include <iostream>
#include <chrono>

namespace geometric
{
   struct age
   {
      using clock = std::chrono::steady_clock;
      clock::time_point t;

      age( ) 
         :t{ clock::now( ) }
      { }

      void restart( )
         { t = clock::now( ); }

      unsigned int seconds_passed( ) const
         { return std::chrono::duration_cast< std::chrono::seconds >
              ( age::clock::now( ) - t ). count( ); }
   };

   std::ostream& operator << ( std::ostream& out, const age& a );
}

#endif

