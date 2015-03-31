
// Written by Hans de Nivelle, June 2014.

#include "reporter.h"

void statistics::reporter::wasreported( )
{
   while( mustreport( ))
   {
      if( nextreport < 60.0 )
         nextreport = 60.0;
      else
      {
         if( nextreport < 600.0 )
            nextreport += 60.0;      // Every minute up to 10 minutes.
         else
         {
            if( nextreport < 60.0 * 60.0 )
               nextreport += 10.0 * 60.0; // Every 10 minutes up to 60 minutes.
            else
               nextreport += 60.0 * 60.0; // Every 60 minutes forever.
         }
      }
   }
}


