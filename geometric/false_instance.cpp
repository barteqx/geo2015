
#include "false_instance.h"

namespace geometric
{

   std::ostream& operator << ( std::ostream& out, const false_instance& f )
   {
      out << "false instance:\n";
      out << f. r << "\n";
      out << "number of choices: " << f. nrchoices << "\n";
      out << f. inst << "\n";
      out << "assumptions: " << f. assumptions << "\n";
      return out;
   }

}

