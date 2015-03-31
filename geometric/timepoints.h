
// Written by Hans de Nivelle, June 2014.

#ifndef GEOMETRIC_TIMEPOINTS_INCLUDED
#define GEOMETRIC_TIMEPOINTS_INCLUDED  1

#include <iostream>
#include <vector>
#include <stdexcept>

namespace geometric
{

   class timepoints
   {
      unsigned int maxsize;
         // This is the maximum number of points that we remember. 
         // After that, we start replacing the last point. 
         // Time points should be provided in increasing order. 
         // The default is maxsize == 2, which means that timepoints
         // is just a time interval.
 
      std::vector< unsigned int > moments; 

   public: 
      timepoints( )
         : maxsize{2}
      { }

      explicit timepoints( unsigned int k )
         : maxsize{k}
      { 
         if( maxsize < 2 )
            maxsize = 2; 
      }

      void insert( unsigned int t )
      {
         if( moments. size( ) == 0 ) 
         {
            moments. push_back(t);
            return;
         }

         if( t < moments. back( ))
            throw std::runtime_error( 
               "tried to insert an earlier time moment" );

         if( t == moments. back( ))
            return;

         if( moments. size( ) < maxsize )
            moments. push_back(t);
         else
            moments. back( ) = t; 
      }

      friend std::ostream& operator << ( std::ostream& , 
                                         const timepoints& ); 
   };

   std::ostream& operator << ( std::ostream& , const timepoints& t );
}

#endif

