
#ifndef GEOMETRIC_EXAMPLES_INCLUDED
#define GEOMETRIC_EXAMPLES_INCLUDED  1

#include "rulesystem.h"

namespace geometric
{
   rulesystem simple( signature& sig );
      // The first simple test.

   rulesystem reflexivity( signature& sig ); 
      // A = B, B = C implies s(A) = s(C).

   rulesystem blz202_5( signature& sig );
   rulesystem demod( signature& sig ); 
   rulesystem substitution( signature& sig ); 
      // Example is called 'subst', but this name exists already. 
}

#endif 

