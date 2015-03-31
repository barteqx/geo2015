
#ifndef GEOMETRIC_MATCHING_RANGE_RESTRICTION_INCLUDED
#define GEOMETRIC_MATCHING_RANGE_RESTRICTION_INCLUDED   1

// Using naive matching, it is easy to enumerate all existing matchings
// and pick the best. Unfortunately, naive matching is too inefficient
// and the only succesfull matching algorithms that we have, are unable
// to find an optimal matching. They can only find a single matching.
// Evidence suggests that for lemma learning, the actual matching used is 
// very important, so we need a way of extracting better matchings.
// 
// The idea is to use range restrictions: If the algorithm finds a matching,
// we run it again with a restriction on the range that forces it to find 
// a better matching if one exists. We continue doing this until it is unable 
// to find a matching with a better range. 
//
// Written by Hans de Nivelle, September 2014.

namespace geometric
{
   namespace matching
   {

      // D must be a small type which can be put in a set<D> .

      template< typename D > 
      struct range_restriction
      {
         virtual bool allows( D d ) const = 0;
            // True if d can be used.

         bool operator( ) ( D d ) const { return allows(d); }

         virtual void print( std::ostream& out ) const = 0;
      };

      template< typename D > 
      inline 
      std::ostream& operator << ( std::ostream& out, 
                                  const range_restriction<D> & c )
      {
         c. print( out );
         return out;
      }


      // A weak restriction is a restriction that allows almost everything.
      // The default object allows everything. Such a restriction
      // can be used if one has to give a restriction but does not have
      // one. 
      // The other constructors allow to give exceptions. Such restrictions
      // can be used e.g. when condensing rules. We assume that the
      // list of exceptions is small. 


      template< typename D > 
      struct weak_restriction : public range_restriction<D> 
      {
         std::list<D> exceptions;

         weak_restriction( ) { }
         weak_restriction( std::initializer_list<D> ex )
            : exceptions{ex} { }
         weak_restriction( const std::list<D> & ex )
            : exceptions{ex} { } 
         
         bool allows( D d ) const override 
            {  for( auto p = exceptions. cbegin( ); 
                         p != exceptions. cend( ); ++ p )
               {
                  if( *p == d ) return false;
               }
               return true;
            } 

         void print( std::ostream& out ) const override 
            {  if( exceptions. size( ))
               {
                  out << "weak_restriction( exceptions:{";
                  for( auto p = exceptions. cbegin( ); 
                            p != exceptions. cend( ); ++ p )
                  {
                     if( p != exceptions. cbegin( ))
                        out << ",";
                     out << *p;
                  }
                  out << "} )";
               }
               else
                  out << "weak_restriction( )";
           }
      };


      // A set restriction is defined by a set S.
      // The set S is referred through a pointer because a restriction
      // is a short-lived object.

      template< typename D > 
      struct set_restriction : public range_restriction<D>
      {
         const set<D> * range;

         set_restriction( const set<D> * r )
            : range{r}
         { }

         bool allows( D d ) const override 
            { return range -> contains(d); }
         void print( std::ostream& out ) const override
            { out << "set_restriction( " << *range << " )"; }
      };
 
      // A border restriction has form ( S, k ), where 
      // S is a set and k in S. 
      // Object d is allowed if
      // 1. d > k and d in S.
      // 2. d < k. 
      // border restrictions can be used to gradually squeeze a 
      // matching set in order to obtain a multiset-minimal solution. 
      // The set S is not stored, but referred to through a pointer.

      template< typename D > 
      struct border_restriction : public range_restriction<D> 
      {
         const set<D> * range;
         D k;

         border_restriction( const set<D> * r, D k )
            : range{r}, k{k}
         { }

         bool allows( D d ) const override
         {
            if( d < k ) return true;
            if( d == k ) return false;
            return range -> contains(d); 
         }

         void print( std::ostream& out ) const override
            { out << "border_restriction( " << *range << " / " << k << " )"; }
      };


      // An element restriction has form ( S, k ), where
      // S is a set and k in S.
      // d is allowed if d in S and d != k.
      // Element restrictions can be used to gradually remove elements
      // from a matching set until a subset minimal set is obtained.
      // The set S is referred to through a pointer.

      template< typename D > 
      struct element_restriction : public range_restriction<D> 
      {
         const set<D> * range;
         D k;

         element_restriction( const set<D> * r, D k )
            : range{r}, k{k}
         { }

         bool allows( D d ) const override
         {
            if( d == k ) return false;
            return range -> contains(d);
         }

         void print( std::ostream& out ) const override
            { out << "element_restriction( " << *range << " / " << k << " )"; }
      };
   }
}

#endif

