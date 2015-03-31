
#include "age.h"

std::ostream& 
geometric::operator << ( std::ostream& out, const geometric::age& a )
{
   unsigned int s = 
      ( std::chrono::duration_cast< std::chrono::seconds >
                 ( age::clock::now( ) - a.t )). count( );

   unsigned int h = s / 3600;
   s -= h * 3600;
   unsigned int m = s / 60;
   s -= m * 60;

   if(h)
      out << h << 'h' << m << 'm' << s << 's';
   else
   {
      if(m)
         out << m << 'm' << s << 's';
      else
         out << s << 's';
   }

   out << " old";
   return out; 
}

