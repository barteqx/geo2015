
// Code written by Ravinder Indarapu and Hans de Nivelle, May 2003.
// Added error recovery on 2nd of March 2004.


#include "printingreading.h"
#include "parsestack.h"
#include "reduction.h"
#include "scanner.h"
#include <sstream>
#include <cstdlib>


namespace prolog 
{


   namespace
   {


      const unsigned int identifier_expr [] = 
         { token::start, token::prefix, token::infix,
           token::bar, token::comma, token::lpar, token::lsqpar, 
	   parsestack::next,
           token::identifier, token::quotedidentifier, parsestack::next,
           token::rpar, token::rsqpar, token::bar,
           token::dot, token::comma, token::identifier, parsestack::next,
	   parsestack::next };

      const unsigned int identifier_pref [] = 
         { token::start, token::prefix, token::infix,
           token::bar, token::comma, token::lpar, token::lsqpar, 
	   parsestack::next,
           token::identifier, parsestack::next,
           token::identifier, token::gluedidentifier,
	   token::quotedidentifier, token::gluedquotedidentifier,
           token::lpar, token::lsqpar, parsestack::next, 
	   parsestack::next };

      const unsigned int identifier_postf [] =
         { token::expr, parsestack::next,
           token::identifier, parsestack::next,
           token::rpar, token::rsqpar, token::bar, token::dot,
	   token::comma, token::identifier, parsestack::next, 
	   parsestack::next };

      const unsigned int identifier_inf [] =
         { token::expr, parsestack::next,
           token::identifier, parsestack::next,
           token::identifier, token::gluedidentifier,
	   token::quotedidentifier, token::gluedquotedidentifier,
           token::lpar, token::lsqpar, parsestack::next, 
	   parsestack::next };


      const unsigned int rule1 [] = 
         { token::start, parsestack::next,
           token::expr, parsestack::next,
           token::dot, parsestack::next, parsestack::next };

	   
      const unsigned int prefinf [] =
         { token::prefix, parsestack::next,
           token::expr, parsestack::next,
           token::infix, parsestack::next,
           parsestack::dontcare, parsestack::next, parsestack::next };

      const unsigned int infinf [] = 
         { token::expr, parsestack::next,
           token::infix, parsestack::next,
           token::expr, parsestack::next,
           token::infix, parsestack::next,
           parsestack::dontcare, parsestack::next, parsestack::next };

      const unsigned int infterminator [] =
         { token::expr, parsestack::next,
           token::infix, parsestack::next,
           token::expr, parsestack::next,
           token::rpar, token::rsqpar, token::dot,
	   token::comma, token::bar, parsestack::next, parsestack::next };

      const unsigned int prefpostf [] =
         { token::prefix, parsestack::next,
           token::expr, parsestack::next,
           token::postfix, parsestack::next,
           parsestack::dontcare, parsestack::next, parsestack::next };

      const unsigned int infpostf [] =
         { token::expr, parsestack::next,
           token::infix, parsestack::next,
           token::expr, parsestack::next,
           token::postfix, parsestack::next,
           parsestack::dontcare, parsestack::next, parsestack::next };

      const unsigned int startpostf [] = 
         { token::start, token::bar,
	   token::comma, token::lpar, token::lsqpar, parsestack::next,
           token::expr, parsestack::next,
           token::postfix, parsestack::next,
           parsestack::dontcare, parsestack::next, parsestack::next };

      const unsigned int prefterminator [] =
         { token::prefix, parsestack::next,
           token::expr, parsestack::next,
           token::rpar,
           token::rsqpar, token::dot, token::comma, token::bar, 
	   parsestack::next, parsestack::next };


      const unsigned int parentheses [] = 
         { token::start, token::prefix, token::infix,
           token::bar, token::comma, token::lpar, token::lsqpar, 
	               parsestack::next,
           token::lpar, parsestack::next,
           token::expr, parsestack::next,
           token::rpar, parsestack::next, parsestack::next };


