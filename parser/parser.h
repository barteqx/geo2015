

// Written June 2006 by Hans de Nivelle. 


#ifndef PARSER_PARSER_INCLUDED
#define PARSER_PARSER_INCLUDED   1


#include "../variable.h"
#include "../element.h"
#include "../varatom.h"
#include "../groundatom.h"
#include "../rule.h"
#include "../rulesystem.h"
#include "../model.h"
#include "../firstorder/sequent.h"


#include "inputstream.h"
#include "errors.h"
#include "token.h"


namespace parser
{

   bool startidentifier( char c );
      // True if identifier can start with character c.

   bool inidentifier( char c );
      // True if c can occur in the middle 
      // or at the end of an identifier. 

   bool isdigit( char c ); 
      // True if c is among 0,1,2,3,4,5,6,7,8,9. 

   std::string converttostring( unsigned int x );


   unsigned int make_unsigned( std::string::const_iterator p1, 
		               std::string::const_iterator p2,
		               const inputstream& mess ) 
      throw( syntaxerror ); 

   token readtoken( inputstream& stream ) throw( syntaxerror ); 


   element make_element( const std::string& s, 
		         const inputstream& mess ) throw( syntaxerror );
  
   variable make_variable( const std::string& s,
		           const inputstream& mess ) throw( syntaxerror );

      // The functions don't read from the inputstream. The inputstream
      // is used only for appending a line number to the error messages.


   groundatom readgroundatom( inputstream& stream,
		              token& next ) throw( syntaxerror );
      // Reads a ground atom. The ground atom remains valid only
      // until a new groundatom is read. 

   varatom readvaratom( inputstream& stream,
		        token& next ) throw( syntaxerror );
      // Reads a variable atom. The varatoms remains valid only
      // until a new varatom is read. 


   rule readrule( inputstream& stream,
		  token& next ) throw( syntaxerror );
      // Reads a rule. The rule remains valid only until the
      // next rule is read. 
      // We read until we encounter a dot, but not after the 
      // dot. This is better for interactive use. 

 

   void readfoargs( std::list< firstorder::term > & args, 
		    inputstream& stream,
		    token& next ) throw( syntaxerror );
      // Read the arguments of an atom or term.
      // It can be either nothing, (), or ( a1, ..., an ). 
 
   firstorder::term readfoterm( inputstream& stream,
		                token& next ) throw( syntaxerror );
      // Reads a first-order term. 

   
   firstorder::formula readfoatom( inputstream& stream,
		                   token& next ) throw( syntaxerror );
      // Reads a first-order atom. 
      // A complete formula between parentheses can also act as atom.


   firstorder::formula readquantifiedform( inputstream& stream,
		                           token& next ) throw( syntaxerror );

   firstorder::formula readfoform( inputstream& stream,
		                   token& next ) throw( syntaxerror );
      // Read a first-order formula, until a comma, point, or
      // closing parenthesis is reached. The comma, point, or
      // closing parenthesis is not read. 

   firstorder::formula readfoform( token op1,
		                   inputstream& stream,
				   token& next ) throw( syntaxerror );
      // Read a first-order formula, until a comma, point, closing
      // parenthesis, or an operator that looses the attraction
      // duel with op1, is encountered. 

   void readpredicatedeclaration( inputstream& stream,
		                  token& next ) throw( syntaxerror );
   void readfunctiondeclaration( inputstream& stream,
		                 token& next ) throw( syntaxerror );
   
      // Reads a declaration. The declaration is not returned, 
      // but added to the predicate table, or 
      // added as an explicit declaration to the function table. 
      // As with rules, we don't read behind the final dot. 

   bool isheader( const token& t ); 
      // True if t is rulesystem, model, firstorder, predicates,
      // functions, end or EOF. 

   void readproblem( inputstream& stream,
		     model& m, rulesystem& r,
		     firstorder::sequent& s ) throw( syntaxerror );

      // Read a model and a rulesystem. 

} 




#endif


