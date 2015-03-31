

// Code written by Ravinder Indarapu and Hans de Nivelle, April 2003.
// Adapted for Bliksem in September 2006.

#include "grammarrule.h"
#include <cstdlib>


namespace prolog
{


   grammarrule::grammarrule( unsigned int x ):
      type(x)
   {
      ASSERT( x == input_expr_dot || 
              x == expr_identifier || 
              x == expr_quotedidentifier ||
              x == expr_gluedidentifier_lpar_args_rpar ||
	      x == expr_gluedquotedidentifier_lpar_args_rpar ||
	      x == expr_lsqpar_list_rsqpar ||
	      x == expr_prefix_expr ||
	      x == expr_expr_infix_expr ||
	      x == expr_expr_postfix ||
	      x == expr_lpar_expr_rpar || 
	      x == args_expr ||
	      x == args_args_comma_expr ||
	      x == list_expr ||
	      x == list_expr_bar_expr ||
	      x == list_expr_comma_list ||
	      x == dot_identifier ||
	      x == comma_identifier ||
	      x == comma_gluedidentifier ||
	      x == bar_identifier ||
	      x == bar_gluedidentifier ||
	      x == infix_identifier ||
	      x == prefix_identifier ||
	      x == postfix_identifier );
   }


   unsigned int grammarrule::righthandsidelength( ) const
   {
      switch( type )
      {
      case input_expr_dot:
         return 2;

      case expr_identifier:
         return 1;
      case expr_quotedidentifier:
	 return 1; 
      case expr_gluedidentifier_lpar_args_rpar:
         return 4;
      case expr_gluedquotedidentifier_lpar_args_rpar:
	 return 4; 
      case expr_lsqpar_list_rsqpar:
         return 3;
      case expr_prefix_expr:
         return 2;
      case expr_expr_infix_expr:
         return 3;
      case expr_expr_postfix:
         return 2;
      case expr_lpar_expr_rpar:
         return 3;

      case args_expr:
         return 1;
      case args_args_comma_expr:
         return 3;

      case list_expr:
         return 1;
      case list_expr_bar_expr:
         return 3;
      case list_expr_comma_list:
         return 3;

      case dot_identifier:
         return 1;

      case comma_identifier:
         return 1;
      case comma_gluedidentifier:
         return 1;

      case bar_identifier:
         return 1;
      case bar_gluedidentifier:
	 return 1;

      case infix_identifier:
         return 1;

      case prefix_identifier:
         return 1;

      case postfix_identifier:
         return 1;

      default:
         ASSERT( false );  
	 exit(0); 
      }
   }