      const unsigned int args1 [] = 
         { token::lpar, parsestack::next,
           token::expr, parsestack::next,
           token::comma, token::rpar, parsestack::next, parsestack::next };

      const unsigned int args2 [] = 
         { token::args, parsestack::next,
           token::comma, parsestack::next,
           token::expr, parsestack::next,
           token::comma, token::rpar, parsestack::next, parsestack::next };


      const unsigned int func [] =
         { token::gluedidentifier, token::gluedquotedidentifier,
	   parsestack::next,
           token::lpar, parsestack::next,
           token::args, parsestack::next,
           token::rpar, parsestack::next, parsestack::next };


      const unsigned int list1 [] = 
         { token::lsqpar, token::comma, parsestack::next,
           token::expr, parsestack::next,
           token::rsqpar, parsestack::next, parsestack::next };

      const unsigned int list2 [] = 
         { token::lsqpar, token::comma, parsestack::next,
           token::expr, parsestack::next,
           token::bar, parsestack::next,
           token::expr, parsestack::next,
           token::rsqpar, parsestack::next, parsestack::next };

      const unsigned int list3 [] =
         { token::lsqpar, token::comma, parsestack::next,
           token::expr, parsestack::next,
           token::comma, parsestack::next,
           token::list, parsestack::next,
           token::rsqpar, parsestack::next, parsestack::next };


      const unsigned int listrule [] =
         { token::lsqpar, parsestack::next,
           token::list, parsestack::next,
           token::rsqpar, parsestack::next, parsestack::next };




      // Throw a syntax error, explaing that the conflict between left
      // and right could not be resolved:

      void throw_operator_conflict( const syntacticdef& left, 
                                    const syntacticdef& right,
                                    const parser::inputstream& is ) 
      throw ( parser::syntaxerror )
      {
         std::ostringstream s; 
         s << "could not resolve operator conflict between ";
         s << left. getstring( );
         s << " and ";
         s << right. getstring( ); 
         throw parser::syntaxerror( s. str( ), is );
      }




      // Decide on the fate of the identifier or quoted_identifier that
      // is situated on position ps. gettoken( ps. size( ) - 2 ).
      // An identifier/quotedidentifier in this position always has to be
      // reduced. However, it is not so easy to decide how to reduce.
      // If the identifier has a prefix/infix/postfix definition, we have to
      // decide whether we are going to use that definition,
      // or still reduce it to an expr. In case the identifier has two
      // operatordefinitions, we have to decide which one we
      // are going to use.
      // The parser::inputstream is used only for creating error messages.



