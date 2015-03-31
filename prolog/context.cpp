

// Code written by Ravinder Indarapu and Hans de Nivelle, April 2003. 


#include "context.h"
#include <cstdlib>


namespace prolog
{



   context context::transition( const token& t ) const
   {
      if( t. type == token::lpar )
      {
         if( value == afterglued )
	    return context( funcarg );
	 else
	    return context( parentheses );
      }

      if( t. type == token::gluedidentifier ||
          t. type == token::gluedquotedidentifier ) 
      {
         return context( afterglued );
      }

      if( t. type == token::lsqpar )
      {
         return context ( listarg );
      }

      if( t. type == token::bar )
      {
         if( value == listarg )
         {
	    return context( listafterbar );
	 }
      }

      return *this;
   }



   bool context::allows( const char* s ) const
   {
      if( value == toplevel && s[0] == '.' && s[1] == 0 )
         return true;

      if( ( value == funcarg || value == listarg ) && 
            s[0] == ',' && s[1] == 0 )
         return true;

      if( value == listarg && s[0] == '|' && s[1] == 0 )
         return true;

      return false;

   }


   std::ostream& operator << ( std::ostream& os, context c )
   {
      switch( c. value )
      {
      case context::toplevel:
         os << "toplevel"; break;

      case context::parentheses:
         os << "parentheses"; break;

      case context::afterglued:
         os << "afterglued"; break;

      case context::funcarg:
         os << "funcarg"; break;

      case context::listarg:
         os << "listarg"; break;

      case context::listafterbar:
         os << "listafterbar"; break;

      default:
         ASSERT( false );
	 exit(0);

      }
      return os; 
   }


   bool operator == ( context c1, context c2 )
   {
      return c1. value == c2. value;
   }

} 

