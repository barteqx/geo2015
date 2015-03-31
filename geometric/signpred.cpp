
#include "signpred.h"
#include <stdexcept> 

bool 
geometric::signature::pred_equals::operator( ) 
               ( const name_arity& n1, const name_arity& n2 ) const
{
   if( n1. arity != n2. arity || n1. name. size( ) != n2. name. size( )) 
      return false;
   {
      for( size_t i = 0; i != n1. name. size( ); ++ i )
      {
         if( n1.name[i] != n2.name[i] )
            return false;
      }
      return true; 
   }
}


size_t 
geometric::signature::pred_hash::operator( ) ( const name_arity& n ) const 
{
   size_t val = n. arity;
   std::hash< std::string > h;

   for( size_t i = 0; i < n. name. size( ); ++ i )
   { 
      val *= 1009; 
      val += h( n.name[i] ); 
   }
   return val; 
}


geometric::signature::signature( )
   : equality{ lookup( { "==" }, 2 )},
     create{ lookup( { "#" }, 1 )}
{
}


bool
geometric::signature::isdefined( const nameformat& name,
                                 size_t arity ) const
{
   return lookuptable. find( { name, arity } ) != lookuptable. end( );
}


geometric::predicate
geometric::signature::clookup( const nameformat& name, 
                               size_t arity ) const
{
   auto p = lookuptable. find( { name, arity } );
   if( p == lookuptable. end( ))
      throw std::runtime_error( "clookup could not find name" );

   return predicate( this, p -> second );
}


geometric::predicate
geometric::signature::lookup( const nameformat& name, size_t arity )
{
   auto p = lookuptable. find( { name, arity } );
   if( p != lookuptable. end( ))
      return predicate( this, p -> second );
   else
   {
      // We extend the signature:

      size_t s = symbols. size( ); 
      symbols. push_back( { name, arity } ); 
      lookuptable. insert( { { name, arity }, s } );
      return predicate( this, s );
   }
}


std::ostream& geometric::operator << ( std::ostream& out, const predicate& p )
{
   auto p1 = p. sig -> symbols [ p. index ]. name. begin( );
   auto p2 = p. sig -> symbols [ p. index ]. name. end( ); 

   if( p1 == p2 )
      out << "(EMPTY)";
   else
   {
      for( auto p = p1; p != p2; ++ p )
      {
         if( p != p1 )
            out << "::";
         out << *p;
      }
   }
   return out;  
}

std::ostream& geometric::operator << ( std::ostream& out, const signature& s )
{
   out << "Signature:\n"; 

   for( size_t i = 0; i < s. symbols. size( ); ++ i ) 
   {
      out << "   " << i << ":       ";
      out << predicate( &s, i ) << "/" << s. symbols [i]. arity << "\n";
   }
   out << "\n";

   if( s. symbols. size( ) != s. lookuptable. size( ))
      throw std::runtime_error( 
               "symbols and lookuptable in signature have different size" ); 

   return out;
}

