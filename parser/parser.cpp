
// Hans de Nivelle, 2006.


#include "parser.h"
#include <cstdlib>


namespace parser
{

   bool startidentifier( char c )
   {
      if( c >= 'a' && c <= 'z' ) return true;
      if( c >= 'A' && c <= 'Z' ) return true;
      if( c == '_' ) return true;
      return false;
   }

   bool inidentifier( char c )
   {
      if( c >= 'a' && c <= 'z' ) return true;
      if( c >= 'A' && c <= 'Z' ) return true;
      if( c >= '0' && c <= '9' ) return true;
      if( c == '_' ) return true;
      return false;
   }
    
   bool isdigit( char c )
   {
      if( c >= '0' && c <= '9' ) return true;
      return false;
   }

   std::string converttostring( unsigned int x )
   {
      if( x >= 10 )
      {
         char c = ( x % 10 ) + '0';
	 x = x / 10;
	 std::string res = converttostring(x);
	 res += c;
	 return res;
      }
      else
      {
         std::string res;
	 char c = x + '0';
	 res += c;
	 return res;
      }
   }


   unsigned int make_unsigned( std::string::const_iterator p1, 
		               std::string::const_iterator p2,
			       const inputstream& mess ) 
      throw( syntaxerror )
   {
      if( p1 == p2 )
         throw syntaxerror( "zero-length integer", mess );

      ASSERT( p1 <= p2 );

      unsigned int val = 0;
      while( p1 != p2 )
      {
         char c = *p1;
	 if( ! isdigit(c) ) 
            throw syntaxerror( "non-digit in integer", mess );

	 unsigned int x = c - '0';

	 ASSERT( x < 10 );

	 val = val * 10;
	 val += x;

	 ++ p1;
      }
      return val;
   }



