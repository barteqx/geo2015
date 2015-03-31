
// A DFAstate is a state of the DFA that implements the tokenizer.
// Code was written by Hans de Nivelle in April 2003.


#ifndef PROLOG_DFASTATE_DECLARED
#define PROLOG_DFASTATE_DECLARED   1


#include <iostream>

#include "../assert.h"


namespace prolog 
{
   

   class DFAstate
   {
   public: 
      int state;


      const static int initial =       1; 

      const static int lpar0 =         2; 
      const static int rpar0 =         3; 
      const static int comma0 =        4; 
      const static int rsqpar0 =       5; 
      const static int whitespace0 =   6; 
      const static int eof0 =          7; 
         // The eof0 token is not used.

      const static int percent0 =      8; 
      const static int percent1 =      9; 

      const static int quoted0 =      10; 
      const static int quoted1 =      11; 
      const static int quoted2 =      12; 
      const static int quoted3 =      13;
      const static int quoted4 =      14;
      const static int quoted5 =      15;

      const static int nil0 =         16; 
      const static int nil1 =         17; 

      const static int div0 =         18; 
      const static int div1 =         19; 
      const static int div2 =         20; 
      const static int div3 =         21; 

      const static int name0 =        22; 
      const static int operator0 =    23; 

      const static int blocked =      -1; 
      const static int unknown =      -2;
         // Actually, blocked and unknown are not DFAstates. 
         // If one would do things correctly, one would have the following:
         // The transition function delta returns either a DFAstate,
         // or blocked, (because it is a partial function).
	 //
         // In DFAdelta the values of delta are cached into a table.
         // Here, a table entry contains either a possible return value of 
         // delta, 
         // or unknown, which indicates that no value has been stored
         // yet. 
         // So, if everything would be correct, one would have a total
         // of 3 types, the DFAstate, the delta-return value, and the
         // table entry. 

      const static int maxstate =    25;
         // Used for defining an array that can be indexed by states.
         // The values blocked and unknown should not be used as index,
         // but they can be used as value.

      DFAstate( int x ) : state( x ) 
      {
      }

      DFAstate( ) : state( unknown )
      {
      }

      bool operator == ( DFAstate other ) 
      { 
         return state == other. state;
      }

     
      bool operator != ( DFAstate other )
      {
         return state != other. state;
      }

   }; 

   std::ostream& operator << ( std::ostream&, DFAstate x ); 

}


#endif


