
// Written by Hans de Nivelle, February 2015.
// Part of Geo III.

#ifndef GEOMETRIC_MATCHED_INCLUDED 
#define GEOMETRIC_MATCHED_INCLUDED 1

#include <iostream>

#include "subst.h"
#include "variable.h"
#include "element.h"

namespace geometric
{
   // R can be lemma, disjunctive or existential. 
   // The difference with false_instance is that false_instance
   // uses the rule class, which is fully polymorphic.
   // It also stores statistics that are useful for selecting the  
   // best false instance. 
   // matched<R> knows the concrete class of its rule.
   // Also, matched<R> does not claim that the rule is false, although
   // in many cases it will be of course.

   template< typename R >
   struct matched
   {
      R r;
      subst<element> inst;

      matched( const R& r, const subst<element> & inst )
         : r{r},
           inst{inst}
      { }

      matched( const R& r, subst<element> && inst )
         : r{r},
           inst{ std::move(inst) }
      { }


      // Collect the unassigned variables (probably with the purpose of
      // creating an error message if there are any).

      set<variable> getunassigned( ) const
      {
         set<variable> res; 
         set<variable> var = r. getfreevariables( );
         for( variable v:var )
         {
            auto p = inst.clookup(v);
            if( !p )
               res. insert(v);
         }
         return res;
      }

   };


   template< typename R >
   inline std::ostream& operator << ( std::ostream& out,
                                      const matched<R> & m )
   {
      out << "rule: " << m.r << "\n";
      out << m. inst << "\n";
      return out;
   }
}

#endif
 
