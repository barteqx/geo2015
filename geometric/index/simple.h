
// A simple index contains a couple of indices that enable quick
// lookup: More precisely, we have: 
//    an index on predicate names.
//    an index on arguments.
//    an index on triples of predicate name, position, and argument.
//
// Written by Hans de Nivelle, September 2014.
// Improved December 2014. (Use hashmap. Included argument position.)


#ifndef GEOMETRIC_INDEX_SIMPLE_INCLUDED
#define GEOMETRIC_INDEX_SIMPLE_INCLUDED  1

#include <iostream>
#include <unordered_map> 
#include "../range.h"
#include "../atom.h"

namespace geometric
{
   namespace index
   {

      // A is the argument type.
      // H is a hasher on the argument type.
      // E is an equality tester on the argument type.
      // S is a historical accident. 

      template< typename A, 
                typename H = std::hash<A>, 
                typename E = std::equal_to<A>,
                size_t S = 6 > 
      class simple
      {

         // Used for lookup of triples (p,i,a) in a hash map: 
         // i is a position on which predicate p has argument a.
         
         struct predarg
         {
            predicate p;
            size_t i; 
            A a;

            predarg( predicate p, size_t i, const A& a )
               : p{p}, i{i}, a{a} { } 

            void print( std::ostream& out ) const
               { out << p << "[" << i << "]:" << a; }
         };


         // hasheq hashes and compares predargs. Using the same 
         // object for equality testing and hashing seems OK:

         struct hasheq
         {
            H h;
            E e;

            size_t operator( ) ( const predarg& p ) const
            {
               return predicate::hash()( p.p ) * 10007 + p.i * 23 + h( p.a ); 
            }

            bool operator( ) ( const predarg& p1, const predarg& p2 ) const
            {
               return predicate::equal_to()( p1.p, p2.p ) && 
                      p1.i == p2.i && e( p1.a, p2.a );
            } 
         };

          
         std::vector< atom<A,S>> atoms;
            // Our stored atoms.

         std::unordered_map< predicate, std::vector< size_t >,
                             predicate::hash, predicate::equal_to > predicates;
            // Maps predicates to vectors of indices of atoms. 
 
         std::unordered_map< A, std::vector< size_t >, H, E > arguments;
            // Maps arguments to vectors of indices of atoms. 

         std::unordered_map< predarg, std::vector< size_t >,
                             hasheq, hasheq > predargs;
            // Index on triples (p,i,a), where p is a predicate, and i is
            // a position on which a occurs. 

      public: 
         size_t insert( const atom<A,S> & at );
         size_t insert( std::initializer_list< atom<A,S>> at );

         size_t size( ) const { return atoms. size( ); } 
            // Number of atoms in the index.

         void restore( size_t k );
            // Restore can be used in two ways:
            //    size_t s = size( );
            //    .... couple of inserts.
            //    backtrack(s);
            //
            //    size_t s = insert( .. ); 
            //       ... some more inserts.
            //    backtrack(s);

         size_t nrarguments( ) const { return arguments. size( ); }
            // Number of distinct arguments that we know. 

         void print( std::ostream& out ) const; 

         range<size_t> find( predicate p ) const;
         range<size_t> find( const A& a ) const; 
         range<size_t> find( predicate p, size_t i, const A& a ) const; 
            // Guaranteed in non-decreasing order,
            // so that range::findcommon can be used. 
            // If p is equality, then i must be zero. (Because of symmetry.)
 
         const atom<A,S> & operator [] ( size_t i ) const
            { return atoms[i]; } 
               // The i-th atom. 

         typename std::vector<atom<A,S>> :: const_iterator cbegin( ) const 
            { return atoms. cbegin( ); }
         typename std::vector<atom<A,S>> :: const_iterator cend( ) const 
            { return atoms. cend( ); } 
   
      };

      template< typename A, typename H, typename E, size_t S > 
      size_t simple<A,H,E,S> :: insert( const atom<A,S> & at )
      {
         size_t s = atoms. size( );
         atoms. push_back( at );  
     
         predicates[ at. p ]. push_back(s);

         // We go through the arguments, and add to arguments.
         // If the same argument occurs twice, we add only once.

         for( auto p = at. args_cbegin( ); p != at. args_cend( ); ++ p )
         {
            for( auto q = at. args_cbegin( ); q != p; ++ q )
               if( arguments. key_eq( )( *p, *q )) goto dontadd;
                  // arguments. key_eq is E, our equality test on arguments.

            arguments[ *p ]. push_back(s);  
         dontadd:
            ;
         }

         // Next we add arguments and position.
         // Equality needs special attention, because it is symmetric.
   
         if( !at. p. isequality( )) 
         {
            for( size_t i = 0; i < at. getarity( ); ++ i )
               predargs[ predarg( at.p,i, at[i] ) ]. push_back(s);
         }
         else
         {
            predargs[ predarg( at.p, 0, at[0] ) ]. push_back(s);

            // If second argument is distinct from first, as determined by
            // E, we add it. Equality arguments are always added at index
            // 0.

            if( !arguments. key_eq( )( at[0], at[1] ))
               predargs[ predarg( at.p, 0, at[1] ) ]. push_back(s);
         }
         return s; 
      }


