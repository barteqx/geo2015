
// Set of D, where D is a type that must have orders < and ==. 
// Written by Hans de Nivelle, 13.07.2014.
// Lookup, insertion, removal are O(log n).
// set< > does not support backtracking.

#ifndef GEOMETRIC_SET_INCLUDED
#define GEOMETRIC_SET_INCLUDED  1

#include <iostream>
#include <map>

namespace geometric
{

   template< typename D >
   class set
   {
      std::map< D, bool > repr;   // There are no false entries. 
                                  // It has to be map, because we 
                                  // want to use the set as ordered
                                  // multiset. 

   public: 
      class const_iterator
      {
         friend class set;

         typename std::map< D, bool > :: const_iterator it;

         const_iterator( typename std::map< D, bool > :: const_iterator it )
            : it{it}
         { }

      public:
         const_iterator operator ++ ( ) { ++it; return *this; }
         const_iterator operator -- ( ) { --it; return *this; } 
         const_iterator operator ++ ( int ) 
            { const_iterator res = *this; ++ it; return res; } 
         const_iterator operator -- ( int ) 
            { const_iterator res = *this; -- it; return res; }

         D operator * ( ) const { return it -> first; } 
         const D* operator -> ( ) const { return it -> first; }  

         bool operator == ( const_iterator p ) { return it == p. it; }
         bool operator != ( const_iterator p ) { return it != p. it; } 
      };


      set( ) { }

      set( std::initializer_list<D> s )
         { for( const D& d : s ) insert(d); }

      // Returns true if d was not present before:
 
      bool insert( D d )
      {
         auto p = repr. insert( std::pair< D, bool > ( d, true ));
         return p. second;
      }

      void insert( const set<D> & s )
      {
         for( const D& d : s ) insert(d);
      }

      set<D> & operator |= ( const set<D> & s )
      {
         insert(s);
         return *this;  
      }

      set<D> operator | ( const set<D> & s ) const
      {
         auto res = *this;
         res. insert(s);
         return res;
      }

      bool erase( D d ) { return repr. erase(d); }
         // Returns true if d was present. 

      bool contains( D d ) const { return repr. find(d) != repr. end( ); } 
      bool operator [] ( D d ) const { return repr. find(d) != repr. end( ); }
         // True if d is present.

      size_t size( ) const { return repr. size( ); } 

      const_iterator begin( ) const { return repr. cbegin( ); }
      const_iterator cbegin( ) const { return repr. cbegin( ); }
      const_iterator end( ) const { return repr. end( ); }
      const_iterator cend( ) const { return repr. cend( ); }

      void clear( ) { repr. clear( ); }
      void print( std::ostream& out ) const;

      D min( ) const { auto p = repr. begin( ); return p -> first; } 
         // Minimal element if we are not empty. 
      D max( ) const { auto p = repr. end( ); -- p; return p -> first; }
         // Maximal element if we are not empty.

      D glb( D d ) const
      {
         auto p = repr. lower_bound(d);
            // Finds first p for which
            // [ p is defined ] ( ! ( *p < d )).
            //                  ( d <= *p ).

         -- p; 
         return p -> first;  
      }
         
         // Returns the greatest element < d if we are not empty,
         // and d is not first( ). 

   };


   template< typename D > 
   void set<D> :: print( std::ostream& out ) const
   {
      out << "{";
      for( auto p = cbegin( ); p != cend( ); ++ p )
      {
         if( p != cbegin( ))
            out << ", ";
         else
            out << " ";
         out << *p;
      }
      out << " }";
   }
 
   template< typename D > 
   std::ostream& operator << ( std::ostream& out, const set<D> & s )
   {
      s. print( out );
      return out;
   }



   // Let d be the highest element for which
   // s1[d] != s2[d]. 
   // If no such element exists, then multi_compare returns 0.
   // If s1[d] = false and s2[d] = true, then multi_compare returns -1.
   // If s1[d] = true and s2[d] = false, then multi_compare returns 1.

   template< typename D >
   int multiset_compare( const set<D> & s1, const set<D> & s2 )
   {
      if( s1. size( ) == 0 )
      {
         if( s2. size( ) == 0 )
            return 0;
         else
            return -1;
      }

      if( s2. size( ) == 0 )
         return 1;

      // Both are non-empty.

      D m1 = s1. max( );
      D m2 = s2. max( );

      if( m1 < m2 ) return -1;
      if( m2 < m1 ) return 1;

      // At the point of the loop condition, we always have m1 == m2:

      while( m1 != s1. min( ) && m2 != s2. min( ))
      {
         m1 = s1. glb( m1 );
         m2 = s2. glb( m2 );
         if( m1 < m2 ) return -1;
         if( m2 < m1 ) return 1;
      }

      // We still have m1 == m2:

      if( m1 == s1. min( ))
      {
         if( m2 == s2. min( ))
            return 0; 
         return -1;
      }

      return 1;  // Because s1 has something left while s2 has not. 
   }


   // true if for all d: s1[d] == true implies s2[d] == true.

   template< typename D >
   bool subset( const set<D> & s1, const set<D> & s2 )
   {
      for( auto p = s1. cbegin( ); p != s1. cend( ); ++ p )
      {
         if( ! s2[ *p ] )
            return false;
      }

      return true;
   }
}

#endif

