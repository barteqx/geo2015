
// Written by Hans de Nivelle, June 2014. 

#ifndef STATISTICS_DISTRIBUTION_INCLUDED
#define STATISTICS_DISTRIBUTION_INCLUDED  1

#include <iostream>
#include <vector>
#include <initializer_list>

namespace statistics
{
   
   class distribution 
   {

      // When a number x is entered into the distribution,
      // we look for the biggest i, s.t.
      // distr [i]. lowerbound <= x. 
      // This will be the point where x is counted.
      // When x is lower than our lowest lower bound, we throw a 
      // range_error. 
      // It is the responsibility of the user, through the constructor
      // call, to set the lowerbounds in such a way that the result
      // is the information that he wants to see.

      struct entry
      {
         double lowerbound;
         double sum;
         unsigned int nrelements;

         entry( double lowerbound )
            : lowerbound{lowerbound},
              sum{0.0},
              nrelements{0}
         { }
      };

      std::vector< entry > distr;
         // True if we have been reported without change.
   
public: 
      distribution( std::initializer_list< double > lowerbounds );
      distribution( const std::vector< double > & lowerbounds ); 
         // Make a distribution, based on lowerbounds.

      distribution( double start, double step, double end );
         // Create a distribution with lowerbounds
         // start, start + step, .... , start + i * step,
         // where i is the greatest number, s.t. start + i*step < end.  

      void clear( );
         // Clear all stored values. (Not the lower bounds!)

      void insert( double x );
         // Insert d into the distribution. 
         // Throws a runtime exception if x cannot be counted. 
         // insert uses binary search, so that the time is logarithmic
         // in the number of intervals.

      unsigned int nrelements( ) const;
         // Total number of elements that we have counted. 

      double sum( ) const;
         // Sum of all elements that have been inserted.

      double average( ) const { return sum( ) / nrelements( ); } 

      friend std::ostream& operator << ( std::ostream& , const distribution& );
   };


   std::ostream& operator << ( std::ostream& stream, const distribution& );

}

#endif