      reduction decide_identifier( const parsestack& ps,
                                   const syntax& synt, 
                                   const parser::inputstream& is ) 
      throw ( parser::syntaxerror )
      {

         unsigned int pslength = ps. size( );

         const token& token1 = ps. gettoken( pslength - 1 );
         const token& token2 = ps. gettoken( pslength - 2 );

         // It is token2 that we are deciding about. 
         // It must be an identifier or a quoted identifier. 

         ASSERT( token2. type == token::identifier ||
                 token2. type == token::quotedidentifier ); 

         partial< syntacticdef > pref2; 
         partial< syntacticdef > inf2; 
         partial< syntacticdef > postf2; 

         if( ps. patternfits( identifier_pref ) &&
             synt. hasprefixdef( token2. getstring( ) ))
            pref2 = synt. getprefixdef( token2. getstring( ));

         if( ps. patternfits( identifier_inf ) &&
             synt. hasinfixdef( token2. getstring( ) ))
            inf2 = synt. getinfixdef( token2. getstring( ));

         if( ps. patternfits( identifier_postf ) &&
             synt. haspostfixdef( token2. getstring( ) )) 
            postf2 = synt. getpostfixdef( token2. getstring( ));

         bool expr2 = ps. patternfits( identifier_expr );

         if( ( inf2. isdefined( ) || postf2. isdefined( ) ) &&
             ( expr2 || pref2. isdefined( ) ))
         {
            std::cout << "could not classify\n"; 
	    ASSERT( false );
	    exit(0);
         }

         if( inf2. isdefined( ) && postf2. isdefined( ) )
         {
            if( token1. type == token::identifier )
            {
               if( synt. hasinfixdef( token1. getstring( )) ||
                   synt. haspostfixdef( token1. getstring( )) )

                  inf2. clear( );    // Resets inf2 into undefined position. 
               else
                  postf2. clear( );  // Resets postf2 into undefined position. 
            }
            else
            {
               std::cout << "could not classify\n";
	       ASSERT( false );
	       exit(0);
            }
         }

         if( pref2. isdefined( ) && expr2 )
         {
            if( token1. type == token::identifier )
            {
               if( synt. hasinfixdef( token1. getstring( )) ||
                   synt. haspostfixdef( token1. getstring( )) )

                  pref2. clear( );    // Resets pref2 into undefined. 
               else
                  expr2 = false;
            }
            else
            {
               std::cout << "could not classify\n";
	       ASSERT( false );
	       exit(0);
            }
         }

         if( inf2. isdefined( ) )
            return reduction( pslength - 2, grammarrule::infix_identifier );

         if( pref2. isdefined( ) )
            return reduction( pslength - 2, grammarrule::prefix_identifier );

         if( postf2. isdefined( ) )
            return reduction( pslength - 2, grammarrule::postfix_identifier );

         if( expr2 )
         {
            if( token2. type == token::identifier )
               return reduction( pslength - 2, grammarrule::expr_identifier );
	    else
               return reduction( pslength - 2, 
			      grammarrule::expr_quotedidentifier );
         }

         std::ostringstream s;
         s << "misplaced identifier ";
         s << "\'" << token2. getstring( ) << "\'"; 

         throw parser::syntaxerror( s. str( ), is );

      }





      // Inspect the parsestack ps, and decide what action has to
      // be taken. decide_action does not perform any actions by itself, it
      // only decides. The action is either a reduction or a shift.
      // decide_action returns a partial<reduction> , which contains a
      // reduction in case one is recommended. When partial<reduction> is
      // undefined, this means that a shift is recommended.
      // decide_action does nothing with the counting_istream. It is there,
      // so that it can be passed as an error message.



