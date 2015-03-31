
// Code written by Hans de Nivelle, May 2005.
// Modified June 2014. 
// Added a hasher and an equality tester in December 2014.

#ifndef GEOMETRIC_ELEMENT_INCLUDED 
#define GEOMETRIC_ELEMENT_INCLUDED  1

#include <iostream>

namespace geometric
{

   class element
   {
      size_t x;

   private:
      explicit element( size_t x )
         : x(x) 
      {
      }
 
   public: 
      element( ) : x(0) { }

      element operator ++ ( int )
      {
         element res = *this; 
         ++ x;
         return res;
      }

      element operator -- ( int )
      {
         element res = *this;
         -- x;
         return res;
      }

      element& operator ++ ( )
      {
         ++ x;
         return *this;
      }

      element& operator -- ( )
      {
         -- x;
         return *this;
      }
  
      size_t getindex( ) const { return x; }

      element next ( ) const { return element(x+1); }

      bool operator == ( element e ) const 
      {
         return x == e.x;
      }

      bool operator != ( element e ) const 
      {
         return x != e.x;
      }

      bool operator < ( element e ) const 
      {
         return x < e.x;
      }

      bool operator > ( element e ) const 
      {
         return x > e.x;
      }

      bool operator <= ( element e ) const 
      {
         return x <= e.x;
      }

      inline bool operator >= ( element e ) const 
      {
         return x >= e.x;
      }

      friend std::ostream& operator << ( std::ostream&, element ); 

      // Needed by unordered_map:

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


   inline std::ostream& operator << ( std::ostream& stream, element e )
   {
      stream << "E" << e.x;
      return stream;
   }
}

#endif


