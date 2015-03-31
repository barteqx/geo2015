

// Code written by Hans de Nivelle, April 2003.
// Modified for Bliksem September 2006 by Hans de Nivelle. 


#ifndef PROLOG_SCANNER_INCLUDED
#define PROLOG_SCANNER_INCLUDED  1


#include "../assert.h"
#include "DFAstate.h"
#include "DFAdelta.h"
#include "token.h"
#include "../parser/inputstream.h"
#include "../parser/errors.h"



namespace prolog 
{

   class scanner
   {

   private:
      static void runDFA( parser::inputstream& stream, 
		          std::string& readstring, 
                          DFAstate& laststate ) throw( ); 

      // Run the DFA described by the DFA-class from DFAstate::initial 
      // until it gets stuck, end-of-file is encountered, or a 
      // read problem occurs. 


      static void removequotes( std::string& ); 
         // removes the quotes from the string. The string should 
         // start and end with a double quote, or should start
	 // and end with a single quote. 

   public:
      static token readtoken( parser::inputstream& stream ) 
      throw( parser::syntaxerror, parser::readerror ); 

   };

} 


#endif


