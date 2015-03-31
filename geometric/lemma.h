
// Written by Hans de Nivelle, August 2014.

#ifndef GEOMETRIC_LEMMA_INCLUDED
#define GEOMETRIC_LEMMA_INCLUDED  1

#include "atom.h"
#include "rulebase.h"
#include <list>

namespace geometric
{

   // A lemma can have an unbounded number of premisses, but at most 
   // one conclusion. 
   // If it has a conclusion, then it can be used in forward
   // reasoning. Otherwise, it can be used as conflict lemma. 
   // The conclusion must be not inherently passive.
 
   class lemma : public rulebase 
   {
      std::list< varatom > active;  // At most one. 

   public:          
      lemma( std::initializer_list< varatom > passive ); 
      lemma( std::initializer_list< varatom > passive, const varatom& active );

      bool hasactive( ) const { return active. size( ); } 
      const varatom& getactive( ) const { return active. front( ); } 

      void makerangerestricted( const signature& sig ) override; 
      normalizer normalize( ) override; 

      void print( std::ostream& out ) const override;
      lemma* clone( ) const override { return new lemma( *this ); } 

      set<variable> getfreevariables( ) const override; 
         
   };

}

#endif


