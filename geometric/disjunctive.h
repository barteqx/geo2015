
// Written by Hans de Nivelle, August 2014.

#ifndef GEOMETRIC_DISJUNCTIVE_INCLUDED
#define GEOMETRIC_DISJUNCTIVE_INCLUDED  1

#include "rulebase.h"

namespace geometric
{

   class disjunctive : public rulebase 
   {
      std::vector< varatom > active;
         // Contains atoms of form p_{E} or p_{T}. 

   public:          
      disjunctive( std::initializer_list< varatom > at );
         // The constructor decides by itself where the atoms in
         // at go. Atoms with truth value F, FE, FT, as well as 
         // equalities and domain predicates, go into passive. 
         // Simple predicates with truth value E or T go into active.
         // (All the decisions are made by insert, which we override) 
         // The constructor tries to merge atoms as much as possible. 

      void insert( varatom at ) override; 
         // Look at the form of at, and decides if it should be inserted
         // into passive, or active. Try to merge with existing
         // atoms as much as possible. 

      size_t nractive( ) const { return active. size( ); }
      const varatom& operator [] ( size_t j ) const
         { return active[j]; } 
      const std::vector< varatom > & getactive( ) const 
         { return active; } 

      void makerangerestricted( const signature& sig ) override; 
      normalizer normalize( ) override;

      void print( std::ostream& out ) const override; 
      disjunctive* clone( ) const override 
         { return new disjunctive( *this ); }

      set<variable> getfreevariables( ) const override;

   };

}

#endif

