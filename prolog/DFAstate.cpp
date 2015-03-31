
// States of the DFA implementing the Tokenizer.
// Code written by Hans de Nivelle, April 2003.


#include "DFAstate.h"



namespace prolog 
{


   std::ostream& operator << ( std::ostream& stream, DFAstate x )
   {

      switch( x. state )
      {
      case DFAstate::initial:
         stream << "initial";
         break;

      case DFAstate::lpar0:
         stream << "lpar0";
         break;

      case DFAstate::rpar0:
         stream << "rpar0";
         break;

      case DFAstate::comma0:
         stream << "comma0";
         break;

      case DFAstate::rsqpar0:
         stream << "rsqpar0";
         break;

      case DFAstate::whitespace0:
         stream << "whitespace0";
         break;

      case DFAstate::eof0:
         stream << "eof0";
         break;

      case DFAstate::percent0:
         stream << "percent0";
         break;

      case DFAstate::percent1:
         stream << "percent1";
         break;

      case DFAstate::quoted0:
         stream << "quoted0";
         break;

      case DFAstate::quoted1:
         stream << "quoted1";
         break;

      case DFAstate::quoted2:
         stream << "quoted2";
         break;

      case DFAstate::quoted3:
	 stream << "quoted3";
	 break;

      case DFAstate::quoted4:
         stream << "quoted4";
	 break;

      case DFAstate::quoted5:
	 stream << "quoted5";
	 break;

      case DFAstate::nil0:
         stream << "nil0";
         break;

      case DFAstate::nil1:
         stream << "nil1";
         break;

      case DFAstate::div0:
         stream << "div0";
         break;

      case DFAstate::div1:
         stream << "div1";
         break;

      case DFAstate::div2:
         stream << "div2";
         break;

      case DFAstate::div3:
         stream << "div3"; 
         break;
     
      case DFAstate::name0:
         stream << "name0";
         break;

      case DFAstate::operator0: 
         stream << "operator0";
         break;
 
      case DFAstate::blocked:
         stream << "blocked";
         break;

      case DFAstate::unknown:
         stream << "unknown";
         break;

      default:
         std::cout << "DFAstate::operator << : \n";
	 std::cout << "ill-defined value\n\n"; 
	 ASSERT( false ); 

      }
      return stream;
   }

}



