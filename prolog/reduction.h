

// Code written by Ravinder Indarapu and Hans de Nivelle, 2003


#ifndef PROLOG_REDUCTION_INCLUDED
#define PROLOG_REDUCTION_INCLUDED   1


#include <iostream>

#include "grammarrule.h"


namespace prolog 
{

   struct reduction
   {
     
      unsigned int position;
      grammarrule rule;
     
      reduction( unsigned int pos, const grammarrule& rule );
     
   };

   std::ostream& operator << ( std::ostream& os, const reduction& );

} 
 
#endif 
 

 
