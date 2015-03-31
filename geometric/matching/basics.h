
// Written by Hans de Nivelle, Dec. 2014.

#ifndef GEOMETRIC_MATCHING_BASICS_INCLUDED 
#define GEOMETRIC_MATCHING_BASICS_INCLUDED  1

#include "../subststack.h"
#include "../index/simple.h" 


namespace geometric
{
   namespace matching
   {

      // Add the range, corresponding to argument a occurring at position i
      // of an atom that has predicate symbol p.

      template< typename A, typename H, typename E, size_t S >
      inline void 
      addrange( const predicate& p, size_t i, 
                const A& a, const index::simple<A,H,E,S> & ind,
                std::vector< range< size_t >> & res )
      {
         // std::cout << "addrange " << p << ", " << i << ", " << a << " ==> ";

         // We look up the triple (p,i,a) in the index.
         // Arguments of equality must always be looked up with i == 0,
         // because of symmetry.

         range<size_t> r = p.isequality( ) ?  
                                ind.find( p, 0, a ) :
                                ind.find( p, i, a );

               // There are many options here. We can keep all ranges,
               // or keep only the shortest. Currently, we keep only
               // the shortest. One could also keep the two shortest, or
               // whatever.

         // std::cout << "r = " << r << "\n";

         if( res. size( ) == 0 )
            res. push_back(r);
         else
         {
            if( r. size( ) < res. back( ). size( ))
               res. back( ) = r;
         }
      }


      // Returns a vector of ranges that limits the occurrences of
      // atom[ instantiated by subst ], in ind. 

      template< typename A, typename H, typename E, size_t S >
      inline 
      std::vector< range< size_t >> getranges( 
                      const varatom& atom,
                      const subst<A> & subst,
                      const index::simple<A,H,E,S> & ind )
      {
         std::vector< range< size_t >> res;

         for( size_t i = 0; i != atom. getarity( ); ++ i )
         {
            auto val = subst. clookup( atom[i] );
            if( val )
            {
               // If val has content, we look up the triple (p,i,*val):

               addrange( atom.p, i, *val, ind, res );
            }
         }

         // If there was no assigned variable, we simply take all
         // occurrences of the predicate symbol. 

         if( res. size( ) == 0 )
            res. push_back( ind. find( atom.p ));

         return res; 
      }


      // Returns a vector of ranges that limits the occurrences of
      // atom[ instantiated by subst + { v:= a } ] in ind.


      template< typename A, typename H, typename E, size_t S >
      inline 
      std::vector< range< size_t >> getranges( 
                      const varatom& atom,
                      const subst<A> & subst,
                      variable v, const A& a, 
                      const index::simple<A,H,E,S> & ind )
      {
         std::vector< range< size_t >> res;

         for( size_t i = 0; i != atom. getarity( ); ++ i )
         { 
            if( atom[i] == v )
               addrange( atom.p, i, a, ind, res ); 
            else
            { 
               auto val = subst. clookup( atom[i] );
               if( val )
               {
                  addrange( atom.p, i, *val, ind, res );
               }
            }
         }

         // If there was no assigned variable, we take all
         // occurrences of the predicate symbol: 

         if( res. size( ) == 0 )
            res. push_back( ind. find( atom.p ));

         return res; 
      }


      // Naively produce all matchings of the premises of r into
      // interpretation, and call f( matching, subst.s ) with each
      // found solution. We use != for equality testing on F.

      template< typename F > void  
      conflictmatch( const std::vector<varatom> & prem, 
                     size_t level, 
                     const index::standardsimple< element > & interpretation,
                     subststack<element> & subst, 
                     F& f ) 
      {
         if( level == prem. size( ))
         {
            // We have a solution: 

            f( subst.s );  // Other parameters can be passed
                         // by capture in f. 
            return; 
         }

         const varatom& atom = prem[ level ];

         // Equality: The variables must
         // be assigned (this is a precondition) and distinct.

         if( atom. p. isequality( ))
         {
            auto p1 = subst. clookup( atom[0] );
            auto p2 = subst. clookup( atom[1] );
            if( !p1 || !p2 )
               throw std::logic_error( "equality has unassigned variables" );

            if( *p1 != *p2 )
            {     
               conflictmatch( prem, level+1, interpretation, subst, f ); 
            }
         }
         else
         {
            std::vector< range< size_t >> occ = 
               matching::getranges( atom, subst.s, interpretation );

            // We enumerate common elements, and try to match:

            size_t s = subst. size( );   // For restoring.

            bool b = findcommon( occ );
            while(b)
            {
               size_t k = * ( occ[0] ++ );
           
               const groundatom& into = interpretation[k];

               if( ( atom.t & into.t ) == truthval::empty && 
                   subst. match_arguments( atom, into ))
               {
                  conflictmatch( prem, level+1, interpretation, subst, f ); 
               }

               // Restore subst. This must always be done, also when
               // matching failed.

               subst. restore(s);
               b = findcommon( occ );
            }
         }
      }


