

// Code written by Hans de Nivelle, May 2005.


#ifndef VARIABLE_INCLUDED 
#define VARIABLE_INCLUDED  1


#include <iostream>


#include "assert.h"


class variable
{
   unsigned int x;

private:
   explicit variable( unsigned int x )
      : x(x) 
   {
   }
 
public: 
   variable( ) : x(0) { }

   variable operator ++ ( int );
   variable& operator ++ ( );
   variable operator -- ( int );
   variable& operator -- ( ); 
  
   unsigned int getindex( ) const; 
   static variable constructfromunsigned( unsigned int i );

   friend bool operator == ( variable, variable );
   friend bool operator != ( variable, variable );
   friend bool operator < ( variable, variable );
   friend bool operator > ( variable, variable );
   friend bool operator <= ( variable, variable );
   friend bool operator >= ( variable, variable );

   friend std::ostream& operator << ( std::ostream&, variable ); 

   struct hash
   {
      unsigned int operator( ) ( variable v ) const { return v.x; }
   };

   struct equal_to
   {
      bool operator( ) ( variable v1, variable v2 ) const
         { return v1.x == v2.x; }
   };

};


inline variable variable::operator ++ ( int ) 
{
   variable res = *this;
   x ++ ;
   return res;
}

inline variable variable::operator -- ( int )
{
   ASSERT( x != 0 );
   variable res = *this;
   x -- ;
   return res;
}

inline variable& variable::operator ++ ( ) 
{
   x ++ ;
   return *this;
}

inline variable& variable::operator -- ( )
{
   ASSERT( x != 0 );
   x -- ;
   return *this;
}


inline unsigned int variable::getindex( ) const
{
   return x;
}

inline variable variable::constructfromunsigned( unsigned int i )
{
   return variable(i);
}


inline bool operator == ( variable v1, variable v2 )
{
   return ( v1. x == v2. x );
}

inline bool operator != ( variable v1, variable v2 )
{
   return ( v1. x != v2. x );
}

inline bool operator < ( variable v1, variable v2 )
{
   return ( v1. x < v2. x );
}

inline bool operator > ( variable v1, variable v2 )
{
   return ( v1. x > v2. x );
}

inline bool operator <= ( variable v1, variable v2 )
{
   return ( v1. x <= v2. x );
}

inline bool operator >= ( variable v1, variable v2 )
{
   return ( v1. x >= v2. x );
}



inline std::ostream& operator << ( std::ostream& stream, variable v )
{
   stream << "V" << v. x;
   return stream;
}


#endif


