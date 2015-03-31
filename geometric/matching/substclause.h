
// Written by Hans de Nivelle, September 2014.

// UNFINISHED !!

#ifndef GEOMETRIC_MATCHING_INCLUDED
#define GEOMETRIC_MATCHING_INCLUDED  1

#include "../atom.h"

// A substclause has form
// ( V_1, ..., V_n ) := ( D_11, ..., D_1n ) | ... | ( D_m1, ..., D_mn ). 
// 
// Substclauses are typically obtained by trying to match
// an atom into an interpretation. 
// For example, matching p_{F}(X,Y) into the interpretation
// { p_{T}(a,b), P_{E}(b,c), Q_{T}(c,d) } results in
// (X,Y) := (a,b) | (b,c).
// Matching p_{F}(X,X) into { P_{T}(a,b), P_{E}(a,a) } results
// in (X) := (a). 
//
// In the paper, a substclause with m = 1 (exactly one choice) is
// called a substlet, but there seems to be no point in having
// a separate class for this.
//
// All matching algorithms operate on substclauses.

namespace geometric 
{
   namespace matching 
   {

      template< typename D > 
      class substclause
      {
         std::vector< variable > domain;         // Size n.
         std::vector< unsigned int > blockings;  // Size m. 
         unsigned int unblocked;                 // In [ 0, .. n ). 
         std::vector<D> ranges;                  // Size m*n.

         // blocking[i] == 0 means that choice i is not blocked.
         // blocking[i] > 0 means that choice i is blocked at some level.
         // Methods that can block (these are checking and choosing methods)
         // are given a level on which they can block choices.
         // Blocking has to be organized in this way, because 
         // a single method may block many choices in a single call, 
         // and blockings must be restorable. 
         // There seems to be no feasible way of passing an unbounded
         // amount of blockings to the caller. 
 
      public: 
         substclause( const varatom& at );
            // Constructs substclause with the variables of at and 0 choices.

         unsigned int nrvariables( ) const { return domain. size( ); } 
         unsigned int nrchoices( ) const { return unblocked; }
            // Number of not blocked choices.

         unsigned int match( const varatom& at, const base<D> & b );
            // Add all possible matchings of at into b. Return the
            // number added. I CANNOT COMPLETE THIS FUNCTION BEFORE
            // I HAVE DECIDED ABOUT THE INTERFACE.

         bool match( const varatom& at, const base<D> & b, 
                     const std::vector< substclause* > & others );  
            // Add all possible matchings of at into b that
            // are consistent with others. Return the number added.

         void commit( );
            // Irreversibly remove the blocked choices.

         void restore( unsigned int l );
            // Restore all choices that have blocking level >= l.
          
         unsigned int 
         check_against( const std::vector< substclause* > & others );

         void print( std::ostream& out, unsigned int level ) const;
            // Print all choices with a blocking level < level.
         void print( std::ostream& out ) const { print( out, 1 ); }
            // Print non-blocked choices.      
      }; 

      template< typename D > 
      substclause<D> :: substclause( const varatom& at )
         : unblocked{0}
      {
         for( auto p = at. args_cbegin( ); p != at. args_cend( ); ++ p )
         {
            // Reoccurring variables are added only once:

            for( auto q = at. args_cbegin( ); q != p; ++ q )
            {
               if( *p == *q ) goto dontadd;
            }
            domain. push_back( *p );
         dontadd:
            ;
         }
      }


      template< typename D >
      void 
      substclause<D>::print( std::ostream& out, unsigned int level ) const
      {
         out << '(';
         for( auto p = domain. begin( ); p != domain. end( ); ++ p )
         {
            if( p != domain. begin( ))
               out << ',';
            out << *p;
        }
        out << ") := ";

        bool first = true;
        for( unsigned int i = 0; i < blockings. size( ); ++ i )
        {
           if( blockings[i] < level )
           {
              if( !first )
                 out << " | ";
              first = false; 
 
              out << '('; 

              auto p1 = ranges. cbegin( ) + ( i * domain. size( ));
              auto p2 = p1 + domain. size( ); 

              for( auto p = p1; p != p2; ++ p ) 
              {
                 if( p != p1 )
                    out << ',';
                 out << *p;
              }
              out << ')';  
           }                    
        }

        if( first )
           out << "FALSE";
     }   


     template< typename D >
     inline std::ostream& 
     operator << ( std::ostream& out, const substclause<D> & cl )
     {
        cl. print( out );
        return out;
     }
     
   }
}

#endif

