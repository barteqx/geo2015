

// Code written by Hans de Nivelle, April 2003.
// Modified for bliksem 130 in september 2006. 


#include "../assert.h"

#include "DFAdelta.h"
#include "DFAclassification.h"


namespace prolog 
{

   // One should use transition, defined below, because
   // that is much more efficient than this one.

   DFAstate DFAdelta::transition_uncached( DFAstate s, char c )
   {
      
      if( s == DFAstate::initial )
      {
         if( c == '(' )
            return DFAstate::lpar0;
         if( c == ')' )
            return DFAstate::rpar0;
         if( c == ',' )
            return DFAstate::comma0;
         if( c == ']' )
            return DFAstate::rsqpar0;

         if( c == ' ' )
            return DFAstate::whitespace0;
         if( c == '\t' )
            return DFAstate::whitespace0;
         if( c == '\n' )
            return DFAstate::whitespace0;
         if( c == 0X0D )
            return DFAstate::whitespace0;  
	       // To avoid problems with windows-files. 

         if( c == '%' )
            return DFAstate::percent0;
         if( c == '\"' )
            return DFAstate::quoted0;
         if( c == '\'' )
            return DFAstate::quoted3;
         if( c == '[' )
            return DFAstate::nil0;
         if( c == '/' )
            return DFAstate::div0;

         if( DFAclassification::startname(c) )
            return DFAstate::name0;

         if( DFAclassification::inoperator(c) )
            return DFAstate::operator0;

      }
 
      if( s == DFAstate::percent0 )
      {
         if( c == '\n' )
            return DFAstate::percent1;

	 if( DFAclassification::incommentpercent(c) )
            return DFAstate::percent0;
      }

      if( s == DFAstate::quoted0 ) 
      {
         if( DFAclassification::inquotedname(c) )
            return DFAstate::quoted1;
      }

      if( s == DFAstate::quoted1 )
      {
         if( DFAclassification::inquotedname(c) )
            return DFAstate::quoted1;

	 if( c == '\"' )
            return DFAstate::quoted2;
      }

      if( s == DFAstate::quoted3 )
      {
         if( DFAclassification::inquotedname(c) )
            return DFAstate::quoted4;
      }

      if( s == DFAstate::quoted4 )
      {
         if( DFAclassification::inquotedname(c) )
            return DFAstate::quoted4;

	 if( c == '\'' )
            return DFAstate::quoted5;
      }

      if( s == DFAstate::nil0 )
      { 
         if( c == ']' )
            return DFAstate::nil1;
      }

      if( s == DFAstate::operator0 )
      {
         if( DFAclassification::inoperator(c) )
            return DFAstate::operator0;
      }

      if( s == DFAstate::div0 )
      {
         if( c == '*' )
            return DFAstate::div1;

	 if( DFAclassification::inoperator(c) )
            return DFAstate::operator0;
      }

      if( s == DFAstate::div1 )
      {
         if( c == '*' )
            return DFAstate::div2;

	 if( DFAclassification::incommentunix(c) )
            return DFAstate::div1;
      }

      if( s == DFAstate::div2 )
      {
         if( c == '/' )
            return DFAstate::div3;

         if( c == '*' )
            return DFAstate::div2;

         if( DFAclassification::incommentunix(c) )
            return DFAstate::div1;
      }

      if( s == DFAstate::name0 )
      {
         if( DFAclassification::inname(c) )
            return DFAstate::name0;
      }

      return DFAstate::blocked;
   }


   // transition first looks in its internal table. If it finds 
   // nothing there, it calls transition_uncached( ) 
   // and stores the result in table [] [] for the
   // next time.

   DFAstate DFAdelta::transition( DFAstate s, char c )
   {

      static DFAstate table [ DFAstate::maxstate ] [ maxcharacter ]; 

      int x = c;
      int y = s. state;

      ASSERT( y >=0 && y < DFAstate::maxstate );
      ASSERT( x >= 0 && x < 256 );

      if( x >= maxcharacter )
         return DFAstate::blocked;

      DFAstate val = table [y] [x];
      if( val == DFAstate::unknown )
      {
         val = transition_uncached( s, c );
	 table [y] [x] = val;
      }
      return val;
   }



   // For debugging: 

   void DFAdelta::printtable( 
		      DFAstate table [ DFAstate::maxstate ] [ maxcharacter ] )
   {
      std::cout << "The following DFA-transitions have been cached:\n\n"; 

      for( unsigned int i = 0; i < DFAstate::maxstate; i ++ ) 
         for( unsigned int j = 0; j < maxcharacter; j ++ ) 
         {
            if( table [i] [j] != DFAstate(DFAstate::unknown ) )
            {
               std::cout << "delta(" << DFAstate(i) << ",";
               std::cout << static_cast<char>(j) << ") = ";
               std::cout << table [i] [j] << "\n";
            }
         }
      std::cout << "\n\n";
   }
         
} 



