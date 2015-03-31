
// Code written by Hans de Nivelle, April 2003.


#ifndef PROLOG_DFACLASSIFICATION_DECLARED
#define PROLOG_DFACLASSIFICATION_DECLARED   1


#include <string>


namespace prolog 
{

   namespace DFAclassification
   {

      bool inoperator( char x );
         // True if the character can occur in an operator.

      bool inname( char x );
         // True if the character can occur in a name.

      bool startname( char x );
         // True if a name can start with this character.

      bool inquotedname( char x );
         // True if the character can occur in a quotedname.

      bool incommentpercent( char x );
         // True if the character is allowed in comment that starts
         // with a '%'.


      bool incommentunix( char x );  
         // True if the character is allowed in Unix-style comment of form
         // / * ----    * /

      
      bool nameneedsquotes( const std::string& s );   
         // This function decides whether string s needs quotes in order to
         // be scanned correctly as a NAME, QUOTEDNAME, GLUEDNAME 
         // or GLUEDQUOTEDNAME.

   } 

} 


#endif


