
// Written by Hans de Nivelle, January 2015.

#ifndef GEOMETRIC_FALSE_INSTANCE_INCLUDED
#define GEOMETRIC_FALSE_INSTANCE_INCLUDED  1

#include "rule.h" 
#include "subststack.h"
#include "set.h"

namespace geometric 
{ 

   // A false instance contains a rule, the number of choices,
   // an instantiating substitution, and a set of assumptions. 

   struct false_instance
   {
      rule r; 
      unsigned int nrchoices; 
         // Do not assume that nrchoices == rule.nractive( ), because
         // some active atoms may be inconsistent!

      subst< element > inst; 
      set< size_t > assumptions;     // Assumptions that we depend on.

      false_instance( const rule& r,
                      unsigned int nrchoices, 
                      const subst< element > & inst, 
                      const set< size_t > & assumptions )
         : r{r},
           nrchoices{ nrchoices }, 
           inst{ inst },      
           assumptions{ assumptions }
      { 
      }

         
      bool isworsethan( const false_instance& other ) const
      {
         if( nrchoices > other. nrchoices ) 
            return true;
         if( nrchoices < other. nrchoices ) 
            return false;

         int c = multiset_compare( assumptions, other. assumptions );
         if( c == 1 )
            return true;
         if( c == -1 )
            return false;

         size_t p1 = r.getpassive( ). size( );
         size_t p2 = other.r.getpassive( ). size( );

         if( p1 > p2 ) 
            return true; 

         return false; 
      } 

   };

   std::ostream& operator << ( std::ostream& out, const false_instance& f );

}

#endif

 