   bool grammarrule::reducible( const syntax& ss, 
                                const std::vector< token > & rhs ) const 
   {

      if( righthandsidelength() != rhs. size( ) )
         return false;

      switch( type )
      {
      case input_expr_dot:
         if( rhs[0]. type == token::expr &&
	     rhs[1]. type == token::dot )
	    return true;
	 break;

      case expr_identifier:
         if( rhs[0]. type == token::identifier )
	    return true;
         break;

      case expr_quotedidentifier:
	 if( rhs[0]. type == token::quotedidentifier )
            return true;
	 break;

      case expr_gluedidentifier_lpar_args_rpar:
         if( rhs[0]. type == token::gluedidentifier &&
             rhs[1]. type == token::lpar &&
	     rhs[2]. type == token::args &&
             rhs[3]. type == token::rpar )
	    return true;
         break;

      case expr_gluedquotedidentifier_lpar_args_rpar:
	 if( rhs [0]. type == token::gluedquotedidentifier &&
             rhs [1]. type == token::lpar &&
	     rhs [2]. type == token::args &&
	     rhs [3]. type == token::rpar )
            return true;
         break;

      case expr_lsqpar_list_rsqpar:
         if( rhs[0]. type == token::lsqpar &&
             rhs[1]. type == token::list &&
	     rhs[2]. type == token::rsqpar )
            return true;
         break;

      case expr_prefix_expr:
         if( rhs[0]. type == token::prefix &&
             rhs[1]. type == token::expr )
            return true;
         break;

      case expr_expr_infix_expr:
         if( rhs[0]. type == token::expr &&
             rhs[1]. type == token::infix &&
             rhs[2]. type == token::expr )
	    return true;
         break;

      case expr_expr_postfix:
         if( rhs[0]. type == token::expr &&
             rhs[1]. type == token::postfix )
            return true;
         break;

      case expr_lpar_expr_rpar:
         if( rhs[0]. type == token::lpar &&
             rhs[1]. type == token::expr &&
             rhs[2]. type == token::rpar )
            return true;
         break;

      case args_expr:
         if( rhs[0]. type == token::expr )
            return true;
         break;

      case args_args_comma_expr:
         if( rhs[0]. type == token::args &&
             rhs[1]. type == token::comma &&
             rhs[2]. type == token::expr )
            return true;
         break;

      case list_expr:
         if( rhs[0]. type == token::expr )
            return true;
         break;

      case list_expr_bar_expr:
         if( rhs[0]. type == token::expr &&
             rhs[1]. type == token::bar &&
             rhs[2]. type == token::expr )
            return true;
         break;

      case list_expr_comma_list:
         if( rhs[0]. type == token::expr &&
             rhs[1]. type == token::comma &&
             rhs[2]. type == token::list )
            return true;
         break;


      case dot_identifier:
         if( rhs[0]. type == token::identifier &&
             rhs[0]. getstring( ) == "." )
            return true;
         break;

      case comma_identifier:
         if( rhs[0]. type == token::identifier &&
             rhs[0]. getstring( ) == "," )
            return true;
         break;

      case comma_gluedidentifier:
         if( rhs[0]. type == token::gluedidentifier &&
	     rhs[0]. getstring( ) == "," )
            return true;
         break;

      case bar_identifier:
	 if( rhs[0]. type == token::identifier &&
             rhs[0]. getstring( ) == "|" )
            return true;
         break;

      case bar_gluedidentifier:
	 if( rhs[0]. type == token::gluedidentifier &&
             rhs[0]. getstring( ) == "|" )
            return true;
	 break;


      case infix_identifier:
	 if( rhs[0]. type == token::identifier &&
	     ss. hasinfixdef( rhs[0]. getstring( )) )
            return true;
         break;

      case prefix_identifier:
         if( rhs[0]. type == token::identifier &&
	     ss. hasprefixdef( rhs[0]. getstring( )) )
            return true;
         break;


      case postfix_identifier:
         if( rhs[0]. type == token::identifier &&
             ss. haspostfixdef( rhs[0]. getstring( )) )
            return true;
         break;
      }

      return false;
   }



   token grammarrule::reduce( const syntax& ss, 
                              const std::vector< token > & rhs ) const
   {

      ASSERT( reducible( ss, rhs ));
      
      switch( type )
      {
      case input_expr_dot:
         return token( token::input, rhs[0]. getexpression( ) );

      case expr_identifier:
      case expr_quotedidentifier:
         {
            std::list< expression > sub; 
            return token( token::expr,
                          expression( funcpred( rhs [0]. getstring( ), 0 ),
                                      sub )); 
         }

      case expr_gluedidentifier_lpar_args_rpar:
      case expr_gluedquotedidentifier_lpar_args_rpar:
         {
            return token( token::expr,
                          expression( 
			       funcpred(
				   rhs [0]. getstring( ),
				   rhs [2]. getexpressionlist( ). size( )),
			       rhs [2]. getexpressionlist( )));
         }

      case expr_lsqpar_list_rsqpar:
         {
            std::list< expression > members = rhs[1]. getexpressionlist( );

            expression res = members. back( );
            members. pop_back( );

            while( members. size( ) != 0 )
            {
               std::list< expression > sub;
	       sub. push_back( members. back( ));
	       sub. push_back( res );

               res = expression( ss. cons, sub ); 
               members. pop_back( ); 
            }
            return token( token::expr, res );
         }   

      case expr_prefix_expr:
         {
            std::list< expression > sub; 
	    sub. push_back( rhs [1]. getexpression( ));

	    const std::string& op = rhs [0]. getsyntacticdef( ). getstring( ); 

            return token( token::expr, expression( funcpred( op, 1 ), sub )); 
         }

      case expr_expr_infix_expr:
         {
            std::list< expression > sub;
	    sub. push_back( rhs[0]. getexpression( ));
	    sub. push_back( rhs[2]. getexpression( ));

	    const std::string& op = rhs [1]. getsyntacticdef( ). getstring( );

            return token( token::expr, expression( funcpred( op, 2 ), sub ));
         }

      case expr_expr_postfix:
         {
            std::list< expression > sub;
	    sub. push_back( rhs[0]. getexpression( ));

	    const std::string& op = rhs[1]. getsyntacticdef( ). getstring( ); 

            return token( token::expr, expression( funcpred( op, 1 ), sub )); 

         }

      case expr_lpar_expr_rpar:
         return rhs[1];


      case args_expr:
         {  
            std::list< expression > arg; 
            arg. push_back( rhs [0]. getexpression( ));
            return token( token::args, arg );
          }

      case args_args_comma_expr:
         {
            std::list< expression > args = rhs [0]. getexpressionlist( );
            args. push_back( rhs [2]. getexpression( )); 
	    return token( token::args, args );
               // Some care must be taken, because there is a source
               // of potential quadratic complexity here. This complexity
               // will show up when terms with a really high arity, say
               // 1000 are read. 
         }

      case list_expr:
         {
            std::list< expression > args;
            args. push_front( expression( ss. nil , args )); 
            args. push_front( rhs [0]. getexpression( ) ); 
            return token( token::list, args );
         }

      case list_expr_bar_expr:
         {
            std::list< expression > args;
            args. push_front( rhs [2]. getexpression( ) );
            args. push_front( rhs [0]. getexpression( ) );
            return token( token::list, args ); 
         }

      case list_expr_comma_list:
         {
            std::list< expression > args = rhs [2]. getexpressionlist( );
            args. push_front( rhs [0]. getexpression( ) ); 
	    return token( token::list, args );
         }

      case dot_identifier:
         return token( token::dot );

      case comma_identifier:
      case comma_gluedidentifier:
         return token( token::comma );

      case bar_identifier:
      case bar_gluedidentifier: 
         return token( token::bar );

      case infix_identifier:
         return token( token::infix, 
                       ss. getinfixdef( rhs[0]. getstring( )) );

      case prefix_identifier:
         return token( token::prefix, 
                       ss. getprefixdef( rhs[0]. getstring( )) );

      case postfix_identifier:
         return token( token::postfix, 
                       ss. getpostfixdef( rhs[0]. getstring( )) );

      }

      ASSERT( false ); 
      exit(0); 
   }




