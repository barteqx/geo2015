

// Written by Hans de Nivelle, September 2006.


#include "tptp.h"
#include <cstdlib>
#include "../partial.h"
#include "../prolog/printingreading.h"


namespace parser
{


tptp::specialsymbols::specialsymbols( ) 
   : tptp_not( prolog::funcpred( "~", 1 )),
     tptp_or( prolog::funcpred( "|", 2 )),
     tptp_nor( prolog::funcpred( "~|", 2 )),
     tptp_and( prolog::funcpred( "&", 2 )),
     tptp_nand( prolog::funcpred( "~&", 2 )),
     tptp_implies( prolog::funcpred( "=>", 2 )),
     tptp_if( prolog::funcpred( "<=", 2 )),
     tptp_equiv( prolog::funcpred( "<=>", 2 )),
     tptp_nequiv( prolog::funcpred( "<~>", 2 )),
     tptp_colon( prolog::funcpred( ":", 2 )),
     tptp_forall( prolog::funcpred( "!", 1 )),
     tptp_exists( prolog::funcpred( "?", 1 )),
     tptp_eq( prolog::funcpred( "=", 2 )),
     tptp_neq( prolog::funcpred( "!=", 2 )), 
     tptp_nil( prolog::funcpred( "[]", 0 )), 
     tptp_cons( prolog::funcpred( ".", 2 )), 
     tptp_dollar( prolog::funcpred( "$", 1 )), 
     tptp_false( prolog::funcpred( "false", 0 )), 
     tptp_true( prolog::funcpred( "true", 0 )), 
     tptp_old_plus( prolog::funcpred( "++", 1 )), 
     tptp_old_minus( prolog::funcpred( "--", 1 ))
   {
   }


bool tptp::specialsymbols::isvariable( prolog::funcpred f ) const 
{
   if( f. getarity( ) != 0 )
      return false;

   const std::string& s = f. getstring( );
   if( s. size( ) == 0 )
      return false;

   if( s[0] >= 'A' && s[0] <= 'Z' )
   {
      for( unsigned int i = 1; i < s. size( ); ++ i )
      {
         bool b =
            ( s[i] >= 'A' && s[i] <= 'Z' ) ||
            ( s[i] >= 'a' && s[i] <= 'z' ) ||
	    ( s[i] >= '0' && s[i] <= '9' ) ||
	    ( s[i] == '_' );
	 if( !b )
            return false;
      }
      return true; 
   }
   else 
      return false;
}



bool tptp::specialsymbols::isfunction_or_predicate( prolog::funcpred f ) const 
{
   const std::string& s = f. getstring( );
   if( s. size( ) == 0 )
      return false;

   if( s[0] >= 'a' && s[0] <= 'z' )
   {
      for( unsigned int i = 1; i < s. size( ); ++ i )
      {
         bool b =
	    ( s[i] >= 'a' && s[i] <= 'z' ) ||
	    ( s[i] >= 'A' && s[i] <= 'Z' ) || 
            ( s[i] >= '0' && s[i] <= '9' ) ||
              s[i] == '_' ;
	 if( !b )
            return false; 
      }
      return true; 
   }

   if( s[0] >= '0' && s[0] <= '9' )
   {
      for( unsigned int i = 1; i < s. size( ); ++ i )
      {
         bool b = ( s[i] >= '0' && s[i] <= '9' );
         if( !b )
            return false;
      }
      return true; 
   }

   for( unsigned int i = 0; i < s. size( ); ++ i )
   {
      if( s[i] != '~' && s[i] != '!'  && s[i] != '#' &&
          s[i] != '$' && s[i] != '^'  && s[i] != '&' &&
	  s[i] != '*' && s[i] != '-'  && s[i] != '+' &&
	  s[i] != '=' && s[i] != '\\' && s[i] != '/' &&
	  s[i] != ':' && s[i] != ';'  && s[i] != '|' &&
	  s[i] != '<' && s[i] != '>'  && s[i] != '?' )
      {
         return false;
      }
   }

   // We forbid the TPTP special symbols:

   if( f == tptp_not ||
       f == tptp_or || f == tptp_nor || 
       f == tptp_and || f == tptp_nand ||
       f == tptp_implies || f == tptp_if || 
       f == tptp_equiv || f == tptp_nequiv || 
       f == tptp_colon || f == tptp_forall || f == tptp_exists || 
       f == tptp_nil || f == tptp_cons || 
       f == tptp_dollar || f == tptp_false || f == tptp_true || 
       f == tptp_old_plus || f == tptp_old_minus )
   {  
      return false;
   }
   return true;
}



bool tptp::specialsymbols::quantifierfree( prolog::expression expr ) const
{
   prolog::funcpred op = expr. getconstructor( );

   if( op == tptp_forall || op == tptp_exists )
      return false;

   for( std::list< prolog::expression > :: const_iterator
           p = expr. subexpressions_begin( );
	   p != expr. subexpressions_end( );
	   ++ p )
   {
      if( ! quantifierfree( *p ))
         return false;
   }
   return true;
}


namespace 
{

