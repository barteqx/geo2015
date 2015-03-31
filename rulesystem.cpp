

// Code written by Hans de Nivelle, November 2005.


#include "rulesystem.h"
#include "simplification.h"
#include <cstdlib>


unsigned int rulesystem::addrule( rule r, 
		                  const std::vector< binary_step > & deriv,
				  bool inductive ) 
{
   ASSERT( r. iswellformed( ));

   unsigned int deriv1 = derivation. size( );
   for( std::vector< binary_step > :: const_iterator 
           p = deriv. begin( );
           p != deriv. end( );
	   ++ p )
   {
      derivation. push_back( *p );
   }
   unsigned int deriv2 = derivation. size( );

   rule r_copy = r. clone_into( predicates_base, arguments_base );

   unsigned int nr = rules. size( );

   rules. push_back( rule_in_system( r_copy, deriv1, deriv2, inductive ));

   return nr; 
}


void rulesystem::updatestatistics( const std::vector< binary_step > & deriv,
		                   unsigned int closure_counter )
{
   for( std::vector< binary_step > :: const_iterator
           p = deriv. begin( );
	   p != deriv. end( );
	   ++ p )
   {
      type_of_step step = p -> derivedwith; 
      unsigned int nr = p -> otherparent;
      ASSERT( nr < nr_rules( ));

      switch( step )
      {
      case step_startwith:
      case step_disjunctive_res:
      case step_existential_res:
         ++ rules [ nr ]. nr_forward;
         rules [ nr ]. last_forward = closure_counter;
	 break;
      case step_equality_simp:
      case step_existential_simp:
      case step_disjunctive_simp:
         ++ rules [ nr ]. nr_simpl;
	 rules [ nr ]. last_simpl = closure_counter;
	 break; 
      }
   }
}


namespace
{
   // Lifetime based on nr premisses. It is the lifetime per
   // application. For every application, the rule earns
   // a lifetime. 

   unsigned int lifetime( unsigned int nrprem )
   {
      if( nrprem < 100 )
         return 100000;
      if( nrprem < 200 )
         return 500;
      if( nrprem < 300 )
         return 100;
      return 100; 
   }
}


void rulesystem::print( std::ostream& stream,
		        unsigned int nr_forward, unsigned int nr_simpl,
			bool inderivation ) const 
{

   unsigned int nr = 0; 
   for( std::vector< rulesystem::rule_in_system > :: const_iterator 
	   r = rules. begin( );
           r != rules. end( );
	   ++ r, ++ nr ) 
   {
      if( rules [ nr ]. nr_forward >= nr_forward &&
          rules [ nr ]. nr_simpl >= nr_simpl &&
	  rules [ nr ]. inderivation >= inderivation )
      {
         stream << "--------------------------------------\n";
         stream << nr << ": "; 

         for( rulesystem::derivation_iterator
                 p = derivation_begin( nr );
	         p != derivation_end( nr );
	         ++ p )
         {
            if( p != derivation_begin( nr ))
               stream << ",";

            switch( p -> derivedwith )
            {
            case rulesystem::step_startwith:
               stream << "Pick("; 
	       break; 

            case rulesystem::step_disjunctive_res:
	       stream << "Disj(";
	       break;
            case rulesystem::step_existential_res: 
               stream << "Exist("; 
	       break; 

            case rulesystem::step_equality_simp:
	       stream << "eq(";
	       break; 
            case rulesystem::step_existential_simp:
               stream << "exist(";
	       break;
            case rulesystem::step_disjunctive_simp:
	       stream << "disj(";
	       break;

            default:
               ASSERT( false );
	       exit(0);
            }
            stream << p -> otherparent;
            stream << ")";
         }
         stream << "\n"; 

         stream << "   (";
	 stream << "nrappl = " << r -> nr_forward << "/";
	 stream << r -> nr_simpl << ", "; 
	 stream << "lastappl = " << r -> last_forward << "/";
	 stream << r -> last_simpl << ") "; 
	 stream << "   "; 

         if( r -> deleted )
            stream << "(deleted";
         else
            stream << "(active";

	 if( r -> inductive ) 
            stream << ",inductive) ";
	 else
            stream << ") "; 

	 stream << "\n"; 
	 stream << "      "; 
         stream << ( r -> r ) << "\n"; 
      }
   }
}


void rulesystem::printderivation( std::ostream& stream,
		                  unsigned int rulenumber ) const 
{
   ASSERT( rulenumber < nr_rules( ));

   for( unsigned int nr = 0; nr < nr_rules( ); ++ nr )
      rules [ nr ]. inderivation = false;

   rules [ rulenumber ]. inderivation = true;

   for( unsigned int nr = nr_rules( ); nr != 0; )
   {
      -- nr;

      if( rules [ nr ]. inderivation )
      {
         for( rulesystem::derivation_iterator
	         p = derivation_begin( nr );
	         p != derivation_end( nr );
	         ++ p )
         {
	    if( p -> derivedwith == step_startwith ||
                p -> derivedwith == step_disjunctive_res ||
                p -> derivedwith == step_existential_res )
            {
	       unsigned int pppp = p -> otherparent;
	       ASSERT( pppp < nr );
	       rules [ pppp ]. inderivation = true;
            }
         }
      }
   }

   print( stream, 0, 0, true );
}


bool rulesystem::canbedeleted( unsigned int nr ) const
{
   if( rules [nr]. deleted || rules [nr]. inductive )
      return false; 
  
   if( ! rules [nr]. r. isdisjunctive( ) || 
       rules [nr]. r. nr_conclusions( ) != 0 )
   {
      return false;
   }

   if( derivation_begin( nr ) == derivation_end( nr ))
      return false;

   return true;
}


int rulesystem::comparerules( unsigned nr1, unsigned int nr2 )
{
   unsigned int p1 = rules [ nr1 ]. r. nr_premisses( );
   unsigned int p2 = rules [ nr2 ]. r. nr_premisses( );
   if( p1 < p2 )
      return -1;
   if( p1 > p2 )
      return 1;

   unsigned int v1 = rules [ nr1 ]. r. nr_variables( );
   unsigned int v2 = rules [ nr2 ]. r. nr_variables( );
   if( v1 < v2 )
      return 1;
   if( v1 > v2 )
      return -1;

   if( nr1 < nr2 )
      return -1;
   if( nr1 > nr2 )
      return 1;

   return 0;
}


void rulesystem::deleteheaviest( unsigned int closure_counter )
{
   // We first count the active, non-inductive rules.

   unsigned int nr_candidates = 0;
   for( unsigned int nr = 0; nr < nr_rules( ); ++ nr )
   {
      if( canbedeleted( nr ))
         ++ nr_candidates; 
   }

   // We currently delete one third of the candiates. 

   for( unsigned int i = 0; i < nr_candidates / 3; ++ i ) 
   {
      unsigned int worst = 0;
      while( ! canbedeleted( worst ))
         ++ worst;  

      for( unsigned nr = worst + 1; nr < nr_rules( ); ++ nr )
      {
         if( canbedeleted( nr ))
         {
            if( comparerules( worst, nr ) == -1 )
               worst = nr;
         }
      }

      std::cout << "Deleting " << worst << ":\n";
      std::cout << rules [ worst ]. r << "\n";
      rules [ worst ]. deleted = true;
   }
}


std::ostream& operator << ( std::ostream& stream, const rulesystem& rs )
{
   stream << "Rulesystem:\n\n"; 
   rs. print( stream, 0, 0, false );
   return stream;
}