   std::ostream& operator << ( std::ostream& os, const grammarrule& rule )
   {

      switch( rule. type )
      {
      case grammarrule::input_expr_dot:
         os << "Input -> Expr .";
         break;

      case grammarrule::expr_identifier:
         os << "Expr -> Identifier";
         break;

      case grammarrule::expr_quotedidentifier:
	 os << "Expr -> QuotedIdentifier";
	 break;

      case grammarrule::expr_gluedidentifier_lpar_args_rpar:
         os << "Expr -> GluedIdentifier ( Args )";
         break;

      case grammarrule::expr_gluedquotedidentifier_lpar_args_rpar:
         os << "Expr -> GluedQuotedIdentifier ( Args )";
         break;

      case grammarrule::expr_lsqpar_list_rsqpar:
         os << "Expr -> [ List ]";
         break;

      case grammarrule::expr_prefix_expr:
         os << "Expr -> Prefix Expr";
         break;

      case grammarrule::expr_expr_infix_expr:
         os << "Expr -> Expr Infix Expr";
         break;

      case grammarrule::expr_expr_postfix:
         os << "Expr -> Expr Postfix";
         break;

      case grammarrule::expr_lpar_expr_rpar:
         os << "Expr -> ( Expr )";
         break;

      case grammarrule::args_expr:
         os << "Args -> Expr";
         break;

      case grammarrule::args_args_comma_expr:
         os << "Args -> Args , Expr";
         break;

      case grammarrule::list_expr:
         os << "List -> Expr";
         break;

      case grammarrule::list_expr_bar_expr:
         os << "List -> Expr | Expr";
         break;

      case grammarrule::list_expr_comma_list:
         os << "List -> Expr , List";
         break;

      case grammarrule::dot_identifier:
         os << "Dot -> Identifier";
         break;

      case grammarrule::comma_identifier:
         os << "Comma -> Identifier";
         break;

      case grammarrule::comma_gluedidentifier:
         os << "Comma -> GluedIdentifier";
         break;

      case grammarrule::bar_identifier:
         os << "Bar -> Identifier";
         break;

      case grammarrule::bar_gluedidentifier:
	 os << "Bar -> GluedIdentifier";
	 break;

      case grammarrule::infix_identifier:
         os << "Infix -> Identifier";
         break;

      case grammarrule::prefix_identifier:
         os << "Prefix -> Identifier";
         break;

      case grammarrule::postfix_identifier:
         os << "Postfix -> Identifier";
         break;

      default:
         ASSERT( false ); 
      }
      return os;

   }

}


