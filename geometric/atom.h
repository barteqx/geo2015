
// Written by Hans de Nivelle, June 2014.

#ifndef GEOMETRIC_ATOM_INCLUDED 
#define GEOMETRIC_ATOM_INCLUDED   1

#include "signpred.h"
#include "truthval.h"
#include "../tostring.h"

#include "variable.h"
#include "element.h"

namespace geometric
{

   // Class atom is defined as a template over the class of
   // its arguments. 
   // varatom will be atom< variable, 6 > .
   // groundatom will be atom< element, 6 > .
   // One can also have atom< unsigned int > . 
   // Type A should have default constructor, copy constructor,
   // assignment, nothrowing move constructor and move assignment,
   // and == .

   template< typename A, size_t S = 6 > 
   struct atom
   {
      static_assert( S >= 3, "S must be at least 3" );

      predicate p;
      truthval t;

   private: 
      size_t arity; 
         // Is always equal to p. getarity( ). 

      struct short_arglist
      {
         A args [S];

         explicit short_arglist( size_t a ); 
         short_arglist( std::initializer_list<A> args );
      };

      struct long_arglist 
      {
         size_t arity;
         A* args;

         explicit long_arglist( size_t a );
         long_arglist( std::initializer_list<A> args );

         long_arglist( const long_arglist& a );
         long_arglist( long_arglist&& a ) noexcept;
         void operator = ( const long_arglist& a ) = delete;
         void operator = ( long_arglist&& a ) noexcept;
         ~long_arglist( ) { delete[] args; } 
      }; 

      // If arity <= S, then short_arglist is defined.  
      // If arity > S, then long_arglist is defined.
     
      union 
      {
         short_arglist   shortargs;
         long_arglist    longargs; 
      };

   public: 
      atom( predicate p, truthval t );
         // Construct p_{t}( a, ..., a ) using default constructor
         // of A.  

      atom( predicate, truthval, std::initializer_list<A> args );
         // Length of args must fit to arity of predicate!
 
      atom( ) = delete;
      atom( const atom& va );
      atom( atom&& va ) noexcept;
     
      void operator = ( atom&& va ) noexcept;
      void operator = ( const atom& va ) { *this = atom( va ); }
 
      A* args_begin( )
         { return arity <= S ? shortargs.args : longargs.args; }
      A* args_end( )
         { return args_begin( ) + arity; }

      const A* args_cbegin( ) const
         { return arity <= S ? shortargs.args : longargs.args; }
      const A* args_cend( ) const
         { return args_cbegin( ) + arity; } 

      const A* args_begin( ) const 
         { return arity <= S ? shortargs.args : longargs.args; }
      const A* args_end( ) const  
         { return args_begin( ) + arity; } 

      A& operator [] ( size_t i ) 
         { return args_begin( )[i]; }
      A operator [] ( size_t i ) const 
         { return args_cbegin( )[i]; }

      size_t getarity( ) const { return arity; } 

      bool contains( const A& a ) const;
         // True if atom contains a among its arguments. 

      ~atom( ); 
   };

   template< typename A, size_t S >
   std::ostream& operator << ( std::ostream& out, const atom<A,S> & at );

   template< typename A, size_t S >
   bool equal_arguments( const atom<A,S> & at1, const atom<A,S> & at2 );
      // True if a1 and a2 have equal arguments. This function 
      // strongly assumes that at1 and at2 have the same predicate symbol.
      // It treats equality correctly (i.e. symmetric). 
      // It uses operator == on A. 


   ////////////////////////////////////////
   //         Implementations:           //
   ////////////////////////////////////////

   // short_arglist:

   template< typename A, size_t S > 
   atom<A,S>::short_arglist::short_arglist( size_t a )
   {
      // We don't check if a fits. 
   }

   template< typename A, size_t S > 
   atom<A,S>::short_arglist::short_arglist( 
      std::initializer_list<A> args )
   {
      size_t i = 0;
      for( auto p = args. begin( ); p != args. end( ); ++ p )
         ( this -> args ) [ i ++ ] = *p;
   }


   // long_arglist:

   template< typename A, size_t S > 
   atom<A,S>::long_arglist::long_arglist( size_t ar ) 
      : arity{ ar },
        args{ new A[ ar ] }
   {
   }


   template< typename A, size_t S > 
   atom<A,S>::long_arglist::long_arglist( std::initializer_list<A> args )
      : arity{ args. size( ) },
        args{ new A[ args. size( ) ] } 
   {
      size_t i = 0; 
      for( auto p = args. begin( ); p != args. end( ); ++ p )
         ( this -> args ) [ i ++ ] = *p; 
   }


   template< typename A, size_t S > 
   atom<A,S>::long_arglist::long_arglist( const long_arglist& a )
      : arity{ a. arity },
        args{ new A [ a. arity ] }
   {
      for( size_t i = 0; i < arity; ++ i )
         args [i] = a. args [i];
   }


   template< typename A, size_t S > 
   atom<A,S>::long_arglist::long_arglist( long_arglist&& a ) noexcept
      : arity{ a. arity },
        args{ a. args } 
   {
      a. arity = 0;
      a. args = nullptr;
   }


   template< typename A, size_t S > 
   void atom<A,S>::long_arglist::operator = ( long_arglist&& a ) noexcept
   {
      std::swap( arity, a. arity );
      std::swap( args, a. args );
   }


