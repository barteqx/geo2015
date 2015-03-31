
// Written by Hans de Nivelle, August 2014.

#ifndef GEOMETRIC_RULEBASE_INCLUDED
#define GEOMETRIC_RULEBASE_INCLUDED  1

#include "atom.h"
#include "unification.h"
#include "set.h" 
#include <vector>

namespace geometric
{
   // rulebase an abstract class from which the concrete rules
   // inherit. We have 
   //    lemma:    A rule with 0 or 1 active conclusions. 
   //    disjunctive: A rule with an unbounded number of conclusions.
   //    existential: A rule with 1 existentially quantified conclusion. 

   class rulebase
   {
   protected:
      std::vector< varatom > passive; 

   public: 
      virtual rulebase* clone( ) const = 0; 
      virtual ~rulebase( ) { } 
 
      virtual void insert( varatom at );
          // The default behaviour is to insert at into passive.
          // Insertion always tries to merge with existing atoms.
          // If we already have P_E( ), and we are inserting
          // P_F( ), then the atoms are merged into P_EF( ). 
          //  
          // A disjunctive rule overrides this. It looks at the form
          // of at. If at is inherently passive, it goes into passive.
          // Otherwise, it goes into active.

      const std::vector<varatom> & getpassive( ) const
         { return passive; } 

      size_t passivesize( ) const { return passive. size( ); }
      const varatom& operator [] ( size_t i ) const 
         { return passive[i]; }
     
      virtual void print( std::ostream& ) const = 0;  
      void printpassive( std::ostream& out ) const;
     
      void sortpassive( );
         // This method sorts the passive atoms with two aims: 
         // The resulting order must be suitable for naive matching 
         // algorithms, and the rule must look good. 
         // In addition, sorting removes redundant #_{F} atoms,
         // and adds #_{F} V atoms for variables V that occur only in
         // equalities.  

      bool occursinpassive( variable v ) const;
         // True if variable v occurs among the passive atoms. 

      void addcreate( const signature& sig, variable v )
      {
         if( !occursinpassive(v)) 
            insert( varatom( sig. create, truthval::F, {v} ));
      }
         // Append #_{F} v  to our passive atoms, in case they contain 
         // no occurrence of v. 

      virtual void makerangerestricted( const signature& sig ) = 0;
         // Make the rule range restricted by inserting suitable
         // #_{F} V predicates. A rule is range restricted if
         // every variable that is free in the conclusion, 
         // also occurs among the passive atoms. 

      virtual normalizer normalize( );
         // Normalize the variables, so that the first variable is
         // V0, the second V1, etc. 
         // Aim of this function is to make rules more readable.
         // and to allow for efficient (indexing based) implementations
         // of substitution. The function returns the resulting normalizer.

      virtual set<variable> getfreevariables( ) const;
         // The free variables of the rule. 
   };


   inline std::ostream& operator << ( std::ostream& out, const rulebase& b )
   {
      b. print( out );
      return out;
   }

}

#endif

