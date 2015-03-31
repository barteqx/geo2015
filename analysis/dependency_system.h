
// Code written by Hans de Nivelle, June 2007.


#ifndef ANALYSIS_DEPENDENCY_SYSTEM_INCLUDED
#define ANALYSIS_DEPENDENCY_SYSTEM_INCLUDED   1


#include <map>

#include "../rulesystem.h"
#include "dependencies.h"


namespace analysis
{

   class dependency_system
   {

      std::map< predicate, dependencies > repr;
  
         // For each predicate, dep stores the dependencies among 
         // its arguments. 
         // When a predicate does not occur, this means that
         // it satisfies all dependencies. In other words it satisfies 
         // the universal constraint forall C ( C ). 

   private:
      void addequationsinheritedintersect( 
                            rule::prem_iterator p1,
                            rule::prem_iterator p2,
                            varatom concl,
                            dependencies dep,
                            const dependency_system& oldsys );
         // A messy function. It first merges dep with the union of the 
         // equality dependencies for concl, next with the dependencies  
         // that concl inherits from the literals in p1 ... p2 using oldsys.
         // After that it computes the intersection of the result with
         // what it already knows has for the atom of concl. 
      

   public:        
      static dependency_system one_iteration( 
                                        const rulesystem& rs,
                                        const dependency_system& oldsys );
         // Creates the dependency system that the rhs inherits 
         // from the lhs. 

      
      static dependency_system computedependencies( const rulesystem& rs );
         // Compute all dependencies. This is done by calling
         // one_iteration, until there are no changes.


         // An important example, which at this moment we cannot handle,
         // is # V0 -> # V1 /\ A(V0,V1), 
         // A(V0,V1) /\ A(V1,V2) -> B(V0,V2).
         // Predicate B is functional, but do not get that. 
         // If this turns out important, a reimplementation may be 
         // necessary. 

      std::list< rule > axioms( 
                           std::vector< predicate > & predicates_base,
                           std::vector< variable > & arguments_base ) const;


      friend std::ostream& operator << ( std::ostream& , 
                                         const dependency_system& );

   };

   std::ostream& operator << ( std::ostream& ,
                               const dependency_system& );

}


#endif


