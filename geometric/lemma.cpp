
// Written by Hans de Nivelle, August 2014.

#include "lemma.h"

geometric::lemma::lemma( std::initializer_list< varatom > passive )
{
   for( auto a : passive )
      insert(a);
}

geometric::lemma::lemma( std::initializer_list< varatom > passive,
                         const varatom& active )
   : active{ { active } } 
{
   for( auto a : passive )
      insert(a);
}

void geometric::lemma::makerangerestricted( const signature& sig )
{
   for( auto p = active. cbegin( ); p != active. cend( ); ++ p )
   {
      for( auto q = p -> args_cbegin( ); q != p -> args_cend( ); ++ q )
         addcreate( sig, *q );
   }
}

geometric::normalizer geometric::lemma::normalize( )
{
   normalizer norm = rulebase::normalize( );
   for( auto p = active. begin( ); p != active. end( ); ++ p )
      *p = norm. apply( *p, label(0));
   return norm;
}

void geometric::lemma::print( std::ostream& out ) const 
{
   printpassive( out );

   if( hasactive( ))
   {
      out << " | ";
      out << getactive( ); 
   }
   else
      out << " | FALSE";
}

auto geometric::lemma::getfreevariables( ) const -> set<variable>
{ 
   set<variable> res = rulebase::getfreevariables( ); 
   for( const varatom& at : active )
   {
      for( variable v : at )
         res. insert(v);
   }
   return res;
}