   token readtoken( inputstream& stream ) throw( syntaxerror )
   {

   whitespace:
      if( stream. nextchar == ' ' ||
          stream. nextchar == '\t' ||
          stream. nextchar == '\n' ||
	  stream. nextchar == 0x0D )
      {
         stream. moveforward( );
	 goto whitespace;
      }

      if( stream. nextchar == EOF )
         return token::token_eof;

      if( stream. nextchar == '(' )
      {
         stream. moveforward( );
	 return token::token_lpar;
      }

      if( stream. nextchar == ')' )
      {
         stream. moveforward( );
	 return token::token_rpar;
      }

      if( stream. nextchar == ',' )
      {
         stream. moveforward( );
	 return token::token_comma;
      }

      if( stream. nextchar == '.' )
      {
         stream. moveforward( );
	 return token::token_dot;
      }


      if( stream. nextchar == '!' )
      {
         stream. moveforward( );
	 if( stream. nextchar == '=' )
         {
            stream. moveforward( );
	    return token::token_diseq;
         }
	 return token::token_not;
      }

      if( stream. nextchar == '/' )
      {
         // This could be either
	 // an and:                     /\
	 // a single slash:             /
	 // a comment:                  /*                 */
         // a comment:                  //          

         stream. moveforward( );

	 if( stream. nextchar == '\\' )
         {
            stream. moveforward( );
	    return token::token_and;
         }

	 if( stream. nextchar == '/' )
         {
            stream. moveforward( );
	    while( stream. nextchar != '\n' && stream. nextchar != EOF )
            {
               stream. moveforward( );
            }

            if( stream. nextchar == EOF )
               return token::token_error;    // Because of EOF inside comment.

            stream. moveforward( );
	    goto whitespace;
         }

	 if( stream. nextchar == '*' )
         {
            stream. moveforward( ); 

         incomment: 
	    while ( stream. nextchar != '*' && stream. nextchar != EOF )
            {
               stream. moveforward( );
	    }

            if( stream. nextchar == EOF )
               return token::token_error;    // Because of EOF inside comment.

	    stream. moveforward( );

	    if( stream. nextchar == '/' )
            {
               stream. moveforward( );
	       goto whitespace;   
            }

	    goto incomment; 
         }

	 return token::token_slash; 
      }

      if( stream. nextchar == '\\' )
      {
         stream. moveforward( );
	 if( stream. nextchar == '/' )
         {
            stream. moveforward( );
	    return token::token_or;
         }
	 return token::token_error;
      }

      if( stream. nextchar == '-' )
      {
         stream. moveforward( );
	 if( stream. nextchar == '>' )
         {
            stream. moveforward( );
	    return token::token_implies;
         }
	 return token::token_error;
      }

      if( stream. nextchar == '[' )
      {
         stream. moveforward( );
	 return token::token_forall1;
      }

      if( stream. nextchar == ']' )
      {
         stream. moveforward( );
	 return token::token_forall2;
      }

      if( stream. nextchar == '<' )
      {
         stream. moveforward( );
	 if( stream. nextchar == '-' )
         {
            stream. moveforward( );
	    if( stream. nextchar == '>' )
            {
               stream. moveforward( );
	       return token::token_equiv;
            }
	    return token::token_error;
         }
	 return token::token_exists1;
      }

      if( stream. nextchar == '>' )
      {
         stream. moveforward( );
	 return token::token_exists2;
      }

      if( stream. nextchar == '#' )
      {
         stream. moveforward( );
	 return token::token_create;
      }

      if( stream. nextchar == '=' )
      {
         stream. moveforward( );
	 return token::token_eq;
      }

      if( stream. nextchar == '%' )
      {
         stream. moveforward( );

	 std::string s;
	 while( inidentifier( stream. nextchar ))
         {
            s. push_back( stream. nextchar );
	    stream. moveforward( );
         }

	 if( s == std::string( "rulesystem" ))
            return token::token_rulesystem;

	 if( s == std::string( "model" ))
            return token::token_model;

	 if( s == std::string( "firstorder" ))
            return token::token_firstorder;

	 if( s == std::string( "predicates" ))
            return token::token_predicates;

	 if( s == std::string( "functions" ))
            return token::token_functions;

	 if( s == std::string( "end" ))
            return token::token_end;

	 return token::token_error; 

      }

      if( startidentifier( stream. nextchar ))
      {
         std::string s;
         s. push_back( stream. nextchar );
	 stream. moveforward( );

	 while( inidentifier( stream. nextchar ))
         {
            s. push_back( stream. nextchar );
	    stream. moveforward( );
         }

	 if( s == std::string( "TRUE" ))
            return token::token_true;

	 if( s == std::string( "FALSE" ))
            return token::token_false;

	 return token( token::token_string, s );
      }

      if( isdigit( stream. nextchar ))
      {
         std::string s;
	 s. push_back( stream. nextchar );
	 stream. moveforward( );

	 while( isdigit( stream. nextchar ))
         {
            s. push_back( stream. nextchar );
	    stream. moveforward( );
         }

	 return token( token::token_unsigned, 
		       make_unsigned( s. begin( ), s. end( ), stream ));
      }

      // We could not read anything. 
      // We advance the tape by one, (in order to avoid non-termination)
      // and return the error token.

      stream. moveforward( );
      return token::token_error;
   }



   element make_element( const std::string& s, const inputstream& mess ) 
      throw( syntaxerror )
   {
      if( s[0] != 'E' )
         throw syntaxerror( "element must start with E", mess );

      unsigned int v = make_unsigned( s. begin( ) + 1, s. end( ), mess );
      return element::constructfromunsigned( v );
   }


   variable make_variable( const std::string& s, const inputstream& mess )
      throw( syntaxerror )
   {
      if( s[0] != 'V' )
         throw syntaxerror( "variable must start with V", mess );

      unsigned int v = make_unsigned( s. begin( ) + 1, s. end( ), mess );
      return variable::constructfromunsigned( v );
   }



