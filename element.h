

// Code written by Hans de Nivelle, May 2005.
//
// An element is an element of a model.


#ifndef ELEMENT_INCLUDED 
#define ELEMENT_INCLUDED  1


#include <iostream>


#include "assert.h"


class element
{
   unsigned int x;

private:
   explicit element( unsigned int x )
      : x{x} 
   {
   }
 
public: 
   element( ) : x(0) { }

   element operator ++ ( int );
   element& operator ++ ( );
   element operator -- ( int );
   element& operator -- ( ); 
  
   unsigned int getindex( ) const; 
   static element constructfromunsigned( unsigned int i );

   friend bool operator == ( element, element );
   friend bool operator != ( element, element );
   friend bool operator < ( element, element );
   friend bool operator > ( element, element );
   friend bool operator <= ( element, element );
   friend bool operator >= ( element, element );

   friend std::ostream& operator << ( std::ostream&, element ); 

   struct hash
   {
      size_t operator( ) ( element e ) const { return e.x; }
   };

   struct equal_to
   {
      bool operator( ) ( element e1, element e2 ) const
         { return e1.x == e2.x; }
   };

};


inline element element::operator ++ ( int ) 
{
   element res = *this;
   x ++ ;
   return res;
}

inline element element::operator -- ( int )
{
   ASSERT( x != 0 );
   element res = *this;
   x -- ;
   return res;
}

inline element& element::operator ++ ( ) 
{
   x ++ ;
   return *this;
}

inline element& element::operator -- ( )
{
   ASSERT( x != 0 );
   x -- ;
   return *this;
}


inline unsigned int element::getindex( ) const
{
   return x;
}

inline element element::constructfromunsigned( unsigned int i )
{
   return element(i);
}


inline bool operator == ( element e1, element e2 )
{
   return ( e1. x == e2. x );
}

inline bool operator != ( element e1, element e2 )
{
   return ( e1. x != e2. x );
}

inline bool operator < ( element e1, element e2 )
{
   return ( e1. x < e2. x );
}

inline bool operator > ( element e1, element e2 )
{
   return ( e1. x > e2. x );
}

inline bool operator <= ( element e1, element e2 )
{
   return ( e1. x <= e2. x );
}

inline bool operator >= ( element e1, element e2 )
{
   return ( e1. x >= e2. x );
}



inline std::ostream& operator << ( std::ostream& stream, element e )
{
   stream << "E" << e.x;
   return stream;
}


#endif


