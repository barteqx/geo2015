
// Written by Hans de Nivelle, June 2014.

#ifndef STATISTICS_TIMER_INCLUDED
#define STATISTICS_TIMER_INCLUDED  1

#include <functional>
#include <chrono>
#include <iostream>

namespace statistics
{
   // A timer remembers the time of its creation.
   // When it is destroyed, it calculates how
   // long it existed, and it calls its callback function
   // with the resulting time, represented as double in seconds. 
   // 
   // A timer can be used for calculating how much time is spent in
   // a function, the average execution time, the distribution of
   // execution times. etc.

   class timer
   {
      std::function< void( double ) > callback;
         // This function is called when the timer is destroyed.

      using clock = std::chrono::steady_clock;

      clock::time_point start;
         // Moment of creation.

   public: 
      timer( ) : 
         start{ clock::now( ) }
      { }

      timer( const std::function< void( double ) > & callback )
         : start{ clock::now( ) },
           callback{ callback }
      { }

      timer( timer&& t ) :
         start{ t. start },
         callback{ std::move( t. callback ) }
      { t. callback = nullptr; }

      timer( const timer& t ) = delete;
      void operator = ( const timer& t ) = delete;

      double duration( ) const 
      {   
         std::chrono::duration< double > d = clock::now( ) - start;
         return d. count( ); 
      }

      ~timer( )
      { 
         if( callback )
            callback( duration( ));
      }
   };


   inline std::ostream& operator << ( std::ostream& out, const timer& t )
   {
      out << "t = " << t. duration( ); 
      return out;
   }

}

#endif
 
