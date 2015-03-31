

// Code written by Hans de Nivelle, April 2003


#ifndef PARSER_INPUTSTREAM_DECLARED 
#define PARSER_INPUTSTREAM_DECLARED  1


#include <string>
#include <iostream> 
#include <stdio.h>


// An inputstream adds counters (counting line and column) to an 
// istream, and it allows for easy inspection of the next character.


namespace parser
{

   class inputstream
   {
   public:
      std::istream& stream;

   public: 
      unsigned int linenumber;
      unsigned int columnnumber;
         // Both apply to nextchar. They are intended for use in 
         // error-messages. 

      int nextchar;
         // When the istream is closed, the nextchar is pushed back
         // into the stream.

      const std::string name;
         // Intended for use in error-messages. 


      bool good( ) const { return stream. good( ); }
      bool eof( ) const { return stream. eof( ); } 
      bool fail( ) const { return stream. fail( ); } 
      bool bad( ) const { return stream. bad( ); } 

      operator bool ( ) const { return ( stream && ( nextchar != EOF )); }
      bool operator ! ( ) const { return ( ! stream || ( nextchar == EOF )); }

   public:
      void moveforward( );
         // Move nextchar one character forward in the stream.  

   public:
      inputstream( std::istream& istream = std::cin, 
                   const std::string& name = "STDIN",
                   unsigned int linenumber = 1,
                   unsigned int columnnumber = 1 );

         // Create inputstream, set name, linenumber and columnumber.
         // Counting for the line- and columnnumber starts at 1. 
         // One must be somewhat careful that the stream (because it
	 // cannot be copied) is stored as a reference. 

      ~inputstream( ); 
         // The next seen character, which was read already from the stream,
         // will be pushed back into the stream, unless it is EOF. 
         // The stream is not closed. 


      inputstream( const inputstream& );
      void operator = ( const inputstream& );
         // Have no definition.

      std::string inlineoffile( ) const;
         // Create the string 'in line L of file F'.
         // This is useful for error messages. 
   };

} 


#endif


