
// Written by Hans de Nivelle, 27.09.2014.

#ifndef GEOMETRIC_PERMUTATIONGENERATOR_INCLUDED
#define GEOMETRIC_PERMUTATIONGENERATOR_INCLUDED  1

#include <stdexcept>
#include <iostream>
#include <random>
#include <chrono>

#include "atom.h"
#include "index/simple.h"
#include "permutation.h" 

namespace geometric
{

   struct permutationgenerator
   {

      enum class strategy
      {
         left2right,      // ( 0, ..., nr-1 ).
         right2left,      // ( nr-1, ..., 0 ).
         random,          // In a PSEUDO-random order. 
         mostfrequent     // Starting with most frequently used element, 
                          // when a geometric::index::simple is present. 
      };

      strategy strat; 
      std::default_random_engine rand; 

      // Seed 0 takes a truly random seed from the system_clock. 
      // Other values of seed result in reproducable pseudo-random sequences.

      permutationgenerator( strategy strat, unsigned int seed = 0 )
         :strat{strat},
          rand{ ( seed == 0 ) ? 
                   std::default_random_engine( 
                      std::chrono::system_clock::now( ). 
                         time_since_epoch( ). count( ))
                  : std::default_random_engine( seed ) } 
      {
         
      } 
   
      permutation makepermutation( unsigned int nr,
          const index::standardsimple< element > * simpl = nullptr );
         // Create a permutation of the numbers { 0,.., nr-1 }.
         // If nr = 0, the empty permutation is generated.
         // If simpl is present, and strat = mostfrequent, then the 
         // permutation will start with the element that occurs
         // most frequently in simpl. The returned permutation
         // is always correct. If strat = mostfrequent, and simpl is absent,
         // then a random permutation is generated.

      static element mostfrequent( 
          const index::standardsimple<element> & simpl );
             // Most frequent element in simpl.

   };

   std::ostream& operator << ( std::ostream& out,
                               const permutationgenerator::strategy& s );

   std::ostream& operator << ( std::ostream& out,
                               const permutationgenerator& gen );
}

#endif

