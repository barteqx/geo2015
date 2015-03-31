
// Written by Hans de Nivelle, August 2014.

#ifndef GEOMETRIC_EXISTENTIAL_INCLUDED
#define GEOMETRIC_EXISTENTIAL_INCLUDED  1

#include "rulebase.h"

namespace geometric
{

   // An existential rule has form 
   // A_1, ..., A_p | EXISTS y B, where the A_i are inherently passive, and 
   // B is not inherently passive. Variable y must not occur in A_1,...,A_p,
   // but must occur in B. 

   class existential : public rulebase 
   {

      variable v;     // The quantified variable. 
      varatom at;     // The quantified atom. 

   public:          
      existential( std::initializer_list< varatom > passive, 
                   variable v, const varatom& at );

      variable getvariable( ) const { return v; }
      const varatom& getatom( ) const { return at; } 

      void makerangerestricted( const signature& sig ) override;
      normalizer normalize( ) override;

      void print( std::ostream& out ) const override; 
      existential* clone( ) const override 
         { return new existential( *this ); } 

      set<variable> getfreevariables( ) const override; 
   };

}

#endif