   groundatom readgroundatom( inputstream& stream,
		              token &next ) throw( syntaxerror ) 
   {

      static std::vector< element > args;
      args. clear( );

      if( next. typeoftoken == token::token_create )
      {
         next = readtoken( stream );
	 if( next. typeoftoken != token::token_string )
	    throw syntaxerror( "element expected", stream );

	 args. push_back( make_element( next. stringattr, stream ));
	 next = readtoken( stream );

         return groundatom( predicate::create, args );
      }

      if( next. typeoftoken != token::token_string )
         throw syntaxerror( "identifier expected", stream );

      std::string pred = next. stringattr;
      next = readtoken( stream );

      if( next. typeoftoken == token::token_lpar )
      {
         next = readtoken( stream );
	 if( next. typeoftoken != token::token_rpar )
         {
            if( next. typeoftoken != token::token_string )
               throw syntaxerror( "element expected", stream );

	    args. push_back( make_element( next. stringattr, stream ));
	    next = readtoken( stream );

	    while( next. typeoftoken != token::token_rpar )
            {
               if( next. typeoftoken != token::token_comma )
                  throw syntaxerror( "comma expected", stream );
	       next = readtoken( stream );

	       if( next. typeoftoken != token::token_string )
                  throw syntaxerror( "element expected", stream );

	       args. push_back( make_element( next. stringattr, stream ));
	       next = readtoken( stream );
            }
	 }

	 next = readtoken( stream );    // This was an rpar.
      }

      if( ! predicate::isdefined( pred, args. size( )))
         throw syntaxerror( "arity does not fit", stream );

      return groundatom( predicate( pred, args. size( )), args );
   }



   varatom readvaratom( inputstream& stream,
		        token &next ) throw( syntaxerror ) 
   {
      static std::vector< variable > args;
      args. clear( );

      if( next. typeoftoken == token::token_create )
      {
         next = readtoken( stream );
         if( next. typeoftoken != token::token_string )
            throw syntaxerror( "variable expected", stream );

         args. push_back( make_variable( next. stringattr, stream ));
	 next = readtoken( stream );

	 return varatom( predicate::create, args );
      }

      if( next. typeoftoken != token::token_string )
         throw syntaxerror( "identifier expected", stream );

      std::string pred = next. stringattr;
      next = readtoken( stream );


      if( next. typeoftoken == token::token_lpar )
      {
         next = readtoken( stream );
	 if( next. typeoftoken != token::token_rpar )
         {
            if( next. typeoftoken != token::token_string )
               throw syntaxerror( "variable expected", stream );

	    args. push_back( make_variable( next. stringattr, stream ));
	    next = readtoken( stream );

	    while( next. typeoftoken != token::token_rpar )
            {
               if( next. typeoftoken != token::token_comma )
                  throw syntaxerror( "comma expected", stream );
	       next = readtoken( stream );

	       if( next. typeoftoken != token::token_string )
                  throw syntaxerror( "variable expected", 
				                  stream );

	       args. push_back( make_variable( next. stringattr, stream ));
	       next = readtoken( stream );
            }
	 }

	 next = readtoken( stream );    // This was an rpar.
      }
      else
      {
         // Either the atom is propositional, or it is a disequality
	 // atom.

         if( next. typeoftoken == token::token_diseq )
         {
            args. push_back( make_variable( pred, stream ));
               // The predicate was not a predicate after all.

            next = readtoken( stream );
	    if( next. typeoftoken != token::token_string )
               throw syntaxerror( "variable expected", stream );

            args. push_back( make_variable( next. stringattr, stream ));
	    next = readtoken( stream );
           
	    return varatom( predicate::diseq, args );
         }
      }

      if( ! predicate::isdefined( pred, args. size( )))
         throw syntaxerror( "arity does not fit", stream );

      return varatom( predicate( pred, args. size( )), args );
   }



