

// Code written by Ravinder Indarapu and Hans de Nivelle, April 2003.
// Modified for Bliksem September 2006.


#ifndef PROLOG_GRAMMARRULE_INCLUDED
#define PROLOG_GRAMMARRULE_INCLUDED   1


#include <iostream>
#include <vector>


#include "../assert.h" 
#include "syntax.h"
#include "token.h"


namespace prolog 
{

   class grammarrule
   {

   private:
      unsigned int type;


   public:

      static const unsigned int input_expr_dot     =                     100;

      static const unsigned int expr_identifier   =                      200;
      static const unsigned int expr_quotedidentifier =                  201;
      static const unsigned int expr_gluedidentifier_lpar_args_rpar  =   202;
      static const unsigned int 
	              expr_gluedquotedidentifier_lpar_args_rpar =        203;
      static const unsigned int expr_lsqpar_list_rsqpar  =               204;
      static const unsigned int expr_prefix_expr         =               205;
      static const unsigned int expr_expr_infix_expr     =               206;
      static const unsigned int expr_expr_postfix        =               207;
      static const unsigned int expr_lpar_expr_rpar      =               208;

      static const unsigned int args_expr             =                  300;
      static const unsigned int args_args_comma_expr  =                  301;

      static const unsigned int list_expr             =                  400;
      static const unsigned int list_expr_bar_expr    =                  401;
      static const unsigned int list_expr_comma_list  =                  402;

      static const unsigned int dot_identifier        =                  500;

      static const unsigned int comma_identifier          =              600;
      static const unsigned int comma_gluedidentifier     =              601;

      static const unsigned int bar_identifier       =                   700;
      static const unsigned int bar_gluedidentifier  =                   701;

      static const unsigned int infix_identifier   =                     800;

      static const unsigned int prefix_identifier  =                     900;

      static const unsigned int postfix_identifier =                     1000;

      // Create a grammarrule.

      grammarrule( unsigned int type );


      unsigned int righthandsidelength( ) const;
         // Return length of the righthandside of the rule.


      // reducible( ) is true if rhs is a vector of tokens of the right
      // length and types in order to enable reduction of this rule. 
      // reducible( ) makes all necessary checks. If 
      // reducible( const syntax& ss, std::vector<token> & rhs ) 
      // returns true, then 
      // reduce( const syntax& ss, std::vector<token> & rhs ) is  
      // guaranteed to succeed.

      bool reducible( const syntax& ss, 
                      const std::vector<token> & rhs ) const;


      // rhs should be a vector of tokens of the right types and length. 
      // reduce applies this rule, and constructs the token
      // into which rhs reduces. reduce( ) checks nothing!
      // All necessary checks are made by reducible( ).

      token reduce( const syntax& ss, const std::vector<token>& rhs ) const;



      friend std::ostream& operator << ( std::ostream& os, 
                                         const grammarrule& rule );


   };
    
   std::ostream& operator << ( std::ostream& os, 
                               const grammarrule& rule ); 

} 


#endif