      template< typename A, typename H, typename E, size_t S >
      size_t simple<A,H,E,S>::insert( std::initializer_list< atom<A,S>> at )
      {
         size_t s = atoms. size( );

         for( auto p = at. begin( ); p != at. end( ); ++ p )
            insert( *p );
     
         return s;
      }  
  
 
      template< typename A, typename H, typename E, size_t S > 
      void simple<A,H,E,S> :: restore( size_t k )
      {
         while( atoms. size( ) > k )
         {
            atom<A,S> at = atoms. back( );
            atoms. pop_back( ); 

            // We look up the occurrence list for at.p, and remove
            // one element from the end. If the list gets empty,
            // we remove it completely, because don't like empty lists.

            auto p = predicates. find( at.p );
            p -> second. pop_back( );
            if( p -> second. empty( ))
               predicates. erase(p);

            // If the same argument occurs twice, it was inserted only
            // once. This implies that we also should remove it
            // only once. 

            for( auto p = at. args_cbegin( ); p != at. args_cend( ); ++ p )
            {
               for( auto q = at. args_cbegin( ); q != p; ++ q )
                  if( arguments. key_eq( )( *p, *q )) goto dontremove;
               {
                  auto a = arguments. find( *p );
                  a -> second. pop_back( );
                  if( a -> second. empty( ))
                     arguments. erase(a);
               } 
            dontremove:
               ;
            }

            if( !at. p. isequality( ))
            {
               for( size_t i = 0; i < at. getarity( ); ++ i )
               {
                  auto a = predargs. find( predarg( at.p, i, at[i] ));
                  a -> second. pop_back( );
                  if( a -> second. empty( ))
                     predargs. erase( a );
               }
            }
            else
            {
               auto a = predargs. find( predarg( at.p, 0, at[0] ));
               a -> second. pop_back( );
               if( a -> second. empty( ))
                  predargs. erase( a );

               if( !arguments. key_eq( )( at[0], at[1] ))
               {
                  a = predargs. find( predarg( at.p, 0, at[1] ));
                  a -> second. pop_back( ); 
                  if( a -> second. empty( ))
                     predargs. erase( a );
               }
            }
         }
      }


      template< typename A, typename H, typename E, size_t S >
      void simple<A,H,E,S> :: print( std::ostream& out ) const 
      {
         out << "Simple Index:\n";
         out << "atoms:\n";
         for( size_t i = 0; i < atoms. size( ); ++ i )
         {
            out << "   " << i << " :   " << atoms[i] << "\n";
         }
         out << "\n";

         out << "predicate:\n";
         for( const auto& pred: predicates )
         {
            out << "   " << pred. first << " :   ";
            out << range< size_t > ( pred. second );
            out << "\n";
         }
         out << "\n";

         out << "argument:\n"; 
         for( const auto& arg: arguments )
         {
            out << "   " << arg. first << " : ";
            out << range< size_t > ( arg. second ); 
            out << "\n"; 
         }
         out << "\n";

         out << "predicate/argument:\n";
         for( const auto& pa : predargs )
         {
            out << "   ";
            pa. first. print( out );
            out << " :   ";
            out << range< size_t > ( pa. second );
            out << "\n";
         }
      }


      template< typename A, typename H, typename E, size_t S >
      range< size_t > 
      simple< A,H,E,S> ::find( predicate p ) const 
      {
         static const std::vector< size_t > empty;
             // This is thread safe, according to the standard.

         auto i = predicates. find(p);
         if( i != predicates. end( ))
            return range< size_t > ( i -> second );
         else
            return range< size_t > ( empty );
      }


      template< typename A, typename H, typename E, size_t S >
      range< size_t > 
      simple<A,H,E,S> ::find( const A& a ) const
      {
         static const std::vector< size_t > empty;
             // This is thread safe, according to the standard.

         auto i = arguments. find(a);
         if( i != arguments. end( ))
            return range< size_t > ( i -> second );
         else
            return range< size_t > ( empty ); 
      }

     
      template< typename A, typename H, typename E, size_t S >
      range< size_t >
      simple<A,H,E,S> :: find( predicate p, size_t i, const A& a ) const
      {
         static const std::vector< size_t > empty;
            // Thread safe according to the standard. 

         auto it = predargs. find( predarg( p, i, a )); 
         if( it != predargs. end( ))
            return range< size_t > ( it -> second );
         else
            return range< size_t > ( empty ); 
      }


      template< typename A, typename H, typename E, size_t S >
      std::ostream& 
      operator << ( std::ostream& out, const simple<A,H,E,S> & s )
      {
         s. print( out );
         return out;
      }


      // An abbreviation that can be used for types T that have
      // T::hash and T::equal_to. This is the case for
      // element and variable. 

      template< typename T > using standardsimple = 
         simple< T, typename T::hash, typename T::equal_to > ;
   } 
}

#endif


