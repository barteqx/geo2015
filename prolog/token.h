

// Class token defines the tokens of the grammar, both the terminals
// and non-terminals.
// 
// Original code was written by Ravinder Indarapu and Hans de Nivelle in 
// March 2003.
// It was modified for Bliksem in September 2006. 


#ifndef PROLOG_TOKEN_INCLUDED
#define PROLOG_TOKEN_INCLUDED   1


#include <iostream>
#include <list>


#include "expression.h"
#include "syntacticdef.h"
#include "attribute.h"




namespace prolog
{

   class token
   {

   public:


   // Type is one of the unsigneds listed below.

      unsigned int type;


   // the actual attributes are defined in attributes.h 

      const attribute* attr;


   // String attribute: 

      static const unsigned int identifier =                   100;
      static const unsigned int quotedidentifier =             101;
      static const unsigned int gluedidentifier =              102;
      static const unsigned int gluedquotedidentifier =        103;
      static const unsigned int error =                        104;


   // No attribute:  

      static const unsigned int lpar =                  200;
      static const unsigned int rpar =                  201;
      static const unsigned int lsqpar =                202;
      static const unsigned int rsqpar =                203;
      static const unsigned int endoffile =             204;
      static const unsigned int dot =                   205;
      static const unsigned int comma =                 206;
      static const unsigned int bar =                   207;
      static const unsigned int start =                 208;


   // Syntacticdef attribute:
 
      static const unsigned int prefix =          300;
      static const unsigned int infix =           301;
      static const unsigned int postfix =         302;


   // Expression attribute:

      static const unsigned int input =              400;
      static const unsigned int expr =               401;


   // List of expressions attribute:

      static const unsigned int args =               500;
      static const unsigned int list =               501;


      token( unsigned int type, const std::string& );  
      token( unsigned int type );
      token( unsigned int type, const syntacticdef& );
      token( unsigned int type, const expression& );
      token( unsigned int type, const std::list< expression > & );


      token( const token& );
      void operator = ( const token& );
       ~token( );

   public: 

      const std::string& getstring( ) const ;
      const syntacticdef& getsyntacticdef( ) const;
      const expression& getexpression( ) const;
      const std::list< expression > & getexpressionlist( ) const;

      friend std::ostream & operator << ( std::ostream& os, const token& t );


   private: 

   // These functions speak about the unsigned integers defined above.

      static bool has_string( unsigned int x );
      static bool has_nothing( unsigned int x );
      static bool has_syntacticdef( unsigned int x ); 
      static bool has_expression( unsigned int x ); 
      static bool has_expressionlist( unsigned int x );


   // Deletes the attribute.

      static void delete_attribute( ); 
   };


   void testtoken( ); 
 
}


#endif


