
// Written by Hans de Nivelle, August 2014.

#include "existential.h"
#include <stdexcept>

geometric::existential::existential( 
   std::initializer_list< varatom > passive,
   variable v, const varatom& at )
   : v{v}, at{at} 
{
   for( auto a : passive )  
      insert(a);
}

void geometric::existential::makerangerestricted( const signature& sig )
{
   for( auto p = at. args_cbegin( ); p != at. args_cend( ); ++ p )
   {
      if( *p != v )
         addcreate( sig, *p );
   }
}

geometric::normalizer
geometric::existential::normalize( ) 
{
   normalizer norm = rulebase::normalize( );

   if( norm. contains( v, label(0)))
      throw std::runtime_error( 
          "existential: Quantified variable occurs among passive" );
   v = norm. lookup( v, label(0) );
   at = norm. apply( at, label(0) ); 

   return norm; 
}

void geometric::existential::print( std::ostream& out ) const 
{
   printpassive( out );

   out << " | ";
   out << "EXISTS " << v << ": " << at;
}

auto geometric::existential::getfreevariables( ) const -> set<variable>
{
   set<variable> result = rulebase::getfreevariables( );
   for( variable w : at )
   {
      if( w != v ) result. insert(w);
         // Variable v is not free because it is bound by our quantifier. 
   }
   return result;
}


