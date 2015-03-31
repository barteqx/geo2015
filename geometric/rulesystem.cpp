
// Written by Hans de Nivelle, February 2015.

#include "rulesystem.h"
#include "subststack.h"
#include "matching/basics.h"
#include "deduction.h"


std::ostream& geometric::operator << ( std::ostream& out,
                                       const indexedrule& ind )
{
   out << ind.r;

   // In most cases, one probably doesn't want to see those:

   // out << ind. prem << "\n";
   // out << ind. vars << "\n";
   return out;
}


geometric::rulesystem::rulesystem( const std::string& name,
      const std::vector< rule > & initials,
      const std::vector< rule > & simplifiers )
   : name{name}
{
   for( const rule& r : initials )
      ( this -> initials ). push_back(r);

   for( const rule& r : simplifiers )
      ( this -> simplifiers ). push_back(r); 
}


geometric::rulesystem::rulesystem( const std::string& name,
      std::initializer_list<rule> initials,
      std::initializer_list<rule> simplifiers )
   : name{name}
{
   for( const rule& r : initials )
      ( this -> initials ). push_back(r);

   for( const rule& r : simplifiers )
      ( this -> simplifiers ). push_back(r); 
}


bool 
geometric::rulesystem::activecontained( const subst< variable > & subst,
                                        const lemma& lem1, 
                                        const lemma& lem2 )
{
   if( lem1. hasactive( ))
   {
      varatom act1 = subst. apply( lem1. getactive( ));

      for( const varatom& prem : lem1. getpassive( ))
      {
         if( act1.p == prem.p && ( act1.t & prem.t ) == act1.t &&
             equal_arguments( act1, prem ))
         {
            return true;
         }
      }

      if( lem2. hasactive( ))
      {
         const varatom& act2 = lem2. getactive( );
         if( act1.p == act2.p && ( act1.t & act2.t ) == act1.t &&
                equal_arguments( act1, act2 ))
         {
            return true;
         }
      }

      return false; 
   }
   else
      return true; 
}


void geometric::rulesystem::subs_collector::operator( ) (
   const subst<variable> & subst,
   const std::vector< std::pair< variable, variable >> & skipped,
   bool& stop )
{
#if 0 
   std::cout << "found a solution " << subst << "\n"; 
   std::cout << "skipped: ";
   for( auto p = skipped. begin( ); p != skipped. end( ); ++ p )
   {
      std::cout << ( p -> first ) << " == " << ( p -> second ) << "  ";
   }
   std::cout << "\n";
#endif

   if( from. islemma( ) &&
       activecontained( subst, from.getlemma( ), into.getlemma( ) ))
   {
      if( results. empty( ))
      {
          results. push_back( { skipped, subst } ); 
      }
      else
      {
         if( results. back( ). skipped. size( ) > skipped. size( ))
         {
            results. front( ) = { skipped, subst };
         }
      }

      if( results. back( ). skipped. size( ) == 0 )
         stop = true;
   }
}


void geometric::rulesystem::addlearnt( std::list<false_instance> added )
{
#if 1
   std::cout << "adding learnt lemmas:\n";
   for( const auto& p : added )
      std::cout << p << "\n";
   std::cout << "\n";
#endif

   const size_t maxskip = 3;
      // Maximum number of equalities that can be skipped during a
   const size_t maxsol = 2;
      // Maximum number of solutions that we keep. 

   while( added. size( ))
   {
      false_instance& first = added. front( ); 
      
      index::standardsimple< variable > ind;
      for( const varatom& at : first. r. getpassive( ))
         ind. insert( at );

      set<variable> vars = first. r. getfreevariables( );

      std::list<skippedequalities> solutions; 
         // Matchings of rules into first that we hope to find.
         
      subststack<variable> subst;
      std::vector< std::pair< variable, variable >> skipped;
      bool stop = false;
 
      for( auto p = simplifiers. begin( ); 
                p != simplifiers.end( ) && !stop;  
                ++ p )
      {
         if( p -> r. getpassive( ). size( ) < 5 ) 
         {
         subs_collector collector( p -> r, maxsol, solutions, first. r );
         matching::subsumptionmatch( 
                        p -> r. getpassive( ), 0,
                        ind, vars, subst, 
                        skipped, maxskip, stop, collector );
         }
      }  

      for( auto p = learnt. begin( );
                p != learnt.end( ) && !stop;
                ++ p )
      {
         if( p -> r. getpassive( ). size( ) < 5 )
         {
         subs_collector collector( p -> r, maxsol, solutions, first. r );
         matching::subsumptionmatch(
                        p -> r. getpassive( ), 0,
                        ind, vars, subst,
                        skipped, maxskip, stop, collector );
         }
      }

      if( solutions. size( ))
      {
         std::list< false_instance > replacement;
         for( equality eq : solutions. front( ). skipped )
         {
            const element* e1 = first. inst. clookup( eq.v1 );
            const element* e2 = first. inst. clookup( eq.v2 ); 
  
            // One can either process all equalities, or only the 
            // true ones.

            if( *e1 == *e2 || true )
            {
               matched<lemma> lem = { first.r.getlemma( ), first.inst };
               merging m = merging( eq.v1, eq.v2 );
               lem = merge( lem, { m } );

               replacement. push_back(
                  false_instance( 
                     rule( lem.r, mergings( first.r, { m } )),
                     0,
                     lem. inst, first. assumptions ));

            }
         } 
     
         added. pop_front( ); 
         for( auto& repl : replacement )
         {
            added. push_back( std::move( repl )); 
         }
      }
      else
      {
         // No solutions found means that first will be added to
         // the rule system.

         std::cout << "first will be added " << first << "\n";
         first.r. sortpassive( );
         learnt. push_back( indexedrule( first.r )); 

         added. pop_front( ); 
      }


   }
}


std::ostream&
geometric::operator << ( std::ostream& out, rulesystem::equality e )
{
   out << e.v1 << " == " << e.v2;
   return out;
}


std::ostream& 
geometric::operator << ( std::ostream& out, 
                         const rulesystem::skippedequalities& sk )
{
   if( sk.skipped. empty( ))
      out << "a complete subsumption\n";
   else
   {
      out << "a partial subsumption: {";
      for( auto p = sk. skipped. cbegin( ); p != sk. skipped. cend( ); ++ p )
      {
         if( p == sk. skipped. cbegin( ))
            out << " ";
         else
            out << ", "; 
         out << *p;
      }
      out << " }\n";
   }
   return out;
}


std::ostream& 
geometric::operator << ( std::ostream& out, const rulesystem& rs )
{
   out << "RuleSystem " << rs. name << ":\n";
   out << "Initial Rules:\n";
   for( const auto& r : rs. initials )
      out << r << "\n\n";
   out << "number of initial rules = " << rs. initials. size( ) << "\n\n";

   out << "Simplifiers:\n";
   for( const auto& r : rs. simplifiers )
      out << r << "\n\n";
   out << "number of simplifiers = " << rs. simplifiers. size( ) << "\n\n";

   out << "Lemmas:\n";
   for( const auto& r : rs. learnt )
      out << r << "\n\n";
   out << "number of learnt lemmas = " << rs. learnt. size( ) << "\n\n";

   return out;
}


