

// Code written by Hans de Nivelle, April 2003.
// Corrected a bug on August 4th 2005, which caused the
// scanner to hang in case of unrecognizable input. 


#include "scanner.h"


namespace prolog 
{


   void scanner::runDFA( parser::inputstream& stream, 
                         std::string& readstring, 
                         DFAstate& laststate ) throw( )
   {
      laststate = DFAstate::initial;
      readstring. clear( ); 

      while( stream )
      {
         char c = stream. nextchar; 
         DFAstate nextstate = DFAdelta::transition( laststate, c );
         if( nextstate == DFAstate::blocked )
            return;

         laststate = nextstate;
         readstring += c; 
         stream. moveforward( ); 
      }

    }
   


   token scanner::readtoken( parser::inputstream& stream )
   throw( parser::syntaxerror, parser::readerror )
   {
      DFAstate laststate;
      std::string scanned;

      if( stream )
      {
         do
         {
            runDFA( stream, scanned, laststate );
         }
         while( stream &&
                ( laststate == DFAstate::whitespace0 ||
                  laststate == DFAstate::percent1 ||
                  laststate == DFAstate::div3 ) );
      }
        
      if( ! stream )
      {
         if( stream. bad( ) || ! stream. eof( ) )
            throw parser::readerror( "could not read", stream );
         else
            return token( token::endoffile );
      }

      switch( laststate. state )
      {
      case DFAstate::lpar0:
         return token( token::lpar );

      case DFAstate::rpar0:
         return token( token::rpar );

      case DFAstate::rsqpar0:
         return token( token::rsqpar );

      case DFAstate::quoted2:
         removequotes( scanned ); 
         if( stream. nextchar == '(' )
            return token( token::gluedquotedidentifier, scanned );
         else
            return token( token::quotedidentifier, scanned );

      case DFAstate::quoted5:
	 removequotes( scanned );
	 if( stream. nextchar == '(' )
            return token( token::gluedquotedidentifier, scanned );
	 else
            return token( token::quotedidentifier, scanned );

      case DFAstate::nil0:
         return token( token::lsqpar );

      case DFAstate::comma0:
      case DFAstate::nil1:
      case DFAstate::div0:
      case DFAstate::name0:
      case DFAstate::operator0:
         if( stream. nextchar == '(' )
            return token( token::gluedidentifier, scanned );
         else
            return token( token::identifier, scanned );

      }

      // We should avoid scanning an empty string, because in that
      // case error recovery will never move forward. 

      if( scanned. size( ) == 0 )
      {
         if( stream ) 
         {
            scanned += stream. nextchar;
	    stream. moveforward( );
	 }
      }
      return token( token::error, scanned );  
   }


   void scanner::removequotes( std::string& s )
   {
      ASSERT( s. size( ) >= 2 );
      ASSERT( ( s[0] == '\'' && s [ s. size( ) - 1 ] == '\'' ) ||
              ( s[0] == '\"' && s [ s. size( ) - 1 ] == '\"' ) );

      s. erase(0,1); 
      s. erase( s. size( ) - 1, s. size( )); 
   }


} 



