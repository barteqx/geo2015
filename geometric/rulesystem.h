
// Written by Hans de Nivelle, January 2015.
// Updated with indexing February 2015.

#ifndef GEOMETRIC_RULESYSTEM_INCLUDED
#define GEOMETRIC_RULESYSTEM_INCLUDED   1

#include <string>
#include "rule.h"
#include "timepoints.h" 
#include "subst.h" 
#include "false_instance.h"
#include "index/simple.h"

namespace geometric
{
  
   // Rule with an index on the premisses, and some statistics on how 
   // the rule was used. 
 
   struct indexedrule 
   {
      rule r;
      index::standardsimple< variable > prem;
      set<variable> vars;

      unsigned int nruses;
      timepoints uses;
         // In Geo 2007f, we counted how often a rule was used
         // in model closures, which is a rather useless measure.
         // When a rule closes a model in an essential way, it will
         // also be used in deriving a new lemma.
         // Hence we count how often the rule is used in derivations.

      indexedrule( const rule& r )
         : r{r},
           vars{ r. getfreevariables( ) },
           nruses{0}
      {
         const auto& pass = r. getpassive( );
         for( const varatom& at : pass )
            prem. insert(at);
      }
   };


   std::ostream& operator << ( std::ostream& out, const indexedrule& ind );
    

   struct rulesystem
   {
      std::string name;

      std::vector<indexedrule> initials;     // Rules of all possible types. 
      std::vector<indexedrule> simplifiers;  // Lemmas only. 
      std::vector<indexedrule> learnt;       // Lemmas only. 

      rulesystem( const std::string& name, 
          const std::vector<rule> & initial = std::vector<rule>( ),
          const std::vector<rule> & simplifiers = std::vector<rule>( ));

      rulesystem( const std::string& name,
          std::initializer_list<rule> initials,
          std::initializer_list<rule> simplifiers );

      void sort( );
         // Sort lemmas by length, or by some other complexity measure. 

      void remove( unsigned int s );
         // Remove lemmas with high number.

      void clearuses( );
         // Clear all the uses. 

      void addlearnt( std::list<false_instance> );
         // Add learnt lemmas to learnt, after they have been simplified and
         // checked for subsumption. We need the full false instances, 
         // because some of the simplifications may look at the matchings.
         // The matching should make the lemma false. 


      // Used by subsumption: 
      // A skipped equality:

      struct equality
      {
         variable v1;
         variable v2;

         equality( variable v1, variable v2 )
            : v1{v1}, v2{v2} 
         { }
      };


      // skipped equalities contains equalities that were skipped
      // after the substitution has been applied.
      // If we have F(A,B,C,D), A = B, C = D and
      // F(X,Y,Z,T), then
      // skipped equalities will contain X = Y, Z = T.

      struct skippedequalities 
      {
         std::vector< equality > skipped;

         // The constructor applies subst on skipped, and puts
         // the results in our own skipped.

         skippedequalities(  
                const std::vector< std::pair<variable,variable>> & skipped,
                const subst< variable > & subst )
         { 
            for( auto e : skipped )
            {
               variable v1 = *subst. clookup( e. first );
               variable v2 = *subst. clookup( e. second );
               ( this -> skipped ). push_back( equality( v1, v2 )); 
            } 
         }

         bool isworsethan( const skippedequalities& other ) const
         {
            return skipped. size( ) > other. skipped. size( );
         }

      };


      static bool activecontained( const subst<variable> & subst,
                                   const lemma& lem1, 
                                   const lemma& lem2 );
         // True if either lem1 has no active atom, or its active
         // atom is contained in lem2.


      // Currently, only lemmas are used in subsumption:

      struct subs_collector 
      {
         const rule& from;
         size_t maxsol;    // Maximum number of results. One is reasonable.
         std::list<skippedequalities> & results;
                           // Results are written into here. 
         const rule& into; 

         subs_collector( const rule& from, size_t maxsol, 
                         std::list< skippedequalities > & results,
                         const rule& into )
            : from( from ),
              maxsol( maxsol ),
              results( results ),
              into( into )
         { } 
        
 
         void operator( ) (
                  const subst<variable> & subst,
                  const std::vector<std::pair<variable,variable>> & skipped,
                  bool& stop );
      };
   };


   std::ostream& operator << ( std::ostream& out, rulesystem::equality e );
   std::ostream& operator << ( std::ostream& out, 
                               const rulesystem::skippedequalities & );
   std::ostream& operator << ( std::ostream& out, const rulesystem& rs );

}

#endif


