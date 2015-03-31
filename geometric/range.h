
// Written by Hans de Nivelle, August 2013.

#ifndef GEOMETRIC_RANGE_INCLUDED
#define GEOMETRIC_RANGE_INCLUDED  1

#include <stdexcept>

namespace geometric
{
   // A range is just a pair of const_iterators p1,p2 with p1 <= p2.
   // We assume of course that the vector remains valid as long as the
   // range exists. 
   // ranges are returned by a base when a predicate or
   // variable is looked up. 

   template< typename A > 
   struct range
   {
      typename std::vector<A> :: const_iterator p1;
      typename std::vector<A> :: const_iterator p2;


      explicit range( const std::vector<A> & v )
         : p1{ v. cbegin( ) },
           p2{ v. cend( ) }
      { } 

      A operator * ( ) const { return *p1; } 
      A operator [] ( size_t i ) const { return p1[i]; }
      void operator += ( size_t i ) { p1 += i; }

      range& operator ++ ( ) 
      {
         ++ p1;
         return *this; 
      }

      range operator ++ ( int )
      {
         range<A> res = *this;
         ++ p1;
         return res;  
      }

      size_t size( ) const { return p2 - p1; }
      bool isempty( ) const { return p1 == p2; }
   
      void advanceto( A a );
         // Advance ourselves to the first position that
         // is less or equal to a, if possible. 
         // Otherwise to the end. We assume that A has an < operator.  
         // We take log(n) time, where n is the number of elements
         // skipped.

   };


   template< typename A >
   std::ostream& operator << ( std::ostream& out, range<A> r )
   {
      out << "{";
      for( auto p = r. p1; p != r. p2; ++ p )
      {
         if( p != r. p1 )
            out << ", ";
         else
            out << " ";
         out << *p;
      }
      out << " }";
      return out;
   }


   template< typename A >
   std::ostream& operator << ( std::ostream& out, 
                               const std::vector<range<A>> & v ) 
   {
      out << "Range Vector:\n";
      for( const auto& r : v )
         out << "   " << r << "\n";
      out << "\n";
      return out;
   }

   
   template< class A >
   inline void range<A>::advanceto( A a )
   {
      // Invariant is that all elements i with i < 0 (the skipped elements)
      // have (*this)[i] < a.

      size_t s = 1;
      while( s < size( ) && (*this)[s] < a )
      { 
         (*this) += ( s + 1 );
         s <<= 1;
      }  

      // Invariant: The first position i with
      // (*this)[i] >= a lies in the interval [ 0, ..., s ],
      // if it exists. 
      // If such position i does not exist, then the interval includes
      // size( ). 
      
      while(s)
      {
         s >>= 1;
         if( s < size( ) && (*this)[s] < a )
            (*this) += (s+1);
      }
   }

 
   // This function only works when all elements
   // in ranges are sorted increasing by the order <. 
   // We advance all members of ranges to the
   // first common element and return true if we managed to do this.
   // Otherwise, we return false. This implies that one of the 
   // ranges involved has reached the end, but not necessarily
   // all ranges. Findcommon uses < for comparison, and also ==.  

   template< class A > bool findcommon( std::vector< range<A>> & ranges )
   {
      if( ranges. size( ) == 0 )
         throw std::logic_error( "findcommon: range vector is empty" ); 

      if( ranges[0]. isempty( ))
         return false;
      A val = *( ranges[0] );   // Invariant is that the first candidate 
                                // is creater or equal to val.
   
      bool unchecked = true;
      while( unchecked )
      {
         unchecked = false;
         
         // Advance val to the greatest value among the first elements:

         for( auto p = ranges. begin( ); p != ranges. end( ); ++ p )
         {
            if( p -> isempty( )) 
               return false;
            if( val < *(*p) )
               val = *(*p);
         }

         // In the next step, we advance all ranges to the
         // first position that is greater or equal to val.
         // If this makes one of the ranges empty, we fail.  
         // If one of the ranges gets advanced to a higher value, we
         // set unchecked = true, and increase value.

         for( auto p = ranges. begin( ); p != ranges. end( ); ++ p )
         {
            p -> advanceto( val );
            if( p -> isempty( )) 
               return false;
            if( val < *(*p))
            {
               val = *(*p);
               unchecked = true;
            }
         }         
      }

      return true; 
   }
}

#endif


