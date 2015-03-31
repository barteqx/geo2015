
// Code written by Hans de Nivelle, May 2005.
// Modified June 2014.
// Added a hasher and an equality tester in December 2014.

#ifndef GEOMETRIC_VARIABLE_INCLUDED 
#define GEOMETRIC_VARIABLE_INCLUDED  1

#include <iostream>

namespace geometric
{

   class variable
   {
      size_t x;

   private:
      explicit variable( size_t x )
         : x(x) 
      {
      }
 
   public: 
      variable( ) : x(0) { }

      variable operator ++ ( int )
      {
         variable res = *this; 
         ++ x;
         return res;
      }

      variable operator -- ( int )
      {
         variable res = *this;
         -- x;
         return res;
      }

      variable& operator ++ ( )
      {
         ++ x;
         return *this;
      }

      variable& operator -- ( )
      {
         -- x;
         return *this;
      }
  
      size_t getindex( ) const { return x; }

      variable next ( ) const { return variable(x+1); }

      bool operator == ( variable v ) const 
      {
         return x == v.x;
      }

      bool operator != ( variable v ) const 
      {
         return x != v.x;
      }

      bool operator < ( variable v ) const 
      {
         return x < v.x;
      }

      bool operator > ( variable v ) const 
      {
         return x > v.x;
      }

      bool operator <= ( variable v ) const 
      {
         return x <= v.x;
      }

      inline bool operator >= ( variable v ) const 
      {
         return x >= v.x;
      }

      friend std::ostream& operator << ( std::ostream&, variable ); 

      // Needed by unordered_map:

      struct hash
      {
         size_t operator( ) ( variable v ) const { return v.x; }
      };

      struct equal_to
      {
         bool operator( ) ( variable v1, variable v2 ) const
            { return v1.x == v2.x; }
      };

   };

   inline std::ostream& operator << ( std::ostream& stream, variable v )
   {
      stream << "V" << v.x;
      return stream;
   }
}

#endif


