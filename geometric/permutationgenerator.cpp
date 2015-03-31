
#include "permutationgenerator.h"
#include <stdexcept>

using gen = geometric::permutationgenerator;

std::ostream& 
geometric::operator << ( std::ostream& out, const gen::strategy& s )
{
   switch(s)
   {
   case permutationgenerator::strategy::left2right:
      out << "left2right"; return out;
   case permutationgenerator::strategy::right2left:
      out << "right2left"; return out;
   case permutationgenerator::strategy::random:
      out << "random"; return out;
   case permutationgenerator::strategy::mostfrequent:
      out << "mostfrequent"; return out;
   }
   throw std::runtime_error( "fall through" );
}

geometric::permutation 
gen::makepermutation( unsigned int nr,
                      const index::standardsimple<element> * simpl )
{
   if( nr < 2 ) return permutation(nr);
      // For 0 and 1, only one permutation is possible.
 
   if( nr > 10000 )
      throw std::runtime_error( "nr is too big" );
     
   switch(strat)
   {
   case strategy::left2right:
      return permutation( 1, 0, nr );
   case strategy::right2left:
      return permutation( nr-1, nr-1, nr );
   case strategy::random:
      return permutation( 10007 % nr, rand( ) % nr, nr );
   case strategy::mostfrequent:
      if( simpl )
      {
         unsigned int v = mostfrequent( *simpl ). getindex( );
         if( v >= nr ) v = nr-1;
         return permutation( nr-1, v, nr );
            // Counts backwards from most frequent. 
      }
      else 
         return permutation( 10007 % nr, rand( ) % nr, nr );
   }
   throw std::runtime_error( "fall through" ); 
}


geometric::element 
geometric::permutationgenerator::mostfrequent( 
                      const index::standardsimple<element> & simpl )
{
   element maxel;
   unsigned int maxfreq = 0;

   element e;
   for( size_t i = 0; i < simpl. size( ); ++ i )
   {
      unsigned int freq = simpl. find(e). size( ); 
      if( freq > maxfreq )
      {
         maxel = e;
         maxfreq = freq;
      }

      ++ e; 
   }
 
   return maxel;
}


std::ostream& 
geometric::operator << ( std::ostream& out,
                        const geometric::permutationgenerator& g )
{
   out << "permutationgenerator( " << g. strat << ", ";
   out << g. rand << " )";
   return out;
}

