
// Code written by Hans de Nivelle, June 2007.

#include "dependency_system.h"


namespace analysis
{


   void dependency_system::addequationsinheritedintersect( 
                                        rule::prem_iterator p1,
                                        rule::prem_iterator p2, 
                                        varatom concl,
                                        dependencies dep,
                                        const dependency_system& oldsys )
   {
      dep. mergewith( dependencies::readequations( concl )); 
     
      // We are going to compute the dependencies that concl inherits
      // from the the premisses.  
     
      dependencies inherited;  
      bool inherits_universal = false;
         // In case one of the premisses has the universal set 
         // of dependencies ( forall P:Dep   P ), the dependencies of 
         // concl will be universal as well. 

      for( rule::prem_iterator p = p1; p != p2; ++ p ) 
      {
         if( (*p). p != predicate::diseq && (*p). p != predicate::create )
         {
            std::map< predicate, dependencies > :: const_iterator
                          it = oldsys. repr. find( (*p). p );
            if( it != oldsys. repr. end( ))
               inherited. mergewith( it -> second, p1, p ); 
            else
               inherits_universal = true;
         }
      }

      if( ! inherits_universal )
      { 
         inherited. project( p1, p2, concl );
         inherited. transfertoconclusion( p1, p2, concl );

         dep. mergewith( inherited );
         dep. reduce( );

         std::map< predicate, dependencies > :: iterator 
                      it = repr. find( concl. p );

         if( it == repr. end( ))
            repr [ concl. p ] = dep; 
         else
            repr [ concl. p ] = 
                    dependencies::intersection( repr [ concl. p ], dep ); 
      }
   }



   dependency_system 
   dependency_system::one_iteration( const rulesystem& rs,
                                     const dependency_system& oldsys )  
   {
      dependency_system res;

      for( std::vector< rulesystem::rule_in_system > :: const_iterator
              rule = rs. rules. begin( );
              rule != rs. rules. end( );
              ++ rule )
      {
         if( rule -> r. isexistential( ))
         {
            variable exvar = * (rule -> r. getcreate( ). a1 );
               // The existential variable.

            dependencies dep = 
                     dependencies::readskolem( exvar, rule -> r. getbody( ));


            res. addequationsinheritedintersect( 
                         rule -> r. prems_begin( ),
                         rule -> r. prems_end( ),
                         rule -> r. getbody( ), dep, oldsys );
         }
         else
         {
            for( rule::conc_iterator 
                    q = rule -> r. concs_begin( );
                    q != rule -> r. concs_end( );
                    ++ q )
            {
               dependencies dep;
               res. addequationsinheritedintersect( 
                                    rule -> r. prems_begin( ),
                                    rule -> r. prems_end( ),
                                    *q, dep, oldsys );
            }
         }
      }

      return res; 
   }
 


   // The dependencies are computed by creating a sequence of
   // dependency systems, s.t. 
   // D0 |= D1 |= D2 |= D3 .....
   // (implication means implication on each of its members,
   //  which is set of dependencies)  
   // Iteration stops when a fixed point is reached, i.e.
   // D_{i+1} |= D_i.  
   //
   // One potential problem is that this procedure has quadratic complexity,
   // which may be too big for large theories. In that case, a 
   // change driven algorithm would be necessary. 


   dependency_system 
   dependency_system::computedependencies( const rulesystem& rs )
   {
      dependency_system res;

      unsigned int cnt = 1; 
      bool fixpoint = false; 
      while( ! fixpoint ) 
      {
         dependency_system next = one_iteration( rs, res );

#if 0
         std::cout << "=========================\n";
         std::cout << "iteration " << cnt << "\n"; 
         std::cout << next << "\n"; 
#endif

         // We check whether each of the members of next implies
         // the corresponding member in res.

         fixpoint = true;
         for( std::map< predicate, dependencies > :: const_iterator
                 itnext = next. repr. begin( );
                 itnext != next. repr. end( ) && fixpoint; 
                 ++ itnext )
         {
            std::map< predicate, dependencies > :: const_iterator
               itres = res. repr. find( itnext -> first );

            if( itres != res. repr. end( ))
            {
               if( ! itnext -> second. implies( itres -> second ))
                  fixpoint = false;
            }
            else
            {
               // Absence means: Universal dependency.
               // Certainly a non-universal dependency does not imply
               // the universal dependency.

               fixpoint = false;
            }
         } 
             
         res = next; 
         ++ cnt;  
      }
      return res; 
   }
 

   std::list< rule > dependency_system::axioms(
                           std::vector< predicate > & predicates_base,
                           std::vector< variable > & arguments_base ) const
   {
      std::list< rule > res;

      for( std::map< predicate, dependencies > :: const_iterator
              p = repr. begin( );
              p != repr. end( );
              ++ p )
      {
         std::list< rule > res2 = 
                p -> second. axioms( predicates_base, arguments_base,
                                     p -> first );
         for( std::list< rule > :: const_iterator
                 q = res2. begin( );
                 q != res2. end( );
                 ++ q )
         {
            res. push_back(*q);
         }
      }
 
      return res;      
   }

            
   std::ostream& operator << ( std::ostream& stream,
                               const dependency_system& ds )
   {
      stream << "Dependency System:\n\n\n";

      for( std::map< predicate, dependencies > :: const_iterator
              p = ds. repr. begin( );
              p != ds. repr. end( );
              ++ p )
      {
         stream << "Predicate: " << ( p -> first ) << ",";
         stream << "\n"; 
         stream << ( p -> second ) << "\n"; 
      }
      return stream;
   }

}