   rule readrule( inputstream& stream, token& next ) throw( syntaxerror )
   {
      static std::vector< predicate > predicates_base;
      static std::vector< variable > variables_base;

      predicates_base. clear( );
      variables_base. clear( );

      rule res( predicates_base, variables_base ); 

      if( next. typeoftoken != token::token_implies )
      {
         varatom va = readvaratom( stream, next );
	 res. addpremisse( va );

	 while( next. typeoftoken != token::token_implies )
         {
            if( next. typeoftoken != token::token_and )
               throw syntaxerror( "and expected", stream ); 

	    next = readtoken( stream );

            va = readvaratom( stream, next );
	    res. addpremisse( va );
         }
      }

      next = readtoken( stream );   // This was the arrow. 

      if( next. typeoftoken == token::token_create )
      {
         next = readtoken( stream );
	 if( next. typeoftoken != token::token_string )
            throw syntaxerror( "variable expected", stream );

	 variable v = make_variable( next. stringattr, stream );
	 next = readtoken( stream );

	 if( next. typeoftoken != token::token_and )
            throw syntaxerror( "and expected", stream );

	 next = readtoken( stream );
	 varatom va = readvaratom( stream, next );
	 res. setexistential( v, va );

	 if( next. typeoftoken != token::token_dot )
             throw syntaxerror( "dot expected", stream ); 

	 return res;
      }
      else
      {
         if( next. typeoftoken != token::token_dot )
         {
            varatom va = readvaratom( stream, next );
	    res. addconclusion( va );

	    while( next. typeoftoken != token::token_dot )
            {
               if( next. typeoftoken != token::token_or )
                  throw syntaxerror( "or expected", stream ); 

	       next = readtoken( stream );

	       va = readvaratom( stream, next );
	       res. addconclusion( va );
            }
	 }

	 return res; 
      }
   }



   void readfoargs( std::list< firstorder::term > & args,
		    inputstream& stream,
		    token& next ) throw( syntaxerror )
   {
      if( next. typeoftoken == token::token_lpar )
      {
         next = readtoken( stream );
	 if( next. typeoftoken != token::token_rpar )
         {
	    args. push_back( readfoterm( stream, next ));
	    while( next. typeoftoken == token::token_comma )
            {
               next = readtoken( stream ); 
               args. push_back( readfoterm( stream, next ));
            }

	    if( next. typeoftoken != token::token_rpar )
               throw syntaxerror( "right parenthesis or comma expected", 
			          stream );

	    next = readtoken( stream );
         }
	 else
            next = readtoken( stream );
      }
   }


