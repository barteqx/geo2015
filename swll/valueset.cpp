

// Written by Hans de Nivelle, April 2006.


#include "valueset.h"


namespace swll
{

   valueset::valueset( )
   {
   }


   valueset::valueset( unsigned int x )
      : repr( std::vector< member > ( 1 + x ))
   {
   }


   void valueset::clear( )
   {
      repr. clear( );
   }


   void valueset::unmarkall( )
   {
      for( std::vector< member > :: iterator 
              p = repr. begin( );
	      p != repr. end( );
	      ++ p )
      {
         ( p -> marked ) = false;
      }
   }


   bool valueset::restricttomarked( ) 
   {
      bool deleted = false;

      for( std::vector< member > :: iterator
              p = repr. begin( );
	      p != repr. end( );
	      ++ p )
      {
         if( p -> contained )
         {
            if( ! p -> marked )
            {
               deleted = true;
	       p -> contained = false;
	    }
	    else
            {
	       p -> marked = false;
            }
         }
      }
      return deleted;
   }

   
   std::ostream& operator << ( std::ostream& stream, 
		               const valueset& s )
   {
      unsigned int cnt = 0;

      stream << "valueset{ ";
      unsigned int i = 0; 
      for( std::vector< valueset::member > :: const_iterator 
              p = s. repr. begin( );
	      p != s. repr. end( );
	      ++ p, ++ i )
      {
         if( p -> contained )
         {
            if( cnt != 0 )
               stream << ", ";
	    stream << i;
	    if( p -> marked )
               stream << "*";

	    ++ cnt;
         }
      }
      stream << " }";
      return stream;
   }

} 


