
// Code written by Hans de Nivelle, April 2006.


#include "errors.h"


namespace parser
{


   std::ostream& operator << ( std::ostream& stream, const syntaxerror& err )
   {
      stream << "Syntax error: " << err. problem;
      stream << " in line " << err. linenumber;
      stream << " of inputstream " << err. streamname;  

      return stream;
   }
            


   std::ostream& operator << ( std::ostream& stream, const readerror& err )
   {
      stream << "Read error: " << err. problem;
      stream << " in line " << err. linenumber;
      stream << " of inputstream " << err. streamname;

      return stream;
   }
   

} 


