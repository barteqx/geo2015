
#include "syntacticdef.h"
#include "../assert.h"


// Code written by Ravinder Indarapu and Hans de Nivelle, March 2003.


namespace prolog
{


   syntacticdef::syntacticdef( const std::string& ident, 
		               unsigned int type, unsigned int attraction )
      : ident( ident ), type( type ), attraction( attraction ) 
   {
      ASSERT( 
         type == FX || type == FY ||
         type == XFX || type == XFY || type == YFX ||
         type == XF || type == YF ); 
	
      ASSERT( attraction >= 0 && attraction < supattraction ); 
   }


   const std::string& syntacticdef::getstring( ) const
   {
      return ident;
   }


   bool syntacticdef::isprefix() const
   {
     return type == FX || type == FY; 
   }


   bool syntacticdef::isinfix() const
   {
      return type == XFX || type == XFY || type == YFX; 
   }


   bool syntacticdef::ispostfix() const
   {
      return type == XF || type == YF;
   }


   int syntacticdef::decideconflict( const syntacticdef& other ) const
   {
      if( attraction < other.attraction )
         return -1;

      if( attraction > other.attraction )
         return 1;

      unsigned int t1 = type;
      unsigned int t2 = other.type;

      if( t1 == FX && t2 == YF )
         return -1;
      if( t1 == FX && t2 == YFX )
         return -1;
      if( t1 == YFX && t2 == YFX )
         return -1;
      if( t1 == YFX && t2 == YF )
         return -1;

      if( t1 == FY && t2 == XF )
         return 1;
      if( t1 == FY && t2 == XFY )
         return 1;
      if( t1 == XFY && t2 == XFY )
         return 1;
      if( t1 == XFY && t2 == XF )
         return 1;

      return 0;
   }


   bool syntacticdef::operator == ( const syntacticdef& other ) const
   {
      if( ident == other. ident &&
          type == other. type &&
          attraction == other. attraction )
      {
         return true;
      }
      else
      {
         return false;
      }
   }


   bool syntacticdef::operator != ( const syntacticdef& other ) const
   {
      if( ident != other. ident ||
          type != other. type ||
          attraction != other. attraction ) 
      {
         return true;
      }
      else
      {
         return false;
      }
   }


   std::ostream& operator << ( std::ostream& os, const syntacticdef& syntdef )
   {
      std::string tpstring;

      switch( syntdef. type )
      {
	 case syntacticdef::FX:
            tpstring = "FX"; 
	    break;
	 case syntacticdef::FY:
            tpstring = "FY"; 
	    break;

	 case syntacticdef::XFX:
            tpstring = "XFX"; 
	    break;
	 case syntacticdef::XFY:
            tpstring = "XFY"; 
	    break;
	 case syntacticdef::YFX:
            tpstring = "YFX"; 
	    break;

	 case syntacticdef::XF:
            tpstring = "XF"; 
	    break;
	 case syntacticdef::YF:
            tpstring = "YF"; 
	    break;

         default:
            tpstring = "ERROR"; 
	    break;
      }

      os << "syntacticdef( ";
      os << syntdef. ident << ", ";
      os << tpstring << ", ";
      os << syntdef. attraction << " )";
      return os;
   }

}




