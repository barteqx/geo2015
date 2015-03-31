

// Code written by Hans de Nivelle, April 2003.


#include "DFAclassification.h"


namespace prolog
{

   namespace DFAclassification
   {

      bool inoperator( char x )
      {
         if( x == '~' || x == '!' || x == '@' ) return true;

         if( x == '#' || x == '$' || x == '^' ) return true;

         if( x == '&' || x == '*' ) return true;

         if( x == '-' || x == '+' || x == '=' ) return true;

         if( x == '\\' || x == '/' ) return true;

         if( x == ':' || x == ';' || x == '|' ) return true;

         if( x == '{' || x == '}' ) return true;

         if( x == '<' || x == '>' ) return true;

         if( x == '?' || x == '.' ) return true;

         return false;
      }



      bool inname( char x )
      {
         if( x >= 'a' && x <= 'z' ) return true;

         if( x >= 'A' && x <= 'Z' ) return true;

         if( x >= '0' && x <= '9' ) return true;

         if( x == '_' ) return true;
  
         return false;
      }


      bool startname( char x )
      {
         if( x >= 'a' && x <= 'z' ) return true;

         if( x >= 'A' && x <= 'Z' ) return true;
                  
         if( x >= '0' && x <= '9' ) return true;

         return false;
      }


      bool inquotedname( char x )
      {
         if( x == '\n' ) return false;

         if( x == '"' || x == '\'' ) return false;
     
         return true;
      }


      bool incommentpercent( char x )
      {
         if( x == '\n' ) return false;

         return true;
      }


      bool incommentunix( char x )
      {
         if( x == '*' ) return false;
     
         return true;
      }

   
      

      bool nameneedsquotes( const std::string& s )
      {
         int length = s. size( );

         if( s == "," )
            return false;

         {
            bool needsquotes = false;

            if( ! startname( s[0] ))
               needsquotes = true;

            for( int i = 1; i < length; i ++ )
            {
               if( ! inname( s[i] ))
                  needsquotes = true;
            }
            if( ! needsquotes )
               return false;
         }


         if( s == "[]" )
            return false;

         {
            bool needsquotes = false;
            for( int i = 0; i < length; i ++ )
            {
               if( ! inoperator( s[i] ))
                  needsquotes = true;
            }

            for( int i = 0; i < length - 1; i ++ )
            {
               if( s[i] == '/' && s[i+1] == '*' )
                  needsquotes = true;
            }
            if( ! needsquotes )
               return false;
         }

         return true;
      }

   } 
} 
 