   firstorder::term readfoterm( inputstream& stream,
		                token& next ) throw ( syntaxerror )
   {
      
      if( next. typeoftoken != token::token_string )
         throw syntaxerror( "identifier expected", stream );

      std::string func = next. stringattr;

      next = readtoken( stream );
      
      std::list< firstorder::term > subterms;
      readfoargs( subterms, stream, next );

      firstorder::function f = firstorder::function( func, subterms. size( ));

      if( ! f. isdeclared( ))
      {
         throw syntaxerror( std::string( func ) + "/" +
		            converttostring( subterms. size( )) + 
		            " not declared as function", stream );
      }

      return firstorder::term( f, subterms );
   }

   
   firstorder::formula readfoatom( inputstream& stream,
		                   token& next ) throw( syntaxerror )
   {
      if( next. typeoftoken == token::token_lpar )
      {
         next = readtoken( stream );
	 
	 firstorder::formula f = readfoform( stream, next );
	 
	 if( next. typeoftoken != token::token_rpar )
            throw syntaxerror( "right parenthesis expected", stream );

	 next =  readtoken( stream );
	 return f;
      }

      if( next. typeoftoken == token::token_false )
      {
         next = readtoken( stream );
	 return firstorder::formula::make_false( );
      }

      if( next. typeoftoken == token::token_true )
      {
         next = readtoken( stream );
	 return firstorder::formula::make_true( );
      }

      if( next. typeoftoken != token::token_string )
         throw syntaxerror( "expecting identifier", stream );

      std::string ident = next. stringattr;
      next = readtoken( stream );

      std::list< firstorder::term > subterms;
      readfoargs( subterms, stream, next );

      if( next. typeoftoken == token::token_eq ||
          next. typeoftoken == token::token_diseq )
      {
         firstorder::function f1 = 
               firstorder::function( ident, subterms. size( ));

         if( ! f1. isdeclared( ))
         {
            throw syntaxerror( std::string( ident ) + "/" +
		               converttostring( subterms. size( )) +
		               " not declared as function", stream ); 
         }
	 firstorder::term arg1 = firstorder::term( f1, subterms );

	 token op = next;
	 next = readtoken( stream );

	 if( next. typeoftoken != token::token_string )
            throw syntaxerror( "identifier expected", stream );

	 ident = next. stringattr;
	 next = readtoken( stream );

	 subterms. clear( );
	 readfoargs( subterms, stream, next );

	 firstorder::function f2 =
               firstorder::function( ident, subterms. size( ));

         if( ! f2. isdeclared( )) 
         {
            throw syntaxerror( std::string( ident ) + "/" +
		               converttostring( subterms. size( )) +
		               " not declared as function", stream );
         }
	 firstorder::term arg2 = firstorder::term( f2, subterms );

         firstorder::formula res = 
	       firstorder::formula::make_equals( arg1, arg2 );

         if( op. typeoftoken == token::token_diseq )
            res = firstorder::formula::make_not( res );

	 return res;
      }

      // It is a usual atom. We build it.
 
      if( ! predicate::isdefined( ident, subterms. size( )))
      {
         throw syntaxerror( std::string( ident ) + "/" + 
		            converttostring( subterms. size( )) +
		            " is not declared as predicate", stream );
      }

      firstorder::formula res = 
            firstorder::formula::make_atom( 
                  predicate( ident, subterms. size( )),
	          subterms );

      return res;
   }


   namespace
   {


      firstorder::formula buildbinaryform( token op,
		                           firstorder::formula f1,
				           firstorder::formula f2 )
      {
	 switch( op. typeoftoken )
         {
         case token::token_and:
            {
               std::list< firstorder::formula > sub;
	       sub. push_back(f1); sub. push_back(f2); 
               return firstorder::formula::make_and( sub ); 
            }

         case token::token_or:
	    {
               std::list< firstorder::formula > sub;
	       sub. push_back(f1); sub. push_back(f2);
	       return firstorder::formula::make_or( sub );
            }

         case token::token_implies:
	    return firstorder::formula::make_implies( f1, f2 ); 

         case token::token_equiv:
	    return firstorder::formula::make_equiv( f1, f2 ); 

         default:
	    ASSERT( false ); exit(0);
         }
      }

     
      // This is used only for the construction of errormessages:

      std::string operatorname( token op )
      {
         switch( op. typeoftoken )
         {
         case token::token_false:
            return std::string( "FALSE" );
         case token::token_true:
            return std::string( "TRUE" );
	 case token::token_and:
            return std::string( "/\\" );
         case token::token_or:
            return std::string( "\\/" );
         case token::token_equiv:
            return std::string( "<->" );
         case token::token_implies:
            return std::string( "->" );
         case token::token_exists1:
            return std::string( "< >" );
         case token::token_forall1:
            return std::string( "[ ]" );
         }
	 return std::string( "unknown" ); 
      }


      // Decide which operator has the priority in a situation
      // F1 op1 F2 op2 F3.
      // -1 means left preference, 
      // 1 means right preference. 
      // The stream is used only for constructing a possible syntax error.

