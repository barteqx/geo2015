
// Written by Hans de Nivelle, December 2014.
// A subst is a partial function from variables to pretty
// much every domain D. 
// subst does not support backtracking, this is done by subststack.


#ifndef GEOMETRIC_SUBST_INCLUDED
#define GEOMETRIC_SUBST_INCLUDED   1

#include "variable.h"
#include "element.h"
#include "atom.h"
#include <unordered_map>
#include <stdexcept> 


namespace geometric
{

   template< typename D >
   class subst
   {
      std::unordered_map< variable, D, 
                          variable::hash, variable::equal_to > repr;
 
   public:
      subst( std::initializer_list<std::pair<const variable,D>> init = {} )
         : repr{ init } 
      { }
   
      void clear( ) { repr. clear( ); } 

      size_t size( ) const { return repr. size( ); }

      D* lookup( variable v ); 
      const D* clookup( variable v ) const;
      const D* lookup( variable v ) const { return clookup(v); } 
         // Returns nullptr if there is no value.
         // lookup( ) is constant in size( ).

      bool assign( variable v, const D& d );
         // If no assignment to v is present, create (v,d) and return true.
         // Otherwise, change nothing and return false. 

      void remove( variable v ) { repr. erase(v); } 
         // Remove variable v from the domain.
 
      atom<D> apply( const varatom& at ) const;
         // Instantiate at. Throw a runtime_error 
         // if some variable is not defined. 

      atom<D> apply( const varatom& at, variable v, const D& d ) const;
         // Instantiate at by *this;{ v :=e }. The advantage of this method
         // is that existential variables can be included without
         // modifying the substitution, so that it can be const.
         // If v occurs in the domain, then { v:=d } has priority. 

      using const_iterator = 
         typename std::unordered_map< variable, D, 
                                      variable::hash, variable::equal_to > :: 
                    const_iterator;

      const_iterator begin( ) const { return repr. cbegin( ); }
      const_iterator cbegin( ) const { return repr. cbegin( ); }

      const_iterator end( ) const { return repr. cend( ); }
      const_iterator cend( ) const { return repr. cend( ); }

      void print( std::ostream& ) const; 

   };


   template< typename D >
   bool subst<D> :: assign( variable v, const D& d )
   {
      auto p = repr. insert( std::make_pair(v,d) ); 
      return p. second;
   }

   template< typename D >
   const D* subst<D> :: clookup( variable v ) const
   {
      auto p = repr. find(v); 
      if( p != repr. end( )) 
         return & ( p -> second );
      else
         return nullptr;
   }

   template< typename D >
   D* subst<D> :: lookup( variable v ) 
   {
      auto p = repr. find(v);
      if( p != repr. end( ))
         return & ( p -> second );
      else
         return nullptr; 
   }

   template< typename D > 
   void subst<D>::print( std::ostream& out ) const
   {
      out << "subst(";
      for( auto p = repr. begin( ); p != repr. end( ); ++ p )
      {
         if( p != repr. begin( ))
            out << ", ";
         else
            out << " ";
         out << ( p -> first ) << " := " << ( p -> second );
      }
      out << " )";
   }


   template< typename D > 
   std::ostream& operator << ( std::ostream& out, const subst<D> & d )
   {
      d. print( out );
      return out;
   }

   template< typename D > 
   atom<D> subst<D> :: apply( const varatom& at ) const
   {
      atom<D> res( at.p, at. t );
      auto p = at. args_cbegin( );
      auto q = res. args_begin( ); 

      while( p != at. args_cend( ))
      {
         const D* d = lookup(*p);
         if(d)
            *q = *d;
         else
            throw std::runtime_error( 
               "subst::apply(at): variable has no instantiation" );
         ++ p; ++ q;
      }
      return res; 
   } 

   template< typename D >
   atom<D> subst<D> :: apply( const varatom& at, 
                              variable v, const D& d ) const
   {
      atom<D> res( at.p, at. t );
      auto p = at. args_cbegin( );
      auto q = res. args_begin( );

      while( p != at. args_cend( ))
      {
         if( *p == v )
            *q = d;
         else
         {
            const D* x = lookup(*p);
            if(x)
               *q = *x;
            else
            {
               throw std::runtime_error(
                  "subst::apply( at,v,d ): variable has no instantiation" );
            }
         }
         ++ p; ++ q;
      }
      return res;
   }

}

#endif

