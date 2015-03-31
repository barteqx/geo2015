
// Written by Hans de Nivelle, July 2014.

#ifndef SWLL_MATCHCOUNTER_INCLUDED
#define SWLL_MATCHCOUNTER_INCLUDED 1

#include <iostream>

namespace swll
{
   struct matchcounter 
   {
      unsigned int attempted;       // Attempted. 
      unsigned int constructed;     // Passed construction. 
      unsigned int interreduced;    // Passed interreduction filter. 
      unsigned int succeeded;       // Succeeded in matching. 

      matchcounter( )
         : attempted{0},
           constructed{0},
           interreduced{0},
           succeeded{0}
      { } 

      double ratio( double d ) const
      {
         if( attempted != 0 )
            return static_cast<double>( d ) / 
                   static_cast<double>( attempted ); 
         else
            return 0;
      }

   };


   inline std::ostream& operator << ( std::ostream& out,
                                      const matchcounter& m )
   {
      out << "SWLL Statistics:\n";
      out << "   attempted:     " << m. attempted << "\n";

      out << "   constructed:   " << m. constructed; 
      out << " (" << m. ratio( m. constructed ) << ")\n";

      out << "   interreduced:  " << m. interreduced; 
      out << " (" << m. ratio( m. interreduced ) << ")\n";

      out << "   succeeded:     " << m. succeeded; 
      out << " (" << m. ratio( m. succeeded ) << ")\n";

      return out; 
   }

}

#endif 