      int decidepriority( token t1, token t2, 
		          const inputstream& stream ) throw( syntaxerror )
      {
         if( t1. typeoftoken == token::token_not )
            return -1;

         if( t1. typeoftoken == token::token_and )
         {
            if( t2. typeoftoken == token::token_and ||
                t2. typeoftoken == token::token_or ||
	        t2. typeoftoken == token::token_implies ||
	        t2. typeoftoken == token::token_equiv )
               return -1;
         }

         if( t1. typeoftoken == token::token_or )
         {
            if( t2. typeoftoken == token::token_and )
               return 1;
	    if( t2. typeoftoken == token::token_or ||
                t2. typeoftoken == token::token_implies ||
	        t2. typeoftoken == token::token_equiv )
               return -1;
         }

         if( t1. typeoftoken == token::token_implies )
         {
            if( t2. typeoftoken == token::token_and ||
                t2. typeoftoken == token::token_or ||
	        t2. typeoftoken == token::token_implies )
               return 1;
         }

         if( t1. typeoftoken == token::token_equiv )
         {
            if( t2. typeoftoken == token::token_and ||
                t2. typeoftoken == token::token_or ) 
               return 1;
         }

         if( t1. typeoftoken == token::token_forall1 ||
             t1. typeoftoken == token::token_exists1 )
         {
            return 1;
         }

	 std::string err = "could not decide conflict between ";
	 err += operatorname( t1 );
	 err += " and ";
	 err += operatorname( t2 );

         throw syntaxerror( err, stream ); 
      }

   }


   firstorder::formula readquantifiedform( 
		          inputstream& stream,
			  token& next ) throw( syntaxerror )
   {
      if( next. typeoftoken != token::token_forall1 &&
          next. typeoftoken != token::token_exists1 )
      {
         throw syntaxerror( "quantifier expected", stream );
      }

      token quant = next;
      next = readtoken( stream );

      std::list< firstorder::function > vars;

      if( next. typeoftoken != token::token_forall2 && 
          next. typeoftoken != token::token_exists2 ) 
      {
         if( next. typeoftoken != token::token_string )
            throw syntaxerror( "identifier expected", stream );

	 vars. push_back( firstorder::function( next. stringattr, 0 ));

	 next = readtoken( stream );

         while( next. typeoftoken != token::token_forall2 &&
                next. typeoftoken != token::token_exists2 )
         {
	    if( next. typeoftoken != token::token_comma )
               throw syntaxerror( "comma expected", stream );
	    next = readtoken( stream );

            if( next. typeoftoken != token::token_string )
               throw syntaxerror( "identifier expected", stream );

            vars. push_back( firstorder::function( next. stringattr, 0 ));

            next = readtoken( stream );
         }
      }

      if( quant. typeoftoken == token::token_forall1 && 
          next. typeoftoken == token::token_exists2 )
      {
         throw syntaxerror( "[ terminated by >", stream );
      }

      if( quant. typeoftoken == token::token_exists1 &&
          next. typeoftoken == token::token_forall2 )
      {
         throw syntaxerror( "< terminated by ]", stream );
      }

      next = readtoken( stream );

      // We declare the quantified variables as 0-arity functions.
      // Then we parse the body.
      // After that we backtrack the function space, in order
      // to remove the quantified variables. 

      unsigned int nrdecls = firstorder::function::nrdeclarations( );
      for( std::list< firstorder::function > :: iterator 
              it = vars. begin( );
	      it != vars. end( );
	      ++ it )
      {
         it -> declare( );
      }

      firstorder::formula f = readfoform( quant, stream, next );

      firstorder::function::backtrackdeclarations( nrdecls );

      if( quant. typeoftoken == token::token_forall1 )
         return firstorder::formula::make_forall( vars, f );
      else
         return firstorder::formula::make_exists( vars, f ); 

   }


