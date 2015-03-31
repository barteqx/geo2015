
// Written by Hans de Nivelle, September 2014.

#include "unification.h"

std::ostream& 
geometric::operator << ( std::ostream& out, geometric::labeledvar lv )
{
   out << lv.var << '/' << lv.lab;
   return out;
}

geometric::variable geometric::normalizer::lookup( variable v, label l )
{
   auto p = repr. find( labeledvar(v,l) );
   if( p == repr. end( ))
   {
      variable r = ( nextvar ++ );
      repr. insert( 
               std::pair< labeledvar, variable > ( labeledvar( v, l ), r ));
      return r;
   }
   else
      return p -> second;
}

geometric::varatom 
geometric::normalizer::apply( varatom at, label l )
{
   for( auto p = at. args_begin( ); p != at. args_end( ); ++ p )
      *p = lookup( *p, l );
   return at; 
}

bool
geometric::normalizer::contains( variable v, label l ) const
{
   return repr. find( labeledvar(v,l)) != repr. end( );
}

std::ostream& 
geometric::operator << ( std::ostream& out, const normalizer& n )
{
   out << "Normalizer:\n";
   out << "   next available variable: " << n. nextvar << "\n";
   out << "   {";
   for( auto p = n. repr. begin( ); p != n. repr. end( ); ++ p )
   {
      if( p != n. repr. begin( ))
         out << ", ";
      else
         out << " ";

      out << ( p -> first ) << " => " << ( p -> second );
   }
   out << " }";
   return out;
}


void geometric::unifier::unify( variable v1, label l1, variable v2, label l2 )
{
   labeledvar lv1 = { v1, l1 };
   labeledvar lv2 = { v2, l2 };

   normalize( lv1 );
   normalize( lv2 );
 
   if( lv1 != lv2 )
      repr. insert( std::make_pair( lv1, lv2 )); 
}


void geometric::unifier::unify_arguments( const varatom& at1, label l1,
                                          const varatom& at2, label l2 )
{
   auto a1 = at1. args_cbegin( );
   auto a2 = at2. args_cbegin( );

   while( a1 != at1. args_cend( ))
   {
      unify( *a1, l1, *a2, l2 );
      ++ a1; ++ a2;
   }
}


auto geometric::unifier::apply( normalizer& norm,  
                                variable v, label l ) const -> variable
{
   labeledvar lv{ v, l };
   normalize( lv );
   return norm. lookup( lv.var, lv.lab );
} 


auto geometric::unifier::apply( normalizer& norm,
                                varatom at, label l ) const -> varatom 
{
   for( auto p = at. args_begin( ); p != at. args_end( ); ++ p )
   {
      *p = apply( norm, *p, l );
   }
   return at; 
}

std::ostream& 
geometric::operator << ( std::ostream& out, const unifier& unif )
{
   out << "Unifier:\n";
   out << "   {";
   for( auto p = unif. repr. begin( ); p != unif. repr. end( ); ++ p )
   {
      if( p != unif. repr. begin( ))
         out << ", ";
      else
         out << " ";

      out << ( p -> first ) << " => " << ( p -> second );
   }
   out << " }"; 
   return out;
}