   // varatom:


   template< typename A, size_t S > 
   atom<A,S>::atom( predicate p, truthval t ) 
      : p{p},
        t{t},
        arity{ p. getarity( ) }
   {
      if( arity <= S )
         new( &shortargs ) short_arglist( arity );
      else
         new( &longargs ) long_arglist( arity );
   }

   template< typename A, size_t S > 
   atom<A,S>::atom( predicate p, truthval t, 
                    std::initializer_list<A> args )
      : p{p},
        t{t},
        arity{ args. size( ) }
   {
      if( p. getarity( ) != args. size( ))
         throw std::length_error(
                 std::string( "arity " ) +
                 tostring( args. size( )) + " does not fit to predicate " +
                 tostring( p ));

      if( arity <= S )
         new( &shortargs ) short_arglist( args );
      else
         new( &longargs ) long_arglist( args );    
   }


   template< typename A, size_t S > 
   atom<A,S>::atom( const atom& va )
      : p{ va. p },
        t{ va. t },
        arity{ va. arity }
   {
      if( arity <= S )
         new( &shortargs ) short_arglist( va. shortargs );
      else
         new( &longargs ) long_arglist( va. longargs );
   }


   template< typename A, size_t S > 
   atom<A,S>::atom( atom&& va ) noexcept 
      : p{ va. p }, 
        t{ va. t }, 
        arity{ va. arity }
   {
      if( arity <= S )
         new( &shortargs ) short_arglist( std::move( va. shortargs ));
      else
         new( &longargs ) long_arglist( std::move( va. longargs ));
   }


   template< typename A, size_t S > 
   void atom<A,S>::operator = ( atom&& at ) noexcept
   {
      if( arity <= S )
      {
         if( at. arity <= S )
            std::swap( shortargs, at. shortargs );
         else
         {
            // We are short, at is long. 
            // This is a complicated swap:

            long_arglist la = std::move( at. longargs );
            at. longargs. ~long_arglist( );

            new( &at.shortargs ) short_arglist( std::move( shortargs ));
            shortargs. ~short_arglist( );

            new( &longargs ) long_arglist( std::move( la ));
         }
      }
      else
      {
         if( at. arity <= S )
         {
            // We are long, at is short:
            // We have to swap:

            long_arglist la = std::move( longargs );
            longargs. ~long_arglist( );

            new( &shortargs ) short_arglist( std::move( at. shortargs ));
            at. shortargs. ~short_arglist( );

            new( &at.longargs ) long_arglist( std::move( la ));
         }
         else
            std::swap( longargs, at. longargs );
      }

      std::swap( p, at. p );
      std::swap( t, at. t );
      std::swap( arity, at. arity );
   }


   template< typename A, size_t S > 
   atom<A,S>:: ~atom( )
   {
      if( arity <= S )
         shortargs. ~short_arglist( );  
      else
         longargs. ~long_arglist( ); 
   }


   // So that one can write    for( auto x : at ):
 
   template< typename A, size_t S >
   inline const A* begin( const atom<A,S> & at ) 
   {
      return at. args_cbegin( );
   }

   template< typename A, size_t S >
   inline const A* end( const atom<A,S> & at )
   {
      return at. args_cend( );
   }

   template< typename A, size_t S >
   inline A* begin( atom<A,S> & at )
   {
      return at. args_begin( );
   }

   template< typename A, size_t S >
   inline A* end( atom<A,S> & at )
   {
      return at. args_end( );
   }


   template< typename A, size_t S >
   inline bool atom<A,S>:: contains( const A& arg ) const 
   {
      for( const A& a : *this ) 
      {
         if( a == arg )
            return true;
      }
      return false;
   }


   template< typename A, size_t S > 
   std::ostream& operator << ( std::ostream& out, const atom<A,S> & at )
   {
      if( at.p. isequality( ) && at.t == truthval::T )
      {
         out << at[0] << ' ' << at.p << ' ' << at[1];
         return out; 
      }

      if( at.p. iscreate( ))
      {
         out << at.p << "_{" << at.t << "} ";
         out << at[0];
         return out;
      }

      out << at.p << "_{" << at.t << "}(";
      for( auto p = at. args_begin( ); p != at. args_end( ); ++ p )
      {
         if( p != at. args_begin( ))
            out << ',';
         out << *p;
      }
      out << ')';
      return out;
   }


   template< typename A, size_t S >
   inline bool equal_arguments( const atom<A,S> & at1, const atom<A,S> & at2 )
   {
      // We assume that at1.p == at2.p.

      if( at1.p.isequality( ))  
      {
         return ( at1[0] == at2[0] && at1[1] == at2[1] ) || 
                ( at1[0] == at2[1] && at1[1] == at2[0] );
      }
      else
      {
         auto p1 = at1. args_cbegin( );
         auto p2 = at2. args_cbegin( );

         while( p1 != at1. args_cend( ))
         {
            if( *p1 != *p2 )
               return false;
            ++ p1; ++ p2;
         }
         return true;
      }
   }

   using varatom = atom< variable, 6 > ;
   using groundatom = atom< element, 6 > ;
   using intatom = atom< int, 6 > ;

}

#endif
      

