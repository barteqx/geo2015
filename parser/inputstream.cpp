

// Code written by Hans de Nivelle, April 2003. 


#include "inputstream.h"

#include <sstream> 


namespace parser
{


   // If the stream is OK, and we did not reach EOF (which is implied
   //  by the fact that the stream is OK), then we read a character and
   // put it in nextchar.


   void inputstream::moveforward( ) 
   {
      if( stream && nextchar != EOF )
      {
      
         if( nextchar == '\t' )
            columnnumber += 5;         // An estimation. 
         else
         { 
            if( nextchar == '\n' ) 
            {
               linenumber ++ ;
               columnnumber = 1;
            }
            else
               columnnumber ++ ;
         }

         nextchar = stream. get( );
      }
   }


 
   inputstream::inputstream( std::istream& stream,
                             const std::string& name, 
                             unsigned int linenumber,
                             unsigned int columnnumber ) :
      stream( stream ),
      linenumber( linenumber ), 
      columnnumber( columnnumber - 1 ),
      nextchar( ' ' ),
      name( name ) 
   {  
      moveforward( );
   }


   
   inputstream::~inputstream( ) 
   {
      if( nextchar != EOF ) 
         stream. putback( nextchar );
   }
 
      

   std::string inputstream::inlineoffile( ) const
   {
      std::ostringstream s;

      s << "in line " << linenumber;
      s << " of file " << name;
      return s. str( );
   }

} 



