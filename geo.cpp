
#include <fstream>

#include "analysis/dependencies.h"
#include "analysis/dependency_system.h"

#include "firstorder/nnf.h"
#include "firstorder/simplifications.h"
#include "firstorder/replacement.h"
#include "firstorder/antiskolemization.h"
#include "firstorder/factor.h"
#include "firstorder/geometric.h"

#include "statistics/distribution.h"
#include "statistics/timer.h"
#include "statistics/reporter.h"

#include "firstorder/testing.h"

#include "swll/searchstate.h"
#include "swll/lemmasystem.h"

#include "parser/parser.h"
#include "parser/token.h"
#include "parser/errors.h"
#include "parser/tptp.h"

#include "simplification.h"

#include "varmatching.h"

#include "varset.h"

#include "search.h" 
#include "false_instance.h"

#include "rulesystem.h"
#include "model.h"

#include "normalizer.h"

#include "varsubst.h"
#include "element.h" 
#include "predicate.h"
#include "groundatom.h" 
#include "varatom.h"
#include "forwardreasoning.h"
#include "partial.h"
#include "application.h"

#include "predicate.stat"
#include "firstorder/function.stat"
#include "prolog/funcpred.stat"

#include <cstdlib>
#include <iostream>


struct parameters
{
   bool tptp_input;
   bool tptp_output;
      // tptp_output is not yet supported.

   bool nonempty;
      // Insist on non-empty models. With nonempty = false,
      // the formula forall x. p(x) -> exists x. p(x) is not provable.

   std::string inputfilename;
      // File from which input is read. If inputfilename = empty, input
      // will be read from standard input. 
   
   std::ifstream* inputstream;


   explicit parameters( const std::list< std::string > & pars )
      : tptp_input( false ),
        tptp_output( false ),
	nonempty( false ),
	inputfilename( "" ),
	inputstream(0)  
   {

      if( pars. size( ) == 0 )
      {
         std::cout << "ERROR: There should be at least one parameter\n\n";
	 exit(0);
      }

      std::list< std::string > :: const_iterator p = pars. begin( ); 
      ++ p;

      while( p != pars. end( ))
      {
         if( *p == "-tptp_input" )
         {
            tptp_input = true;
	    ++ p; 
            goto recognized;
         }

	 if( *p == "-tptp_output" )
         {
            tptp_output = true;
	    ++ p;
	    goto recognized;
         }

	 if( *p == "-nonempty" )
         {
            nonempty = true;
	    ++ p;
	    goto recognized;
         }

	 if( *p == "-inputfile" ) 
         {
            ++ p;
	    if( p == pars. end( ))
            {
               std::cout << "ERROR: expecting filename after -inputfile\n\n";
	       exit(0);
            }

	    if( inputfilename. size( ))
            {
               std::cout << "ERROR: multiple filenames\n\n";
	       exit(0);
            }

	    inputfilename = *(p++);
	    inputstream = new std::ifstream( inputfilename. c_str( ));
            if( ! ( *inputstream ))
            {
               std::cout << "ERROR: ";
	       std::cout << "could not open inputfile ";
	       std::cout << inputfilename<< "\n\n";
	       exit(0);
            }

            goto recognized;
         }
      
	 std::cout << "ERROR: unrecognized parameter: " << *(p++);
	 std::cout << "\n";
	 exit(0);

      recognized:
	 ;
      }
   }


   ~parameters( )
   {
      delete inputstream;
   }


};


std::ostream& operator << ( std::ostream& stream , const parameters& pars )
{
   stream << "Parameters:\n\n";

   stream << "tptp_input =     " << pars. tptp_input << "\n";
   stream << "tptp_output =    " << pars. tptp_output << "\n";
   stream << "nonempty =       " << pars. nonempty << "\n";
   stream << "inputfile =      " << ( pars. inputfilename. size( ) ?
	                               pars. inputfilename :
				       "(stdin)" ) << "\n";
   stream << "\n";
   return stream;
}

#include "geometric/signpred.h"
#include "geometric/truthval.h" 
#include "geometric/variable.h"
#include "geometric/element.h"
#include "geometric/atom.h"
#include "geometric/rulebase.h"
#include "geometric/rule.h"
#include "geometric/rulesystem.h"
#include "geometric/range.h"
#include "geometric/unification.h"
#include "geometric/substqueue.h"
#include "geometric/searchstate.h"
#include "geometric/interpretation.h" 
#include "geometric/set.h"
#include "geometric/examples.h"
#include "geometric/permutation.h"
#include "geometric/permutationgenerator.h"
#include "geometric/subst.h"
#include "geometric/matched.h"

