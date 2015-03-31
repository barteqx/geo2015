

// Code written by Ravinder Indarapu and Hans de Nivelle, April 2003.


#ifndef PROLOG_CONTEXT_INCLUDED
#define PROLOG_CONTEXT_INCLUDED  1


#include <iostream>
#include <string>

#include "../assert.h"
#include "token.h"


// Contexts are used by the parser for deciding what to do with
// '|' , ',' and '.' . For example, a ',' is reduced into COMMA
// only when it occurs in a list context. A '.' is reduced into
// DOT only when it occurs on TOPLEVEL.
// Without Contexts it would be impossible to type things like
// '[ a, (,), b ]' or ' (.) . '
//
// Contexts are also used by the printer in order to decide
// whether or not Trees built by '|', ',' and '.' need to
// be parenthesized.


namespace prolog
{

   class context
   {
   private:
      unsigned int value;

   public:
      static const unsigned int toplevel              = 1000;
      static const unsigned int parentheses           = 1001;


      // Only purpose of AFTERGLUED is to distinguish
      // the '(' belonging to a function application from a free 
      // // occurring '('.

      static const unsigned int afterglued            = 1002;
      static const unsigned int funcarg               = 1003;
      static const unsigned int listarg               = 1004;
      static const unsigned int listafterbar          = 1005;


      explicit context( unsigned int x ) : value( x ) 
      {
      }

      context( );
         // Has no definition.


      // If the i-th element of the parsestack has this Context, and
      // the (i+1)-th element has token t, then the (i+1)-th element gets
      // the context, returned by transition(t)

      context transition( const token& t ) const;


      // True if this context allows the string to fulfill its special role.
      // This is the case only if one of the following holds:
      // s == "." and this == toplevel,
      // s == "," and this == funcarg or listarg, 
      // s == "|" and this == listarg. 

      bool allows( const char* s ) const;
    
      friend std::ostream& operator << ( std::ostream& , context ); 
      friend bool operator == ( context c1, context c2 ); 
   };

   std::ostream& operator << ( std::ostream& , context c );
   bool operator == ( context c1, context c2 );

} 

#endif

