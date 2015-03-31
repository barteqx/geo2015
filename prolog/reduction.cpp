

// Code written by Ravinder Indarapu and Hans de Nivelle, April 2003


#include "reduction.h"


namespace prolog 
{

   reduction::reduction( unsigned int pos, const grammarrule& rule ): 
      position( pos ), rule( rule )
   {
   }


   std::ostream& operator << ( std::ostream& os, const reduction& red )
   {
      os << "reduction of " << red. rule;  
      os << " on position " << red. position; 
      return os;
   }
    
} 


 