#include "geometric/subststack.h"
#include "geometric/false_instance.h"
#include "geometric/best_false_instances.h"
#include "geometric/deduction.h"

#include "geometric/matching/range_restriction.h"
#include "geometric/matching/basics.h"
#include "geometric/atom_hasheq.h"

#include "geometric/index/simple.h"


int main( int nrargs, char* args [] )
{

   geometric::variable v0;
   auto v1 = v0. next( ); 
   auto v2 = v1. next( ); 
   auto v3 = v2. next( ); 
   auto v4 = v3. next( );
   auto v5 = v4. next( ); 
   auto v6 = v5. next( );
   auto v7 = v6. next( ); 
   auto v8 = v7. next( );
   auto v9 = v8. next( ); 
   auto v10 = v9. next( ); 
 
   geometric::element e0;
   auto e1 = e0. next( );
   auto e2 = e1. next( ); 
   auto e3 = e2. next( ); 
   auto e4 = e3. next( ); 
   auto e5 = e4. next( ); 
   auto e6 = e5. next( ); 
   auto e7 = e6. next( ); 
   auto e8 = e7. next( ); 
   auto e9 = e8. next( ); 
   auto e10 = e9. next( );

   geometric::signature sig;
   auto P = sig. lookup( { "P" }, 1 );
   auto Q = sig. lookup( { "Q" }, 1 );
   auto R = sig. lookup( { "R" }, 1 );

   auto A = sig. lookup( { "A" }, 1 ); 
   auto B = sig. lookup( { "B" }, 1 );
   auto C = sig. lookup( { "C" }, 1 );
   auto D = sig. lookup( { "D" }, 2 );

   auto p3 = sig. lookup( { "p3" }, 3 );
   auto p4 = sig. lookup( { "p4" }, 4 ); 
   auto q4 = sig. lookup( { "q4" }, 4 );

   auto S = sig. lookup( { "S" }, 2 );
 
   while( true )
   {

      geometric::searchstate state( 
         geometric::permutationgenerator::strategy::random,
         geometric::demod( sig ), 
       { // geometric::groundatom( sig.create, geometric::truthval::T, {e0} ),
         // geometric::groundatom( sig.create, geometric::truthval::T, {e1} ),
         // geometric::groundatom( sig.create, geometric::truthval::T, {e2} ),
         // geometric::groundatom( p3, geometric::truthval::T, {e0,e1,e2} ),
         // geometric::groundatom( A, geometric::truthval::E, { e0 } ),
         // geometric::groundatom( A, geometric::truthval::T, { e1 } ),
         // geometric::groundatom( B, geometric::truthval::T, { e1 } ),
         // geometric::groundatom( C, geometric::truthval::E, { e2 } ), 
         // geometric::groundatom( A, geometric::truthval::E, { e0 } ) } ); 
         // geometric::groundatom( C, geometric::truthval::E, { e0 } ) 
       } );

#if 0
      geometric::lemma into( 
         { geometric::varatom( A, geometric::truthval::FE, { v2 } ), 
           geometric::varatom( A, geometric::truthval::F, { v3 } ),
           geometric::varatom( B, geometric::truthval::FT, { v0 } ),
           geometric::varatom( B, geometric::truthval::ET, { v3 } ), 
           geometric::varatom( S, geometric::truthval::F, { v2, v6 } ),
           geometric::varatom( S, geometric::truthval::F, { v3, v7 } ),
           geometric::varatom( sig. equality, geometric::truthval::T,{v6,v7} ),
           geometric::varatom( B, geometric::truthval::F, { v4 } ),
           geometric::varatom( sig. equality, geometric::truthval::T, {v3,v4} ),
           geometric::varatom( p3, geometric::truthval::T, {v2,v1,v0} ) } );

      geometric::subst< geometric::element > ss
         ( {{ v0, e0 }, { v1, e0 }, { v2, e1 }, { v3, e1 },
            { v4, e4 }, { v6, e6 }, { v7, e6 }} );
     
      geometric::rulesystem rs = geometric::reflexivity( sig ); 
      std::cout << rs << "\n";

      rs. addlearnt( { geometric::false_instance( geometric::rule(into), 0, 
                   ss, { } ) } );

      std::cout << "final " << rs << "\n";
#endif

      bool b = state. search( 60*60*24*7 );

      std::cout << state << "\n";

      if(b)
         std::cout << "found model\n";
      else
         std::cout << "found refutation\n";

      return 0;
   }


   std::cout << "hallo\n\n";

   
   std::list< std::string > pppp;
   for( unsigned int i = 0; i < nrargs; ++ i )
   {
      pppp. push_back( args [i] );
   }

   parameters pars = parameters( pppp );

   std::cout << "\n";
   std::cout << pars << "\n";

   parser::inputstream is = 
      ( pars. inputfilename. size( ) ? 
	      parser::inputstream( *( pars. inputstream ),
			           pars. inputfilename ) :
	      parser::inputstream( )
      );



   model m;
   rulesystem rs;
   firstorder::sequent fol;


   if( pars. tptp_input )
   {
      try
      {
         parser::tptp::readproblem( is, fol );
      }
      catch( parser::syntaxerror s )
      {
         std::cout << "ERROR: ";
	 std::cout << s << "\n";
	 return 0;
      }
      catch( parser::tptp::conversion_error c )
      {
         std::cout << "ERROR: ";
	 std::cout << c << "\n";
	 return 0;
      }
   }
   else
   {
      try
      {
         readproblem( is, m, rs, fol );
      }
      catch( parser::syntaxerror s )
      {
         std::cout << "ERROR: "; 
         std::cout << s << "\n\n";
         return 0; 
      }
   }

   fol. collectusedidentifiers( );

   std::cout << m << "\n";
   std::cout << rs << "\n"; 

   firstorder::simplifications::flatten( fol ); 

   if( fol. formulas. size( ))
   {

      firstorder::simplifications::flatten( fol );
      std::cout << "-----------------------------------------------------\n";
      std::cout << "Input:\n\n";
      std::cout << fol << "\n";


      firstorder::nnf::convert_to_nnf( fol );
      firstorder::simplifications::flatten( fol ); 
      std::cout << "-----------------------------------------------------\n"; 
      std::cout << "NNF:\n\n";
      std::cout << fol << "\n";


      firstorder::simplifications::fully_simplify( fol );
      std::cout << "-----------------------------------------------------\n";
      std::cout << "Full simplification:\n\n";
      std::cout << fol << "\n";


      firstorder::antiskolemization::antiskolemize( fol );
      firstorder::simplifications::flatten( fol );
      std::cout << "-----------------------------------------------------\n";
      std::cout << "Anti-Skolemization:\n\n";
      std::cout << fol << "\n"; 


      firstorder::factor::convert_to_cuden( fol );
      firstorder::simplifications::flatten( fol );
      std::cout << "-----------------------------------------------------\n";
      std::cout << "Factored normal form (CUDEN):\n\n";
      std::cout << fol << "\n";


      firstorder::geometric::addtherules( fol, rs );

      std::cout << "-----------------------------------------------------\n";
      std::cout << "Rulesystem after conversion:\n\n";
      std::cout << rs << "\n";

   }

   std::cout << "\n";
   std::cout << "predicates / arities\n\n";

   predicate::printtable( std::cout );

#if 0
   application::test_swll_match( m, rs );
   return 0;
#endif
#if 0
   simplification::test_swll_subsumption( rs );
   return 0;
#endif

   analysis::dependency_system ds =
      analysis::dependency_system::computedependencies(rs); 

   std::cout << "-------------------------------------------\n";
   std::cout << ds << "\n";

   {
      // We add the inductive axioms arising from the dependencies
      // to the rulesystem. 

      std::vector< predicate > predicates_base;
      std::vector< variable > arguments_base; 
    
      std::list< rule > 
         inductive = ds. axioms( predicates_base, arguments_base );

      std::vector< rulesystem::binary_step > deriv;

      for( std::list< rule > :: const_iterator 
              it = inductive. begin( );
              it != inductive. end( );
              ++ it )
      {
         rs. addrule( *it, deriv, true );
      }
   }


   std::cout << "-----------------------------------------------------\n";
   std::cout << "Rule system with inductive axioms:\n\n";
   std::cout << rs << "\n";

   
   if( pars. nonempty )
   {
      if( m. nr_elements( ) == 0 )
      {
         // In that case, we insert one element into the model:

         element e0;
	 std::vector< element > args;
	 args. push_back( e0 );

	 std::vector< unsigned int > parents;

         m. addatom( groundatom( predicate::create, args ), 9999999, parents );
      }
   }

#if 0 
   std::cout << rs <<  "\n\n";

   variable v0;
   variable v1 = v0; v1 ++ ;
   variable v2 = v1; v2 ++ ;
   variable v3 = v2; v3 ++ ;
   variable v4 = v3; v4 ++ ;
   variable v5 = v4; v5 ++ ;
   variable v6 = v5; v6 ++ ;
   variable v7 = v6; v7 ++ ;
   variable v8 = v7; v8 ++ ;
   variable v9 = v8; v9 ++ ;
   variable v10 = v9; v10 ++ ;

   predicate pp = predicate::predicate( "p", 1 ); 
   predicate ss = predicate::predicate( "s", 2 ); 
   predicate aa = predicate::predicate( "a", 1 );
   predicate bb = predicate::predicate( "b", 1 );
   predicate qq = predicate::predicate( "q", 4 );

   std::vector< predicate > predicates_base;
   std::vector< variable > variables_base; 

   rule r( predicates_base, variables_base ); 

   std::vector< variable > arguments;
   arguments. push_back( v0 );
   r. addpremisse( varatom::varatom( aa, arguments ));

   arguments. clear( );
   arguments. push_back( v0 );
   arguments. push_back( v1 );
   r. addpremisse( varatom::varatom( ss, arguments ));

   arguments. clear( );
   arguments. push_back( v2 );
   r. addpremisse( varatom::varatom( bb, arguments ));

   arguments. clear( );
   arguments. push_back( v2 );
   arguments. push_back( v3 );
   r. addpremisse( varatom::varatom( ss, arguments ));

   arguments. clear( );
   arguments. push_back( v1 );
   arguments. push_back( v3 );
   r. addpremisse( varatom::varatom( predicate::diseq, arguments ));

   std::cout << r << "\n";

   analysis::dependencies dep1;
   
   dep1. addequation( 4, 5 );
   dep1. addequation( 5, 4 );

   std::list< unsigned int > arg;
   arg. push_back( 5 );
   arg. push_back( 4 );

   dep1. addskolem( 4, qq, arg );
   dep1. addskolem( 5, qq, arg );
   dep1. addskolem( 6, qq, arg );

   
   rule::prem_iterator p1 = r. prems_begin( );
   ++ p1;
   ++ p1;
   ++ p1;


   std::cout << dep1 << "\n";
   std::cout << *p1 << "\n";

   // dep1. project( r. prems_begin( ), r. prems_end( ), *p1 );

   std::cout << dep1 << "\n";

   dep1. transfertoconclusion( r. prems_begin( ), r. prems_end( ), *p1 );

   std::cout << dep1 << "\n";

   return 0; 
#endif

   std::cout << "\n"; 
   std::cout << "-----------------------------------------------------\n";
   std::cout << "Starting Search:\n\n\n\n";

   std::list< unsigned int > ref = search::extendstomodel( m, rs );

   // In case we found a proof, insist that the original model is restored:

   if( ref. size( ))
   { 
      if( pars. nonempty )
      {
         ASSERT( m. nr_elements( ) == 1 && m. nr_atoms( ) == 1 );
      }
      else
      {
         ASSERT( m. nr_elements( ) == 0 && m. nr_atoms( ) == 0 );
      }
   }

   if( ref. size( ))
   {
      std::cout << "\n"; 
      std::cout << "START-OF-FINAL-RULESYSTEM\n"; 
      std::cout << rs << "\n\n";
      std::cout << "END-OF-FINAL-RULESYSTEM\n\n";

      std::cout << "Rules that have been used more than once:\n"; 
      rs. print( std::cout, 2, 0, false );
      std::cout << "\n\n";

      // It is unlikely that we find more than one proof, but it is
      // possible: 
    
      for( std::list< unsigned int > :: const_iterator
              p = ref. begin( );
              p != ref. end( );
              ++ p )
      { 
         std::cout << "START-OF-PROOF\n"; 
         rs. printderivation( std::cout, *p );
         std::cout << "END-OF-PROOF\n\n"; 
         std::cout << "could not extend to model in ";
         std::cout << m. closure_counter << " attempts\n\n";
      }
   }
   else 
   {
      std::cout << "\n"; 
      std::cout << "START-OF-MODEL\n\n";
      std::cout << m << "\n";
      std::cout << "END-OF-MODEL\n\n"; 
   }

   return 0; 
}


