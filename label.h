
// Code written by Hans de Nivelle, November 2005. 

#ifndef LABEL_INCLUDED
#define LABEL_INCLUDED    1

#include <iostream>

class label
{
   int tag;

public: 
   explicit label( int tag )
      : tag( tag )
   {
   }

   bool operator == ( label other ) const 
   {
      return tag == other. tag;
   }

   bool operator != ( label other ) const 
   {
      return tag != other. tag;
   }

   void print( std::ostream& stream ) const 
   {
      stream << "label(" << tag << ")";
   }

};


inline std::ostream& operator << ( std::ostream& stream, label l )
{
   l. print( stream );
   return stream;
}


#endif