   void addvars( const tptp::specialsymbols& symb,
		 prolog::expression expr,
		 std::map< prolog::funcpred, bool > & vars ) 
   {
      prolog::funcpred op = expr. getconstructor( );

      if( symb. isvariable( op ))
         vars [ op ] = true;

      for( std::list< prolog::expression > :: const_iterator
              p = expr. subexpressions_begin( );
	      p != expr. subexpressions_end( );
	      ++ p )
      {
         addvars( symb, *p, vars );  
      }
   }

}


firstorder::formula tptp::universal_closure( 
		             const specialsymbols& symb,
		             const std::map< prolog::funcpred, bool > & vars,
                             firstorder::formula f )
{
   if( vars. size( ) == 0 )
      return f;
   else
   {
      std::list< firstorder::function > varlist;

      for( std::map< prolog::funcpred, bool > :: const_iterator
              p = vars. begin( );
              p != vars. end( );
	      ++ p )
      {
         if( p -> second ) 
         {
            varlist. push_back( firstorder::function( 
			             p -> first. getstring( ), 0 ));
         }
      }

      return firstorder::formula::make_forall( varlist, f );
   }
}






std::map< prolog::funcpred, bool > 
tptp::collectvariables( const specialsymbols& symb, prolog::expression expr )
{
   std::map< prolog::funcpred, bool > result;
   addvars( symb, expr, result );  
   return result;
}



// convert first-order term:

firstorder::term tptp::convert_fot( const specialsymbols& symb,
		                    prolog::expression expr ) 
throw( conversion_error )
{
   prolog::funcpred op = expr. getconstructor( );

   firstorder::function f = firstorder::function( op. getstring( ),
					          op. getarity( ));

   if( ! symb. isvariable( op ))
   {
      if( symb. isfunction_or_predicate( op ))
      {
         // We probably need to produce a function declaration for op:

         f. declare( ); 
      }
      else
         throw conversion_error( expr, "incorrect function" );
   }

   std::list< firstorder::term > subterms;

   for( std::list< prolog::expression > :: const_iterator
           p = expr. subexpressions_begin( );
	   p != expr. subexpressions_end( );
	   ++ p )
   {
      subterms. push_back( convert_fot( symb, *p ));
   }
   return firstorder::term( f, subterms );
}



// convert first-order formula:


firstorder::formula tptp::convert_fof( const specialsymbols& symb,
		                       prolog::expression expr )
throw( conversion_error ) 
{
  
   prolog::funcpred op = expr. getconstructor( ); 

   if( op == symb. tptp_dollar )
   {
      if( expr. subexpressions_begin( ) -> getconstructor( ) == 
		     symb. tptp_false )
         return firstorder::formula::make_false( );

      if( expr. subexpressions_begin( ) -> getconstructor( ) == 
		     symb. tptp_true )
         return firstorder::formula::make_true( );

      throw conversion_error( expr, "unknown constant after dollar" );
   }

   if( op == symb. tptp_not ) 
   {
      return firstorder::formula::make_not( 
		  convert_fof( symb, *expr. subexpressions_begin( ) ));
   }


   if( op == symb. tptp_implies || op == symb. tptp_if || 
       op == symb. tptp_equiv || op == symb. tptp_nequiv )
   {
      std::list< prolog::expression > :: const_iterator 
                  p = expr. subexpressions_begin( );

      firstorder::formula sub1 = convert_fof( symb, * ( p ++ ) );
      firstorder::formula sub2 = convert_fof( symb, * ( p ++ ) );

      if( op == symb. tptp_implies )
         return firstorder::formula::make_implies( sub1, sub2 );
      if( op == symb. tptp_if )
         return firstorder::formula::make_implies( sub2, sub1 );
      if( op == symb. tptp_equiv )
         return firstorder::formula::make_equiv( sub1, sub2 );
      if( op == symb. tptp_nequiv )
         return firstorder::formula::make_not( 
                   firstorder::formula::make_equiv( sub1, sub2 ));

      ASSERT( false ); exit(0); 
   }

   if( op == symb. tptp_or || op == symb. tptp_nor || 
       op == symb. tptp_and || op == symb. tptp_nand )
   {
      std::list< firstorder::formula > sub;
      for( std::list< prolog::expression > :: const_iterator
              p = expr. subexpressions_begin( );
	      p != expr. subexpressions_end( );
	      ++ p )
      {
         sub. push_back( convert_fof( symb, *p ));
      }

      if( op == symb. tptp_or )
         return firstorder::formula::make_or( sub );
      if( op == symb. tptp_nor )
         return firstorder::formula::make_not( 
		     firstorder::formula::make_or( sub ));
      if( op == symb. tptp_and )
         return firstorder::formula::make_and( sub );
      if( op == symb. tptp_nand )
         return firstorder::formula::make_not( 
                     firstorder::formula::make_and( sub ));

      ASSERT( false ); exit(0); 
   }

   if( op == symb. tptp_colon )
   {
      std::list< prolog::expression > :: const_iterator 
	                     p = expr. subexpressions_begin( ); 
      prolog::funcpred quant = ( p -> getconstructor( ));
      
      if( quant != symb. tptp_forall && quant != symb. tptp_exists )
         throw conversion_error( expr, "unknown quantifier" );

      prolog::expression varlist = * ( p -> subexpressions_begin( ));
      ++p;
      firstorder::formula body = convert_fof( symb, *p );

      std::list< firstorder::function > fovars;
      
      while( varlist. getconstructor( ) != symb. tptp_nil )
      {
         if( varlist. getconstructor( ) != symb. tptp_cons )
            throw conversion_error( expr,
			    "incorrect variable list in quantification" );

	 std::list< prolog::expression > :: const_iterator
                               q = varlist. subexpressions_begin( );
	 prolog::funcpred v = ( q ++ ) -> getconstructor( );

	 if( v. getarity( ) != 0 || ! symb. isvariable( v ))
            throw conversion_error( expr,
			"quantified object does not look like a variable" );


	 varlist = *q;
	 fovars. push_back( firstorder::function( v. getstring( ), 0 ));
      }

      if( quant == symb. tptp_forall )
         return firstorder::formula::make_forall( fovars, body );
      if( quant == symb. tptp_exists )
         return firstorder::formula::make_exists( fovars, body );

      ASSERT( false ); exit(0); 
   }

   if( op == symb. tptp_eq || op == symb. tptp_neq )
   {
      std::list< prolog::expression > :: const_iterator
                    p = expr. subexpressions_begin( );
      firstorder::term t1 = convert_fot( symb, * ( p ++ ) );
      firstorder::term t2 = convert_fot( symb, * ( p ++ ) );

      if( op == symb. tptp_eq )
         return firstorder::formula::make_equals( t1, t2 );
      if( op == symb. tptp_neq )
         return firstorder::formula::make_not(
		    firstorder::formula::make_equals( t1, t2 ));
      ASSERT( false ); exit(0);
   }

   if( ! symb. isfunction_or_predicate( op ))
      throw conversion_error( expr, "incorrect predicate" );

   predicate pred = predicate( op. getstring( ), op. getarity( ));

   std::list< firstorder::term > terms;
   for( std::list< prolog::expression > :: const_iterator
           p = expr. subexpressions_begin( );
	   p != expr. subexpressions_end( );
	   ++ p )
   {
      terms. push_back( convert_fot( symb, *p ));
   }
   return firstorder::formula::make_atom( pred, terms );
}



prolog::syntax tptp::syntax( )
{
   prolog::syntax synt; 

   synt. doublequotes = true;

   synt. insert( prolog::syntacticdef( "$", 
                 prolog::syntacticdef::FX, 99 ));

   synt. insert( prolog::syntacticdef( "!",
                 prolog::syntacticdef::FX, 400 ));
   synt. insert( prolog::syntacticdef( "?",
                 prolog::syntacticdef::FX, 400 ));

   synt. insert( prolog::syntacticdef( "=",
                 prolog::syntacticdef::XFX, 405 ));
   synt. insert( prolog::syntacticdef( "!=",
                 prolog::syntacticdef::XFX, 405 ));

   synt. insert( prolog::syntacticdef( "~",
                 prolog::syntacticdef::FY, 450 ));
   synt. insert( prolog::syntacticdef( ":",
                 prolog::syntacticdef::XFY, 450 ));

   synt. insert( prolog::syntacticdef( "|",
                 prolog::syntacticdef::XFY, 502 ));
   synt. insert( prolog::syntacticdef( "~|",
                 prolog::syntacticdef::XFY, 502 ));

   synt. insert( prolog::syntacticdef( "&",
                 prolog::syntacticdef::XFY, 503 ));
   synt. insert( prolog::syntacticdef( "~&",
                 prolog::syntacticdef::XFY, 503 ));

   synt. insert( prolog::syntacticdef( "=>",
                 prolog::syntacticdef::XFY, 504 ));
   synt. insert( prolog::syntacticdef( "<=",
                 prolog::syntacticdef::XFY, 504 ));

   synt. insert( prolog::syntacticdef( "<=>",
                 prolog::syntacticdef::XFY, 505 ));
   synt. insert( prolog::syntacticdef( "<~>",
                 prolog::syntacticdef::XFY, 505 ));

   return synt;
}




std::ostream& tptp::operator << ( std::ostream& stream,
		            const tptp::conversion_error& err ) 
{
   stream << "TPTP conversion error:\n"; 
   stream << err. explanation << "\n";
   stream << "in prolog expression "; 
   stream << err. expr << "\n";
   return stream;
}



// read a TPTP-problem form is. The formulas are inserted into
// seq. Necessary declarations of functions and predicates are inserted
// into ts.


void tptp::readproblem( parser::inputstream& is, 
			firstorder::sequent& seq )
throw( parser::syntaxerror, conversion_error ) 
{
   
   prolog::syntax synt = syntax( );
   specialsymbols symb;

   while( true )
   {
      partial< prolog::expression > expr = prolog::readexpression( is, synt );
     
      if( expr. isundefined( ))
	 return;
            // This is the normal and from the loop.

      prolog::funcpred tptp_fof = prolog::funcpred( "fof", 3 );
      prolog::funcpred tptp_cnf = prolog::funcpred( "cnf", 3 );
         // We do not find these special enough to define them
	 // in tptp::specialsymbols.

      if( expr. contents( ). getconstructor( ) == tptp_fof ||
          expr. contents( ). getconstructor( ) == tptp_cnf ) 
      {
	 std::list< prolog::expression > :: const_iterator
	    p = expr. contents( ). subexpressions_begin( );

	 if( p -> getconstructor( ). getarity( ) != 0 )
            throw conversion_error( *p, "formula name is compound term" );
	 prolog::funcpred name = ( p ++ ) -> getconstructor( );
	
	 if( p -> getconstructor( ). getarity( ) != 0 )
            throw conversion_error( *p, "formula type is compound term" );

	 prolog::funcpred type = ( p ++ ) -> getconstructor( );

	 firstorder::formula form = convert_fof( symb, *p ); 

	 if( expr. contents( ). getconstructor( ) == tptp_cnf )
         {
            // CNF has implicit variables, so we need to collect them
	    // and replace form by the universal closure of form. 
	 
            // We first check that *p is quantifier free.

            if( ! symb. quantifierfree( *p ))
               throw conversion_error( *p, "cnf is not quantifier free" );

	    // Then we make the closure:

            std::map< prolog::funcpred, bool > 
		                 vars = collectvariables( symb, *p );
	    form = universal_closure( symb, vars, form );
         }

	 // We need to insert form at the correct side of the sequent.
	 // Nearly everything goes left, only conjectures go right:

	 if( type == prolog::funcpred( "axiom", 0 ) ||
             type == prolog::funcpred( "hypothesis", 0 ) || 
             type == prolog::funcpred( "lemma", 0 ) ||
	     type == prolog::funcpred( "definition", 0 ) ||
	     type == prolog::funcpred( "negated_conjecture", 0 ))
         {
	    seq. formulas. push_back( form );
         }
	 else
         {
            if( type != prolog::funcpred( "conjecture", 0 ))
               throw conversion_error( expr. contents( ),
			               "unknown formula type" ); 

	    seq. formulas. push_back( firstorder::formula::make_not( form ));
         }
      }
      else
      {
         throw tptp::conversion_error( expr. contents( ),
				       "unknown type of formula" );
      }
   }
}

}


