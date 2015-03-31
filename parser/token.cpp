

// Code written by Hans de Nivelle, April 2006.
// Modified for first-order, June 2006. 

#include "token.h"
#include <cstdlib>


namespace parser
{


   std::ostream& operator << ( std::ostream& stream, 
		               const token& t )
   {
   
      switch( t. typeoftoken )
      {
      case token::token_lpar:
         stream << "lpar";
	 return stream;

      case token::token_rpar:
         stream << "rpar";
	 return stream;

      case token::token_comma:
         stream << "comma";
	 return stream;

      case token::token_dot:
         stream << "dot";
	 return stream;

      case token::token_slash:
         stream << "slash";
	 return stream;


      case token::token_false:
	 stream << "false";
	 return stream;

      case token::token_true:
	 stream << "true";
	 return stream;

      case token::token_not:
	 stream << "not";
	 return stream;

      case token::token_and:
         stream << "and";
	 return stream;

      case token::token_or:
         stream << "or";
	 return stream;

      case token::token_implies:
         stream << "implies";
	 return stream;

      case token::token_equiv:
         stream << "equiv";
	 return stream;

      case token::token_forall1:
         stream << "forall1";
	 return stream;

      case token::token_forall2:
         stream << "forall2";
	 return stream;

      case token::token_exists1:
         stream << "exists1";
	 return stream;

      case token::token_exists2:
         stream << "exists2";
	 return stream;

      case token::token_create:
         stream << "create";
	 return stream;

      case token::token_diseq:
         stream << "diseq";
	 return stream;

      case token::token_eq:
         stream << "eq";
	 return stream;


      case token::token_rulesystem:
         stream << "rulesystem";
	 return stream;

      case token::token_model:
         stream << "model";
	 return stream;

      case token::token_firstorder:
         stream << "firstorder";
	 return stream;

      case token::token_predicates:
         stream << "predicates";
	 return stream;

      case token::token_functions:
	 stream << "functions";
	 return stream;

      case token::token_end:
         stream << "end";
	 return stream;


      case token::token_string:
         stream << "string( ";
	 stream << t. stringattr;
	 stream << " )";
	 return stream;

      case token::token_unsigned:
	 stream << "unsigned( ";
	 stream << t. unsignedattr;
	 stream << " )";
	 return stream;


      case token::token_error:
         stream << "error";
	 return stream;

      case token::token_eof:
         stream << "eof";
	 return stream;

      }
      
      ASSERT( false ); exit(0);
   } 

} 