      partial< reduction > decide_action( const parsestack& ps, 
		                          const syntax& synt, 
			                  const parser::inputstream& is ) 
      throw ( parser::syntaxerror )
      {
         unsigned int pslength = ps. size( );

         if( pslength > 2 &&
             ps. gettoken ( pslength - 1 ). type == token::endoffile )
                throw parser::syntaxerror( "unexpected end of file", is );


         if( pslength > 1 &&
            ps. gettoken ( pslength - 1 ). type == token::error )
                throw parser::syntaxerror( 
		     std::string( "garbage: " ) + 
                     ps. gettoken( pslength - 1 ). getstring( ), is );


         if( pslength >= 2 )
         {

            // Decide whether an identifier or gluedidentifier 
	    // should be changed into one of the special symbols 
	    //           comma, bar or dot. 
            // We are considering the name on position pslength - 1.

            context last_context =     ps. getcontext( pslength - 1 );
            token last_token =         ps. gettoken( pslength - 1 );
            unsigned int last_type =   last_token. type;

            if( last_type == token::identifier ||
                last_type == token::gluedidentifier ) 
            {
               const std::string& s = last_token. getstring( );

               if( last_type == token::identifier && 
		       pslength >= 3 && 
                       s == "." && last_context. allows( "." ))
                  return reduction( pslength - 1, 
				    grammarrule::dot_identifier );

               if( pslength >= 3 && 
                       s == "," && last_context. allows( "," ))
               {
                  if( last_type == token::identifier ) 
                     return reduction( pslength - 1, 
				       grammarrule::comma_identifier );
	          else
                     return reduction( pslength - 1,
				       grammarrule::comma_gluedidentifier );
               }

               if( pslength >= 3 &&
		       s == "|" && last_context. allows( "|" ))
               {
                  if( last_type == token::identifier )
                     return reduction( pslength - 1, 
				       grammarrule::bar_identifier );
	          else
                     return reduction( pslength - 1,
				       grammarrule::bar_gluedidentifier );
               }
            }

         }   

         if( pslength >= 3 && 
             (
                ps. gettoken( pslength - 2 ).type == token::identifier ||
	        ps. gettoken( pslength - 2 ).type == token::quotedidentifier ))
         {
            return decide_identifier( ps, synt, is );
         }

         if( ps. patternfits( rule1 ))
            return reduction( pslength - 2, grammarrule::input_expr_dot );

         if( ps. patternfits( prefinf ))
         {
            const token& token4 = ps. gettoken( pslength - 4 );
            const token& token2 = ps. gettoken( pslength - 2 );
            int dir = token4. getsyntacticdef( ). 
		              decideconflict( token2. getsyntacticdef( ));
            if( dir == -1 )
               return reduction( pslength - 4, grammarrule::expr_prefix_expr );

            if( dir == 0 )
               throw_operator_conflict( token4. getsyntacticdef( ),
                                        token2. getsyntacticdef( ),
                                        is );
         }

         if( ps. patternfits( infinf ) )
         {
            const token& token4 = ps. gettoken( pslength - 4 );
            const token& token2 = ps. gettoken( pslength - 2 );
	    int dir = token4. getsyntacticdef( ). 
		              decideconflict( token2. getsyntacticdef( ));
            if( dir == -1 )
               return reduction( pslength - 5, 
                                 grammarrule::expr_expr_infix_expr );


	    if( dir == 0 )
               throw_operator_conflict( token4. getsyntacticdef( ),
                                        token2. getsyntacticdef( ),
                                        is );
         }

         if( ps. patternfits( infterminator ))
         {
            return reduction( pslength - 4,
                              grammarrule::expr_expr_infix_expr );
         }

         if( ps. patternfits( prefpostf ) )
         {
            const token& token4 = ps. gettoken( pslength - 4 );
            const token& token2 = ps. gettoken( pslength - 2 );

            int dir = token4. getsyntacticdef( ).
                              decideconflict( token2. getsyntacticdef( ));

            if( dir == -1 )
               return reduction( pslength - 4, grammarrule::expr_prefix_expr );

            if( dir == 1 )
               return reduction( pslength - 3, 
			         grammarrule::expr_expr_postfix );

            throw_operator_conflict( token4. getsyntacticdef( ),
                                     token2. getsyntacticdef( ), 
                                     is ); 
         }

         if( ps. patternfits( infpostf ))
         {
            const token& token4 = ps. gettoken( pslength - 4 );
            const token& token2 = ps. gettoken( pslength - 2 );

            int dir = token4. getsyntacticdef( ).
		                 decideconflict( token2. getsyntacticdef( ));

	    if( dir == -1 )
               return reduction( pslength - 5,
                                 grammarrule::expr_expr_infix_expr );

            if( dir == 1 )
               return reduction( pslength - 3,
                                 grammarrule::expr_expr_postfix );

            throw_operator_conflict( token4. getsyntacticdef( ),
			             token2. getsyntacticdef( ),
                                     is );
         }

         if( ps. patternfits( startpostf ))
            return reduction( pslength - 3, grammarrule::expr_expr_postfix );

         if( ps. patternfits( prefterminator ))
            return reduction( pslength - 3, grammarrule::expr_prefix_expr );

         if( ps. patternfits( parentheses ))
            return reduction( pslength - 3, 
			      grammarrule::expr_lpar_expr_rpar );

         if( ps. patternfits( args1 ))
            return reduction( pslength - 2, grammarrule::args_expr );

         if( ps. patternfits( args2 ))
            return reduction( pslength - 4, 
			      grammarrule::args_args_comma_expr );

         if( ps. patternfits( func ))
         {
            if( ps. gettoken( pslength - 4 ). type == token::gluedidentifier )
               return reduction( pslength - 4,
                   grammarrule::expr_gluedidentifier_lpar_args_rpar );
	    else
               return reduction( pslength - 4,
		   grammarrule::expr_gluedquotedidentifier_lpar_args_rpar );
         }

         if( ps. patternfits( list1 ))
            return reduction( pslength - 2, grammarrule::list_expr );

         if( ps. patternfits( list2 ))
            return reduction( pslength - 4, grammarrule::list_expr_bar_expr );

         if( ps. patternfits( list3 ))
            return reduction( pslength - 4, 
			      grammarrule::list_expr_comma_list );

         if( ps. patternfits( listrule ))
            return reduction( pslength - 3, 
                              grammarrule::expr_lsqpar_list_rsqpar );

         // If we have a terminator on top of the stack, and we could 
         // not find a reduction, then the input is wrong. 

         const token& token1 = ps. gettoken( pslength - 1 );
         if( token1. type == token::rpar || 
             token1. type == token::rsqpar ||
             token1. type == token::dot )
                throw parser::syntaxerror( "syntax error", is );

         return partial< reduction > ( ); 
       }



   }



#define READERDEBUG   0  


