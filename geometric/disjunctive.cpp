
// Written by Hans de Nivelle, August 2014.

#include "disjunctive.h"
#include "../tostring.h"


geometric::disjunctive::disjunctive( std::initializer_list< varatom > at )
{
   for( auto a : at )  
      insert(a);
}


void geometric::disjunctive::insert( varatom at )
{
   if( at. p. iscreate( ))
   {
      if( at. t != truthval::F )
         throw std::runtime_error( std::string( "cannot insert " ) + 
            tostring( at ) + 
            " to rule, because create must always have label F" );
   }

   if( at. p. isequality( ))
   {
      if( at. t != truthval::T )
         throw std::runtime_error( std::string( "cannot insert " ) +
            tostring( at ) + 
            " to rule, because equality must always have label T" );
   }

   for( auto p = passive. begin( ); p != passive. end( ); )
   {
      if( p -> p == at.p && equal_arguments( *p, at ))
      {
         at.t |= ( p -> t ); 
         p = passive. erase(p); 
      }
      else
         ++ p;
   }

   for( auto p = active. begin( ); p != active. end( ); )
   {
      if( p -> p == at.p && equal_arguments( *p, at )) 
      {
         at.t |= ( p -> t ); 
         p = active. erase(p);
      }
      else
         ++ p;
   }

   // If at.t contains F, or at.p is equality, then
   // the atom goes into passive. Otherwise into active.

   if( ( at.t & truthval::F ) != truthval::empty || at. p. isequality( ))
   {
      passive. push_back( at );
   }
   else
   {
      // Without false, we have empty,E,T,ET.

      switch( at.t )
      {
      case truthval::empty:
         // We don't add anything. 
         break; 
      case truthval::E:
         active. push_back( at );
         break; 
      case truthval::T:
         active. push_back( at );
         break;
      case truthval::ET:
         // We add them separately.
         at.t = truthval::E;
         active. push_back( at );
         at.t = truthval::T;
         active. push_back( at );
         break;   
      }
   }
}


void geometric::disjunctive::makerangerestricted( const signature& sig )
{
   for( auto p = active. cbegin( ); p != active. cend( ); ++ p )
   {
      for( auto q = p -> args_cbegin( ); q != p -> args_cend( ); ++ q )
         addcreate( sig, *q ); 
   }
}


geometric::normalizer geometric::disjunctive::normalize( ) 
{
   normalizer norm = rulebase::normalize( );
   for( auto p = active. begin( ); p != active. end( ); ++ p )
      *p = norm. apply( *p, label(0));
   return norm;
}


void geometric::disjunctive::print( std::ostream& out ) const 
{
   printpassive( out ); 

   out << " | "; 
   if( active. size( ))
   {
      for( auto p = active. begin( ); p != active. end( ); ++ p )
      {
         if( p != active. begin( ))
            out << ", ";
         out << *p;
      }
   }
   else
      out << "FALSE";
}

auto geometric::disjunctive::getfreevariables( ) const -> set<variable>
{
   set<variable> res = rulebase::getfreevariables( );
   for( const varatom& at : active )
   {
      for( variable v : at )
         res. insert(v);
   }
   return res;
}