   firstorder::formula readfoform( inputstream& stream,
		                   token& next ) throw( syntaxerror )
   {
      firstorder::formula f1 = firstorder::formula::make_false( );

      if( next. typeoftoken == token::token_exists1 ||
          next. typeoftoken == token::token_forall1 )
      {
         f1 = readquantifiedform( stream, next );
      }
      else
      {
         if( next. typeoftoken == token::token_not )
	 {
	    token pref = next;
	    next = readtoken( stream );
	    f1 = readfoform( pref, stream, next );
	    f1 = firstorder::formula::make_not( f1 ); 
         }
	 else 
         {
            if( next. typeoftoken == token::token_lpar ||
                next. typeoftoken == token::token_string ||
	        next. typeoftoken == token::token_false ||
	        next. typeoftoken == token::token_true )
            {
               f1 = readfoatom( stream, next );
	    }
	    else
               throw syntaxerror( "bad start of formula", stream );
         }
      }

      while( next. typeoftoken != token::token_rpar &&
             next. typeoftoken != token::token_dot &&
	     next. typeoftoken != token::token_comma )
      {
         token op = next;
	 next = readtoken( stream );

	 if( op. typeoftoken != token::token_and &&
             op. typeoftoken != token::token_or &&
	     op. typeoftoken != token::token_implies &&
	     op. typeoftoken != token::token_equiv )
         {
            throw syntaxerror( "expecting binary operator", stream );
         }
	 firstorder::formula f2 = readfoform( op, stream, next );

	 // If next is an operator it should have smaller priority.

	 if( next. typeoftoken == token::token_and ||
             next. typeoftoken == token::token_or ||
	     next. typeoftoken == token::token_implies ||
	     next. typeoftoken == token::token_equiv )
         {
            ASSERT( decidepriority( op, next, stream ) == -1 );
         }

         f1 = buildbinaryform( op, f1, f2 ); 

      }
      return f1;
   }


   firstorder::formula readfoform( token op1,
		                   inputstream& stream,
		                   token& next ) throw( syntaxerror )
   {
      firstorder::formula f1 = firstorder::formula::make_false( );

      if( next. typeoftoken == token::token_exists1 ||
          next. typeoftoken == token::token_forall1 )
      {
         f1 = readquantifiedform( stream, next );
      }
      else
      {
         if( next. typeoftoken == token::token_not )
         {
            token pref = next;
	    next = readtoken( stream );
	    f1 = readfoform( pref, stream, next );
	    f1 = firstorder::formula::make_not( f1 );
         }
	 else
         {
            if( next. typeoftoken == token::token_lpar ||
                next. typeoftoken == token::token_string ||
	        next. typeoftoken == token::token_false ||
	        next. typeoftoken == token::token_true )
            {
               f1 = readfoatom( stream, next );
            }
            else
               throw syntaxerror( "garbage at atom position", stream );
         }
      }

      while( ( next. typeoftoken == token::token_and ||
               next. typeoftoken == token::token_or ||
	       next. typeoftoken == token::token_implies ||
	       next. typeoftoken == token::token_equiv ) &&
             decidepriority( op1, next, stream ) == 1 )
      {
         token op2 = next;
	 next = readtoken( stream );

	 firstorder::formula f2 = readfoform( op2, stream, next );
         f1 = buildbinaryform( op2, f1, f2 ); 
      }
      return f1;
   }


   void readpredicatedeclaration( inputstream& stream,
		                  token& next ) throw( syntaxerror )
   {
      if( next. typeoftoken != token::token_string )
         throw syntaxerror( "identifier expected", stream );

      std::string s = next. stringattr;
      
      next = readtoken( stream );
      if( next. typeoftoken != token::token_slash )
         throw syntaxerror( "slash expected", stream );

      next = readtoken( stream );
      if( next. typeoftoken != token::token_unsigned )
         throw syntaxerror( "unsigned int expected", stream );

      unsigned int ar = next. unsignedattr;

      (void) predicate( s, ar );

      next = readtoken( stream );
   }


   void readfunctiondeclaration( inputstream& stream,
		                 token& next ) throw( syntaxerror )
   {
      if( next. typeoftoken != token::token_string )
         throw syntaxerror( "identifier expected", stream );

      std::string s = next. stringattr;
      
      next = readtoken( stream );
      if( next. typeoftoken != token::token_slash )
         throw syntaxerror( "slash expected", stream );

      next = readtoken( stream );
      if( next. typeoftoken != token::token_unsigned )
         throw syntaxerror( "unsigned int expected", stream );

      unsigned int ar = next. unsignedattr;

      firstorder::function f = firstorder::function( s, ar );
      f. declare( );

      next = readtoken( stream );
   }



