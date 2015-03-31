
// Written by Hans de Nivelle, June/September 2014.

#include "rulebase.h"
#include "../assert.h"
#include "../tostring.h"
#include <map>

void geometric::rulebase::insert( varatom at )
{
   if( at. p. iscreate( ))
   {
      if( at. t != truthval::F )
         throw std::runtime_error( std::string( "cannot insert " ) +
            tostring( at ) +
            " to lemma, because create must have truth-value F" );
   }

   if( at. p. isequality( ))
   {
      if( at. t != truthval::T )
         throw std::runtime_error( std::string( "cannot insert " ) +
            tostring( at ) +
            " to lemma, because equality must have truth-value T" );
   }

   // We always merge with an existing atom, when possible:

   for( auto p = passive. begin( ); p != passive. end( ); ++ p )
   {
      if( p -> p == at.p && equal_arguments( *p, at )) 
      {
         ( p -> t ) |= at.t; 
         return;
      }
   }

   passive. push_back( at );
}


void geometric::rulebase::printpassive( std::ostream& out ) const 
{
   if( passive. size( ))
   {
      for( auto p = passive. begin( ); p != passive. end( ); ++ p )
      {
         if( p != passive. begin( ))
            out << ", ";
         out << *p;
      }
   }
   else
      out << "TRUE";
}


geometric::normalizer geometric::rulebase::normalize( ) 
{
   normalizer norm;

   label lab = label(0);
   for( auto p = passive. begin( ); p != passive. end( ); ++ p )
      *p = norm. apply( *p, lab );

   return norm;
}
 

namespace geometric 
{ 
   namespace 
   { 

      // Returns number of distinct, unassigned variables:

      unsigned int nr_unassigned( 
                    const std::map< variable, bool > & assigned,
                    const varatom& at )
      {
         unsigned int u = 0;
         for( auto p = at. args_cbegin( ); p != at. args_cend( ); ++ p )
         {
            // We count unassigned, first time occurrences:

            if( assigned. find( *p ) == assigned. end( ))
            {
               bool firsttime = true;
               for( auto q = at. args_cbegin( ); q != p && firsttime; ++ q ) 
               { 
                  if( *p == *q )
                     firsttime = false; 
               }

               if( firsttime )
                  ++ u;
            }
         }
         return u;
      }


      // Preference categories are as follows: 
      // 0. Fully assigned equality or create atoms.
      // 1. Regular atoms.
      // 2. Unassigned create atoms.
      // 3. Unassigned equalities.

      unsigned int category( const std::map< variable, bool > & assigned,
                             const varatom& at )
      {
         if( at.p.isequality( ) && at.t == truthval::T )
         {
            if( nr_unassigned( assigned, at ) == 0 )
               return 0;
            else
               return 3;
         }

         if( at.p.iscreate( ) && at.t == truthval::F )
         {
            if( nr_unassigned( assigned, at ) == 0 )
               return 0;
            else
               return 2;  
         }    

         return 1; // Regular atom.
      }

 
      // If the last variable of at1 is unassigned, 
      // and occurs in at2 at a position that
      // is not last, then at1 should be assigned before at2.
      // The idea behind this rule is that functional chains of form
      // A(X), P(X,Y), P(Y,Z), P(Z,T) will be matched in the right order.

      bool prefer_chain( const std::map< variable, bool > & assigned,
                         const varatom& at1, const varatom& at2 )
      {
         if( at1. getarity( ) == 0 || at2. getarity( ) == 0 )
            return false;

         if( at1.p.iscreate( ) || at1.p.isequality( ) ||
             at2.p.iscreate( ) || at2.p.isequality( ))
         {
            return false;
         }

         variable last = at1[ at1. getarity( ) - 1 ];
         if( assigned. find( last ) != assigned. end( ))
            return false;

         for( size_t i = 0; i < at2. getarity( ) - 1; ++ i )
         {
            if( last == at2[i] )
               return true;
         }
         return false; 
      }
   }
}

