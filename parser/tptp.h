
// Code written by Hans de Nivelle, September 2006.


#ifndef PARSER_TPTP_INCLUDED
#define PARSER_TPTP_INCLUDED    1


#include "../prolog/syntax.h"
#include "../prolog/expression.h"
#include "../prolog/funcpred.h"


#include "../firstorder/sequent.h"

#include "inputstream.h"
#include "errors.h"



namespace parser
{

   namespace tptp
   {

      struct conversion_error 
      {
         prolog::expression expr;
         std::string explanation;

         conversion_error( prolog::expression expr, 
                           const std::string& explanation )
            : expr( expr ), explanation( explanation )
         {
         }

      };

   
      std::ostream& operator << ( std::ostream& , const conversion_error& );


      prolog::syntax syntax( );
         // Produce a syntax that contains the operator definitions
         // of TPTP.



      struct specialsymbols 
      {
         prolog::funcpred tptp_not;

	 prolog::funcpred tptp_or;
	 prolog::funcpred tptp_nor;

	 prolog::funcpred tptp_and;
	 prolog::funcpred tptp_nand;

	 prolog::funcpred tptp_implies;
	 prolog::funcpred tptp_if; 

	 prolog::funcpred tptp_equiv;
	 prolog::funcpred tptp_nequiv;

	 prolog::funcpred tptp_colon;
	 prolog::funcpred tptp_forall;
	 prolog::funcpred tptp_exists;

	 prolog::funcpred tptp_eq;
	 prolog::funcpred tptp_neq;

	 prolog::funcpred tptp_nil;
	 prolog::funcpred tptp_cons;
            // These are special because they are used in quantifications.

	 prolog::funcpred tptp_dollar;
	 prolog::funcpred tptp_false;
	 prolog::funcpred tptp_true;

	 prolog::funcpred tptp_old_plus;
	 prolog::funcpred tptp_old_minus;

         specialsymbols( );

         bool isvariable( prolog::funcpred f ) const;
            // True if f looks like a TPTP-variable.

         bool isfunction_or_predicate( prolog::funcpred f ) const;
            // True if f looks like user defined predicate or function. 
	    // Maybe we are somewhat too liberal here, but we wanted
	    // to be nice to the users. 

         bool quantifierfree( prolog::expression expr ) const;
            // True if expr is quantifier free. 
      };


      std::map< prolog::funcpred, bool > 
      collectvariables( const specialsymbols& symb, prolog::expression expr );
         // Collect the variables from an expression. What qualifies as 
         // variable is decided by symb. isvariable( ). 


      firstorder::formula universal_closure( 
		              const specialsymbols& symb, 
		              const std::map< prolog::funcpred, bool > & vars,
		              firstorder::formula f );
         // Universally close f under the variables in vars.

      firstorder::term convert_fot( const specialsymbols& symb,
		                    prolog::expression expr )
      throw( conversion_error );
         // Convert a first-order term. We also declare all
         // non-variable symbols as functions. 


      firstorder::formula convert_fof( const specialsymbols& symb,
		                       prolog::expression expr )  
      throw( conversion_error );
         // Convert a first-order formula,
         // and declare the function/predicate symbols 
         // that occur in it. 



      void readproblem( inputstream& stream,
		        firstorder::sequent& seq )
      throw( parser::syntaxerror, conversion_error );

   }

}



#endif



