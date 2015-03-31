
// Written by Hans de Nivelle, June 2014.

#ifndef STATISTICS_REPORTER
#define STATISTICS_REPORTER  1

#include <chrono>
#include <iostream>

namespace statistics
{

   // The idea of class reporter is that you can write code like this:
   // 
   // static reporter r; 
   // static unsigned int counter = 0;
   // counter ++ ;
   // if( r. mustreport( ))
   // {  
   //    std::cout << "this function was called " << counter << "times\n";  
   //    std::cout << "at time " << r. duration( ) << "\n"; 
   //    r. wasreported( ); 
   // }
   //
   // report will report at times (in minutes)
   // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
   // 10, 20, 30, 40, 50, 60,       (every 10 minutes).
   // 120, 180, 240, ...            (every 60 minutes). 
   // 
   // Don't forget to call wasreported( ).

   struct reporter
   {
      using clock = std::chrono::steady_clock;

      clock::time_point start;
   
      double nextreport; 
         // Time (in seconds since our moment of creation) on
         // which a new report should happen. 

      reporter( )
         : start( clock::now( )),
           nextreport{ 0.0 } 
      { }
    
      reporter( const reporter& ) = delete;
      void operator = ( const reporter& ) = delete;

 
      // Returns time (in seconds since our moment of creation). 
 
      double duration( ) const
      {
         std::chrono::duration< double > d = ( clock::now( ) - start );
         return d. count( ); 
      }

      bool mustreport( ) const
      {
         return duration( ) >= nextreport;
      }

      void wasreported( );
         // Compute next reporting time. 

      void printduration( std::ostream& out ) const
      {
         double d = duration( );
         out << "duration = " << d;
         out << " (" << ( d / 60.0 ) << " min)";
         out << " (" << ( d / 3600.0 ) << " hr)\n";
      }

   };
}

#endif