void geometric::rulebase::sortpassive( )
{
   ASSERT( passive. size( ) < 10000 );
      // This would screw up our point counting.
      // If it happens, the method needs to be reimplemented.

   std::vector< varatom > matched;
      // This vector will eventually replace passive.

   std::map< variable, bool > assigned;
      // These are the variables that occur in matched.
      // If a naive matching algorithm matches the atoms
      // in sorted, the variables in assigned obtain a value.

   while( passive. size( ))
   {
#if 0 
      std::cout << "assigned:   ";
      for( auto p = assigned. begin( ); p != assigned. end( ); ++ p )
         std::cout << "   " << ( p -> first );
      std::cout << "\n";

      std::cout << "passive:   ";
      for( auto p = passive. begin( ); p != passive. end( ); ++ p )
         std::cout << "   " << *p;
      std::cout << "\n";
#endif

      // A vector of zeroes with the size of passive:

      std::vector< unsigned int > penalties = 
         std::vector<unsigned int> ( passive. size( ), 0 );

      for( size_t i = 0; i < passive. size( ); ++ i )
      {
         penalties[i] += 100 * nr_unassigned( assigned, passive[i] );

         for( size_t j = 0; j < passive. size( ); ++ j )
            if( i != j )
            { 
               unsigned int c1 = category( assigned, passive [i] );
               unsigned int c2 = category( assigned, passive [j] );
               if( c1 < c2 )
                  penalties[j] = 100000000; 
           
               if( c1 == c2 )
               {
                  if( prefer_chain( assigned, passive[i], passive[j] ))
                     penalties[j] += 1;
               }
            }
      }

      size_t best = 0;
      for( size_t i = 1; i < penalties. size( ); ++ i )
      {
         if( penalties[i] < penalties[ best ] )
            best = i;
      }

#if 0
      std::cout << "penalties:   "; 
      for( auto p = penalties. begin( ); p != penalties. end( ); ++ p )
         std::cout << "   " << *p;
      std::cout << "\n";

      std::cout << "best = " << best << "\n";
#endif

      // If best is not an instantiated #_{F} V atom, we will copy it:
 
      if( ! ( passive[ best ].p.iscreate( ) &&
              passive[ best ].t == truthval::F &&
              nr_unassigned( assigned, passive[ best ] ) == 0 ))
      {
         // If we are inserting an equality, we check for uninstatiated
         // variables, and add #_{F} predicates, if necessary:  

         if( passive [ best ].p.isequality( ) &&
             passive [ best ].t == truthval::T )
         {
            variable v0 = passive [ best ][0];
            variable v1 = passive [ best ][1];

            const predicate& cr = passive[ best ].p.getcreatefromsignature( );
               // Get the create from the signature of the equality
               // symbol of passive [ best ].

            if( assigned. find(v0) == assigned. end( ))
               matched. push_back( varatom( cr, truthval::F, {v0} ));

            if( assigned. find(v1) == assigned. end( ) && v0 != v1 )
               matched. push_back( varatom( cr, truthval::F, {v1} )); 
         }

         // Mark variables in best as assigned:
 
         for( auto a = passive[ best ]. args_cbegin( ); 
                   a != passive[ best ]. args_cend( ); 
                   ++ a )
         {
            assigned [ *a ] = true;
         }

         matched. push_back( passive [ best ] );
      }

      // Finally, we erase best from passive:

      passive. erase( passive. begin( ) + best ); 
   }

   passive = std::move( matched );
}

             
bool geometric::rulebase::occursinpassive( variable v ) const
{
   for( auto p = passive. begin( ); p != passive. end( ); ++ p )
   {
      if( p -> contains(v))
         return true;
   }
   return false;
}

auto geometric::rulebase::getfreevariables( ) const -> set<variable>
{
   set<variable> result;
   for( const varatom& at : passive )
   {
      for( variable v : at )
         result. insert(v);
   } 
   return result;
}