   bool isheader( const token& t )
   {
      if( t. typeoftoken == token::token_rulesystem ||
          t. typeoftoken == token::token_model ||
	  t. typeoftoken == token::token_firstorder || 
	  t. typeoftoken == token::token_predicates ||
	  t. typeoftoken == token::token_functions || 
	  t. typeoftoken == token::token_end ||
	  t. typeoftoken == token::token_eof )
      {
         return true;
      }
      return false;
   }


   void readproblem( inputstream& stream,
		     model& m, rulesystem& rs,
		     firstorder::sequent& s ) throw( syntaxerror )  
   {

      token next = readtoken( stream );

#if 0
      // Just read the tokens and print them. For debugging
      // the tokenizer.

      while( next. typeoftoken != token::token_eof &&
             next. typeoftoken != token::token_error ) 
      {
	 std::cout << next << " ";
	 next = readtoken( stream );
      }

      std::cout << next << "\n\n";
      return;
#endif

   toplevel:
      if( next. typeoftoken == token::token_eof )
         return;

      if( next. typeoftoken == token::token_rulesystem )
      {
         next = readtoken( stream ); 

	 while( ! isheader( next ))
         {
            rule r = readrule( stream, next );
	    std::vector< rulesystem::binary_step > derivation; 

            if( ! r. iswellformed( ))
               throw syntaxerror( 
			 "rule is not well-formed(this is not a syntax error)",
			                       stream );

	    rs. addrule( r, derivation, false );

	    if( next. typeoftoken != token::token_dot )
               throw syntaxerror( "dot expected", stream );

	    next = readtoken( stream );
         }

	 goto toplevel;
      }

      if( next. typeoftoken == token::token_model )
      {
         next = readtoken( stream );

	 while( ! isheader( next ))
         {
            groundatom ga = readgroundatom( stream, next );
	    std::vector< unsigned int > parents;

	    if( ! m. canbeadded( ga ))
               throw syntaxerror( "undeclared element", stream );

	    m. addatom( ga, 9999999, parents );

	    if( next. typeoftoken != token::token_dot &&
                next. typeoftoken != token::token_comma )
            {
               throw syntaxerror( "comma or dot expected", 
			                       stream );
            }
	    next = readtoken( stream );
         }

	 goto toplevel;
      }

      if( next. typeoftoken == token::token_firstorder )
      {
         next = readtoken( stream );
	 while( ! isheader( next ))
         {
            firstorder::formula f = readfoform( stream, next );
	    if( next. typeoftoken != token::token_dot )
            {
               throw syntaxerror( 
			"formula should have been followed by a dot", stream );
            }
	    next = readtoken( stream );

	    s. formulas. push_back(f); 
         }

	 goto toplevel; 
      }

      if( next. typeoftoken == token::token_predicates )
      {
         next = readtoken( stream );
	 while( ! isheader( next ))
         {
            readpredicatedeclaration( stream, next );
	    if( next. typeoftoken != token::token_dot &&
                next. typeoftoken != token::token_comma )
            {
               throw syntaxerror( "comma or dot expected", stream );
            }
	    next = readtoken( stream );
         }

	 goto toplevel;
      }

      if( next. typeoftoken == token::token_functions )
      {
         next = readtoken( stream );
	 while( ! isheader( next ))
         {
            readfunctiondeclaration( stream, next );
	    if( next. typeoftoken != token::token_dot &&
                next. typeoftoken != token::token_comma )
            {
               throw syntaxerror( "comma or dot expected", stream );
            }
	    next = readtoken( stream );
         }

	 goto toplevel;
      }

      if( next. typeoftoken == token::token_end )
         return;

      std::cout << next << "\n";
      throw syntaxerror( "unknown header", stream ); 
   }

} 