   partial< expression > readexpression( parser::inputstream& inputreader, 
	                                 const syntax& synt ) 
   throw ( parser::syntaxerror, parser::readerror )
   {
      parsestack ps;

      try
      {
         while( true )
         {

#if READERDEBUG
            std::cout << "\n"; 
            std::cout << "parsestack: " << ps << "\n";
#endif

            // The normal endstate after successfully reading a term:

            if( ps. size( ) == 2 &&
                ps. gettoken(0). type == token::start &&
                ps. gettoken(1). type == token::input )
            {
               return ps. gettoken(1). getexpression( );
            }


            // The normal endstate on end of file:

            if( ps. size( ) == 2 &&
                ps. gettoken(0). type == token::start &&
                ps. gettoken(1). type == token::endoffile )
            {
               return partial< expression > ( );
            }


            partial< reduction > red = decide_action( ps, synt, inputreader );

            if( red. isdefined( ) ) 
            {
#if READERDEBUG
               std::cout << red << "\n";
#endif

               unsigned int rhs_length = red. contents( ). 
                                              rule. righthandsidelength( );

               std::vector< token > rhs;

               for( unsigned int i = 0; i < rhs_length; i ++ )
                  rhs. push_back( 
			  ps. gettoken( red. contents( ). position + i ));

               token lhs = red. contents( ). rule. reduce( synt, rhs );
               ps. reduce( red. contents( ). position, lhs, rhs_length );
            }
            else
            {
               token t = scanner::readtoken( inputreader );
#if READERDEBUG
               std::cout << "pushing " << t ;
#endif
               ps. shift( t );
            }

         }
      }
      catch( const parser::syntaxerror& s ) 
      {
         // We caught a syntax error, and try to recover gracefully,
	 // so that the reader behaves nice, when used interactively.

#if READERDEBUG 
         std::cout << "\n"; 
         std::cout << "parsestack: " << ps << "\n";  
         std::cout << "caught a syntax error: " << s << "\n";
#endif 

         token last = ps. gettoken( ps. size( ) - 1 );  
         while( last. type != token::endoffile &&
                last. type != token::dot && 
                ( last. type != token::identifier || 
		  last. getstring( ) != "." ))
         {
#if READERDEBUG 
            std::cout << "flushed " << last << "\n";
#endif
            last = scanner::readtoken( inputreader );
         }
	 throw(s);
      }
      // We don't catch read errors, because we can do nothing
      // meaningful with them. 
   } 


} 