      // Equalities U==V are either matched into equalities 
      // or skipped, in which case {U,V} is appended to skipped.
      // At most maxskip equalities can be skipped. Setting maxskip = 0,
      // ensures that no equality will be skipped.
      // Creates are always skipped, but the arguments are put in
      // creates, so that they can be dealt with later.
      // Often, they can be ignored. 
      // Function f will be called as f( subst, skipped, stop ).
      // Function f can assign stop = true if the current solution is
      // so good, that there is no need to see more.

      template< typename F > 
      inline void 
      subsumptionmatch( const std::vector<varatom> & prem,
                        size_t level,
                        const index::standardsimple<variable> & index,
                        const set<variable> & variables, 
                        subststack<variable> & subst,
                        std::vector<std::pair<variable,variable>> & skipped, 
                        size_t maxskip, 
                        bool& stop, 
                        F& f )
      {
#if 0
         std::cout << "Entering subsumption match at level " << level << "\n";
         std::cout << subst << "\n";

         std::cout << "skipped: ";
         for( auto p = skipped. begin( ); p != skipped. end( ); ++ p ) 
         {
            std::cout << "(" << ( p -> first ) << ", ";
            std::cout << ( p -> second ) << ") ";
         }
         std::cout << "\n";
#endif
 
         if( level == prem. size( ))
         {
            f( subst.s, skipped, stop ); 
            return; 
         }

         const varatom& atom = prem [ level ];

         if( atom.p.iscreate( ) && atom.t == truthval::F )
         {
            // We inefficiently enumerate the variables in
            // index.

            if( subst. lookup( atom[0] ))
               throw std::runtime_error( "subsumptionmatch: Argument of "\
                                         "create already has assignment" );

            size_t s = subst. size( ); 
            for( variable v : variables ) 
            {
               subst. assign( atom[0], v );    
               subsumptionmatch( prem, level+1, index, variables, 
                                 subst, skipped, maxskip, stop, f );
               subst. restore(s); 
               if( stop ) return; 
            }
            return; 
         }

         if( atom.p. isequality( ) && atom.t == truthval::T )
         {
            // We know that equality is fully assigned.

            const variable* v1 = subst. lookup( atom[0] );
            const variable* v2 = subst. lookup( atom[1] );
            if( !v1 || !v2 )
            {
               throw std::runtime_error( "subsumptionmatch: Argument of "\
                                         "equality has no assignment" );
            }

            if( *v1 == *v2 )  
               return; 
                  // Because the equality became a tautology. 

            std::vector< range< size_t >> 
               occ = matching::getranges( atom, subst.s, index );
            
            bool b = findcommon( occ );
            while(b)
            {
               size_t k = * ( occ[0] ++ );
               const varatom& into = index[k];

               if( into.t == truthval::T )
               {
                  if( subst. match( atom[0], into[0] ) &&
                      subst. match( atom[1], into[1] ))
                  {
                     subsumptionmatch( prem, level+1, index,
                                       variables, subst, skipped, 
                                       maxskip, stop, f );
                     return;
                  }

                  if( subst. match( atom[0], into[1] ) &&
                      subst. match( atom[1], into[0] ))
                  {
                     subsumptionmatch( prem, level+1, index,
                                       variables, subst, skipped, 
                                       maxskip, stop, f );
                     return; 
                  }
               }
               b = findcommon( occ );
            }

            // If we made it up to here, then we could not match
            // the equality. If we have room left, we skip the equality.

            if( skipped. size( ) < maxskip )
            {
               skipped. push_back( { atom[0], atom[1] } );
               subsumptionmatch( prem, level+1, index,
                                 variables, subst, skipped,
                                 maxskip, stop, f );
               skipped. pop_back( );
            }

            return; 
         }

         // Usual atom receives usual treatment:  

         std::vector< range< size_t >>
            occ = matching::getranges( atom, subst.s, index );

         size_t s = subst. size( );
         bool b = findcommon( occ );
         while(b)  
         {
            size_t k = * ( occ[0] ++ ); 
            const varatom& into = index[k]; 

            if( ( atom.t & into.t ) == atom.t &&
                  subst. match_arguments( atom, into ))
            { 
               subsumptionmatch( prem, level+1, index, variables, 
                                 subst, skipped, maxskip, stop, f ); 
            }

            // Restore subst. This must always be done, also when
            // matching failed.

            subst. restore(s);
            if( stop ) return; 
            b = findcommon( occ );
         }
      }
   }
}

#endif
