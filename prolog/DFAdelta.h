
// Code written by Hans de Nivelle, April 2003.
// Modified for Bliksem in September 2006.


#ifndef PROLOG_DFADELTA_INCLUDED
#define PROLOG_DFADELTA_INCLUDED    1


#include "DFAstate.h"


namespace prolog 
{

   namespace DFAdelta 
   {
      const int maxcharacter = 130;

      DFAstate transition_uncached( DFAstate s, char c );
         // Returns delta(s,c). delta(s,c) is a partial function. When no
         // value is defined, then DFAstate::blocked is returned.


      DFAstate transition( DFAstate s, char c );
         // Returns delta(s,c). It always returns the same value as 
         // transition_uncached( ) defined above, but it caches the values
         // in a table. 


      void printtable( DFAstate table[ DFAstate::maxstate ] [ maxcharacter ] );
   } 

}  


#endif


