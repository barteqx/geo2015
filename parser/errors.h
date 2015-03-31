
// this file defines syntaxerror and readerror. A readerror 
// means that reading of input was attempted and failed, for some
// reason different than encountering end of file.
//
// A syntaxerrors means that it was attempted to read a term, 
// that enough characters were successfully read, but that they could not
// be parsed according to the input grammar.
//
// Code written by Hans de Nivelle, April 2006.


#ifndef PARSER_ERRORS_DECLARED
#define PARSER_ERRORS_DECLARED    1 


#include <string>
#include <iostream>

#include "inputstream.h" 


namespace parser
{

   struct syntaxerror
   {
      std::string problem;
      
      std::string streamname;
      unsigned int linenumber;


      // The description of the problem should be such that it can
      // be followed by 'in line ... of file ...'


      syntaxerror( const std::string& problem, 
                   const inputstream& stream ) : 
         problem( problem ), 
         streamname( stream. name ),
         linenumber( stream. linenumber ) 
      {
      }

         

   };

   std::ostream& operator << ( std::ostream&, const syntaxerror& ); 
 


   // A readerror is thrown when there is a problem with the inputstream,
   // and the parser cannot read from it.


   struct readerror
   { 

      std::string problem;

      std::string streamname; 
      unsigned int linenumber; 


      // The description of the problem should be such that it can
      // be followed by 'in line ... of file ...'


      readerror( const std::string& problem, 
                 const inputstream& stream ) : 
         problem( problem ), 
         streamname( stream. name ),
         linenumber( stream. linenumber )  
      {
      }

   

   };


   std::ostream& operator << ( std::ostream&, const readerror& );

} 


#endif


