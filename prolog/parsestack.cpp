

// Code written by Ravinder Indarapu and Hans de Nivelle, April 2003.


#include "parsestack.h"


namespace prolog 
{

   parsestack::parsestack( ) 
   {
      contextvec. push_back( context( context::toplevel ));
      tokenvec. push_back( token( token::start ) );
   }


   void parsestack::computecontexts( unsigned int position )
   {
      ASSERT( position >= 1 );

      unsigned int size = contextvec. size( );
	  
      for( unsigned int i = position; i < size; i ++ )
      {
         contextvec[i] = contextvec[ i-1 ]. transition( tokenvec [ i-1 ] );
      }
   }


   void parsestack::shift( const token& t )
   {
      contextvec. push_back( context( context::toplevel ) );
      tokenvec. push_back( t );

      computecontexts( contextvec. size( ) - 1 );
   }


   void parsestack::reduce( unsigned int position, 
                            const token& lhs, unsigned int rhs_length )
   {

      ASSERT( position != 0 );
      ASSERT( position + rhs_length <= contextvec. size( ));

      if( rhs_length >= 1 )
      {
         contextvec. erase( contextvec. begin( ) + position + 1,
                            contextvec. begin( ) + position + rhs_length );
         tokenvec. erase( tokenvec. begin( ) + position + 1,
                          tokenvec. begin( ) + position + rhs_length );
         tokenvec[ position ] = lhs;
      }
      else
      {
         contextvec. insert( contextvec. begin( ) + position, 
                             context( context::toplevel ) ); 
         tokenvec. insert( tokenvec. begin( ) + position, lhs ); 
      }
      computecontexts( position ); 
   }



   bool parsestack::patternfits( const unsigned int* pa ) const
   {

      unsigned int len = 0;
      for( const unsigned int* q = pa; *q != next; )
      {
         while( *q != next )
            ++ q;
	 ++ q;

	 ++ len;
      }

      if( len > tokenvec. size( ))
         return false;
            // If the pattern does not fit due to length, we return false.

      // Next we check the pattern: 
      
      unsigned int pos = tokenvec. size( ) - len; 
      while( *pa != next )
      {
         // Check if we have a dontcare in the list of alternatives,
	 // or tokenvec [ pos ]:

	 while( *pa != next )  
         {
            if( *pa == dontcare ) 
               goto found; 
            if( tokenvec [ pos ]. type == *pa )
               goto found;

	    ++ pa; 
         }
         return false;
      found:
         while( *pa != next )
            ++ pa; 
         ++ pa;

	 ++ pos; 
      }
      return true;
   }



   std::ostream& operator << ( std::ostream& os, const parsestack& ps )
   {
      os << "[ ";
      unsigned int size = ps. contextvec. size( );

      for( unsigned int i = 0; i < size; i ++ )
      {
         os << ps. getcontext(i);
         os << " : ";
         os << ps. gettoken(i);

	 if( i + 1 < size )
            os << ",  "; 
      }
      os << " ]";
      return os;
   }

} 


