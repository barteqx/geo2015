
// A base is a datastructure which facilities quick lookup of atoms
// that we are matching into. It is called 'match base' in the report.
// Written by Hans de Nivelle, August/September 2014.

// NEEDS TO BE REWRITTEN (ONCE MORE)!
// I want additional indexing on pairs (p,V). 
// So we have predicates, variables, and pairs of predicates and
// variables.

#ifndef GEOMETRIC_BASE_INCLUDED
#define GEOMETRIC_BASE_INCLUDED  1

#include <iostream>
#include <vector>
#include "range.h"

namespace geometric
{

   template< typename A >
   void push_back( std::vector< std::vector<A>> & v, unsigned int i, A a )
   {
      while( v. size( ) <= i )
         v. push_back( std::vector<A>( ));
      v[i]. push_back(a);
   }


   // Class A must have a .getindex( ) const method.

   template< typename A, unsigned int S = 6 > 
   class base 
   {
      std::vector< atom<A,S>> atoms;

      std::vector< std::vector< unsigned int >> predicates;
         // Contains, for every predicate symbol, the indices of the atoms 
         // in which the symbol occurs. New atoms are always added at the 
         // end of atoms, after which the index is appended to the proper 
         // vector in atoms. 
 
      std::vector< std::vector< unsigned int >> arguments;
         // Contains, for argument A, the indices of the atoms in which this
         // A occurs. Each vector of indices is sorted. 

   public: 
      unsigned int insert( const atom<A,S> & at );
      unsigned int insert( std::initializer_list< atom<A,S>> at );

      unsigned int size( ) const { return atoms. size( ); } 
      void restore( unsigned int k );
         // Restore can be used in two ways:
         //    unsigned int s = size( );
         //    .... couple of inserts.
         //    backtrack(s);
         //
         //    unsigned int s = insert( .. ); ... some more inserts.
         //    backtrack(s);

      void print( std::ostream& out ) const; 

      range<unsigned int> find( predicate p ) const;
      range<unsigned int> find( A a ) const; 
         // Guaranteed to be in non-decreasing order,
         // so that range::findcommon can be used. 

   };


   template< class A, unsigned int S > 
   unsigned int base<A,S> :: insert( const atom<A,S> & at )
   {
      unsigned int s = atoms. size( );
      atoms. push_back( at );  
     
      push_back( predicates, at. p. getindex( ), s );  
         // This is a local method that increases the size of predicates,
         // if required.

      // If the same argument occurs twice in at, we insert it only once:

      for( auto p = at. args_cbegin( ); p != at. args_cend( ); ++ p )
      {
         for( auto q = at. args_cbegin( ); q != p; ++ q )
            if( *p == *q ) goto dontadd;

         push_back( arguments, p -> getindex( ), s ); 
            // If necessary, the size of arguments will be increased. 
      dontadd:
         ;
      }
      return s; 
   }


   template< class A, unsigned int S >
   unsigned int base<A,S>::insert( std::initializer_list< atom<A,S>> at )
   {
      unsigned int s = atoms. size( );

      for( auto p = at. begin( ); p != at. end( ); ++ p )
         insert(*p);
     
      return s;
   }  
       

   template< class A, unsigned int S > 
   void base<A,S> :: restore( unsigned int k )
   {
      while( atoms. size( ) > k )
      {
         atom<A,S> at = atoms. back( );
         atoms. pop_back( ); 
 
         predicates[ at. p. getindex( ) ]. pop_back( );
            // We know that the references were added in chronological
            // order, so we can remove from the end. 

         // If the same argument occurs twice, it was inserted only
         // once. This implies that we also should remove it
         // only once. 

         for( auto p = at. args_cbegin( ); p != at. args_cend( ); ++ p )
         {
            for( auto q = at. args_cbegin( ); q != p; ++ q )
               if( *p == *q ) goto dontremove;

            arguments[ p -> getindex( ) ]. pop_back( );
               // Again, we can safely remove from the end. 
         dontremove:
            ;
         }
      }
   }


   template< typename A, unsigned int S >
   void base<A,S> :: print( std::ostream& out ) const 
   {
      out << "Match Base:\n";
      out << "atoms:\n";
      for( unsigned int i = 0; i < atoms. size( ); ++ i )
      {
         out << "   " << i << " : " << atoms[i] << "\n";
      }
      out << "\n";

      out << "predicates:\n";
      for( unsigned int i = 0; i < predicates. size( ); ++ i )
      {
         if( predicates[i]. size( ))
         {
            // We have to go through atoms, in order to find the predicate:

            auto a = atoms. begin( );
            while( a -> p. getindex( ) != i )
               ++ a;

            out << "   " << ( a -> p ) << " : ";
            out << range< unsigned int > ( predicates [i] );
            out << "\n";
         }
      }
      out << "\n";

      out << "arguments:\n"; 
      for( A a; a. getindex( ) < arguments. size( ); ++ a )
      {
         if( arguments [ a. getindex( ) ]. size( ))
         {
            out << "   " << a << " : ";
            out << range< unsigned int > ( arguments[ a. getindex( ) ] );
            out << "\n"; 
         }
      }
      out << "\n";
   }


   template< typename A, unsigned int S >
   range< unsigned int > 
   base< A,S> ::find( predicate p ) const 
   {
      static std::vector< unsigned int > empty;
          // This is thread safe, as far as I can see.
          // We need an empty vector, so that we can return
          // iterators to it, when the predicate p is out of range. 

      unsigned int i = p. getindex( );
      if( i < predicates. size( ))
         return range< unsigned int > ( predicates [i] );
      else
         return range< unsigned int > ( empty );
   }


   template< typename A, unsigned int S >
   range< unsigned int > 
   base<A,S> ::find( A a ) const
   {
      static std::vector< unsigned int > empty;
          // This is thread safe, as far as I can see.
          // We need an empty vector, so that we can return
          // iterators to it, when the argument a is out of range.

      unsigned int i = a. getindex( );
      if( i < arguments. size( ))
         return range< unsigned int > ( arguments [i] );
      else
         return range< unsigned int > ( empty ); 
   }


   template< typename A, unsigned int S >
   std::ostream& operator << ( std::ostream& out, const base<A,S> & b )
   {
      b. print( out );
      return out;
   }
 
}

#endif

