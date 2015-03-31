

// Code written by Hans de Nivelle, April 2006. 
// Modified for first order June 2006.


#ifndef PARSER_TOKEN_INCLUDED
#define PARSER_TOKEN_INCLUDED   1


#include "../assert.h" 

#include <string>
#include <iostream>


namespace parser
{
     
   struct token
   {

      // Possible types of token:

      const static unsigned int token_lpar =      1; 
      const static unsigned int token_rpar =      2; 
      const static unsigned int token_comma =     3;
      const static unsigned int token_dot =       4;
      const static unsigned int token_slash =     5;


      const static unsigned int token_false =     10;
      const static unsigned int token_true =      11;
      const static unsigned int token_not =       12;
      const static unsigned int token_and =       13;
      const static unsigned int token_or =        14;
      const static unsigned int token_implies =   15;
      const static unsigned int token_equiv =     16;
      const static unsigned int token_forall1 =   17;
      const static unsigned int token_forall2 =   18;
      const static unsigned int token_exists1 =   19;
      const static unsigned int token_exists2 =   20; 
      const static unsigned int token_create =    21; 
      const static unsigned int token_diseq =     22;
      const static unsigned int token_eq =        23;


      const static unsigned int token_rulesystem =   30;
      const static unsigned int token_model =        31;
      const static unsigned int token_firstorder =   32; 
      const static unsigned int token_predicates =   33;
      const static unsigned int token_functions =    34;
      const static unsigned int token_end =          35;


      const static unsigned int token_string =       40;
      const static unsigned int token_unsigned =     41;

      const static unsigned int token_error =        50; 
      const static unsigned int token_eof =          51;


      unsigned int typeoftoken; 
      unsigned int unsignedattr;
      std::string stringattr;

      token( unsigned int typeoftoken )
         : typeoftoken( typeoftoken ),
           unsignedattr( 0 ),
	   stringattr( std::string( ))
      {
         ASSERT( typeoftoken != token_string && 
                 typeoftoken != token_unsigned ); 
      }


      token( unsigned int typeoftoken,
             unsigned int unsignedattr )
         : typeoftoken( typeoftoken ),
           unsignedattr( unsignedattr )
      {
         ASSERT( typeoftoken == token_unsigned ); 
      }


      token( unsigned int typeoftoken,
             const std::string& stringattr ) 
         : typeoftoken( typeoftoken ),
           unsignedattr( 0 ),
	   stringattr( stringattr )
      {
         ASSERT( typeoftoken == token_string ); 
      }

       
   };


   std::ostream& operator << ( std::ostream& , const token& t );

} 


#endif


