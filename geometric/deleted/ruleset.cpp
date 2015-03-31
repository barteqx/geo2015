
// written by Hans de Nivelle, June 2014.

#include "ruleset.h"

std::ostream& geometric::operator << ( std::ostream& out, rule_use r )
{
   switch(r)
   {
   case rule_use::disj_res_active:
      out << "disj_act"; return out;
   case rule_use::disj_res_passive:
      out << "disj_pass"; return out;

   case rule_use::ext_res_active:
      out << "ext_act"; return out; 
   case rule_use::ext_res_passive:
      out << "ext_pass"; return out;

   case rule_use::copy:
      out << "copy"; return out;
   case rule_use::inst:
      out << "inst"; return out;
   }
   throw std::runtime_error( "fall through with rule_use" ); 
}


std::ostream& geometric::operator << ( std::ostream& out, parent p )
{
   out << p.u << '(' << p.n << ')';
   return out;
}


geometric::annotated_rule::annotated_rule( unsigned int number )
   : number{number},
     r{ {}, {} }  
{ }

geometric::annotated_rule::annotated_rule( 
   unsigned int number, rule&& r )
   : number{number},
     r{ std::move(r) } 
{ }
 
geometric::annotated_rule::annotated_rule(
   unsigned int number, rule&& r, std::vector< parent > && p )
   : number{number},
     r{ std::move(r) },
     parents{ std::move(p) }
{ }

 
std::ostream& geometric::operator << ( 
        std::ostream& out, const geometric::annotated_rule& ar )
{
   out << ar. number << ":   ";
   out << "active = " << ar. active << ", ";
   out << "applied = " << ar. applications << ", ";
   out << "simplified = " << ar. simpl << "\n";

   if( ar. parents. size( ))
   {
      out << "   parents: ";
      for( auto p = ar. parents. begin( ); p != ar. parents. end( ); ++ p )
      {
         if( p != ar. parents. begin( ))
            out << ", ";
         out << *p;
      }
      out << "\n";
   }
   out << "      " << ar. r << "\n";

   return out;
}


unsigned int geometric::ruleset::findindex( unsigned int number ) const 
{
   if( rules. size( ) == 0 || number < rules [0]. number )
      return 0;

   unsigned int i0 = 0;
   unsigned int i1 = rules. size( );

   while( i0 + 1 < i1 )
   {
      unsigned int i = ( i0 + i1 ) / 2;
      if( rules [i]. number <= number )
         i0 = i;
      else
         i1 = i;
   }

   return i1;
}


geometric::ruleset::iterator
geometric::ruleset::insert( annotated_rule&& ar )
{
   unsigned int i = findindex( ar. number ); 
   if( i != 0 && rules [ i - 1 ]. number == ar. number )
      throw insert_exists( ); 

   std::vector< annotated_rule > :: iterator 
      p = rules. begin( ) + i;

   return rules. insert( p, std::move(ar) );
}


geometric::ruleset::iterator 
geometric::ruleset::find( unsigned int nr )
{
   unsigned int i = findindex( nr );
   if( i == 0 || rules [ i - 1 ]. number != nr )
      return rules. end( );
   else
      return rules. begin( ) + ( i - 1 );
}


geometric::ruleset::const_iterator 
geometric::ruleset::find( unsigned int nr ) const
{
   unsigned int i = findindex( nr );
   if( i == 0 || rules [ i - 1 ]. number != nr )
      return rules. cend( );
   else
      return rules. cbegin( ) + ( i - 1 );
}


geometric::ruleset::iterator
geometric::ruleset::moveto( iterator it, ruleset& rs )
{
   if( &rs != this )
   {
      iterator res = rs. insert( std::move(*it) );
      rules. erase( it );
      return res; 
   }
   else
      return it; // Wsyo tak budyet kak bywalo. 
}


std::ostream& geometric::operator << ( 
       std::ostream& out, const geometric::ruleset& rs )
{
   for( auto p = rs. begin( ); p != rs. end( ); ++ p )
   {
      out << "---------------------------------------------------" <<
             "------------------------\n";
      out << *p;
   }
   return out;
}


