

// Written by Ravinder Indarapu and Hans de Nivelle in March 2003.
// Somewhat modified in September 2006.


#ifndef PROLOG_SYNTAX_INCLUDED
#define PROLOG_SYNTAX_INCLUDED  1


#include <map>
#include <iostream>


#include "../assert.h"
#include "syntacticdefsforidentifier.h"
#include "funcpred.h"




// A syntax determines in which format terms are read and printed.
// Its main component are the definitions of Prolog-style 
// prefix/infix/postfix operator definitions.
// In addition, it defines:
//
// - which operators are used for constructing lists. 
//   The defaults are dot (.) and nil ([]). 
// 
//  - Whether list syntax is used when printing a term. List syntax is 
//    always accepted as input.
//
// - Up to which depth terms are printed. This does not affect the 
//   depth of terms that can be read. 



namespace prolog 
{

   class syntax
   {

   private:

      std::map < std::string, syntacticdefsforidentifier > hh; 


//  Function that is used for representing the empty list.
//  Should be 0-arity. 

   public: 
      funcpred nil;


//  Function that used for constructing lists. Should be
//  2-arity. 

      funcpred cons;


//  If true, then list notation is used when printing output.
//  List notation is always accepted as input.

      bool printlistsyntax;

//  The maximal depth up to which terms are printed. A subtree that
//  occurs at a depth deeper than maxprintdepth, is printed as
//  <...> . One could argue that this is not part of the syntax,
//  but I don't know a better place to put it.

      unsigned int maxprintdepth;

// Which kind of quotes are used when printing dangerous identifiers.
// If doublequotes == true, it will be ", otherwise '

      bool doublequotes;


//  Create a new empty syntax. 
//  The syntax is initially empty, the default nil is [], and the
//  default cons is the dot. By default, printlistsyntax = true. The
//  default value for maxprintdepth is +infinity. 

      syntax( );


//  Adds a definition to this syntax. If the syntax contained a definition
//  that is inconsistent with this one, then it is overwritten, and
//  true is returned. 

      bool insert( const syntacticdef& sd );


//  Delete sd from this syntax, if it occurs. Otherwise do nothing.
//  True is returned in case a deletion took place. 

      bool remove( const syntacticdef& sd );


// True if the string has a definition of the required type in this
// syntax.

      bool hasprefixdef( const std::string& ident ) const;
      bool hasinfixdef( const std::string& ident ) const;
      bool haspostfixdef( const std::string& ident ) const;


// Get the required definition if there is one. 

      const syntacticdef& getprefixdef( const std::string& ident ) const;
      const syntacticdef& getinfixdef( const std::string& ident ) const;
      const syntacticdef& getpostfixdef( const std::string& ident ) const;

      friend std::ostream& operator << ( std::ostream&, const syntax& );  
   };
   
}

#endif


