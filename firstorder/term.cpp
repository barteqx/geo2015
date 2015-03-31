

// Code written by Hans de Nivelle, June 2006.

#include "term.h"


namespace firstorder
{

   term::term( const term& other )
      : repr( other. repr ) 
   {
      ASSERT( repr -> reference_count ); 
      ++ ( repr -> reference_count );
   }


   void term::operator = ( term other )
   {
      -- ( repr -> reference_count );
      if( ( repr -> reference_count ) == 0 )
         delete repr;

      repr = other. repr;
      ASSERT( repr -> reference_count );
      ++ ( repr -> reference_count );
   }


   term::~term( ) 
   {
      -- ( repr -> reference_count );
      if( ( repr -> reference_count ) == 0 )
         delete repr;
   }




   term::term( function f, const std::list< term > & subterms )
      : repr( new term_node( f, subterms ))
   {
      ASSERT( f. getarity( ) == subterms. size( )); 
      ( repr -> reference_count ) = 1;
   }

   term::term( function f, const std::vector< term > & subterms )
      : repr( new term_node( f, subterms ))
   {
      ASSERT( f. getarity( ) == subterms. size( ));
      ( repr -> reference_count ) = 1;
   }


   std::list< term > :: const_iterator term::subterms_begin( ) const
   {
      return ( repr -> subterms. begin( ));
   }

   std::list< term > :: const_iterator term::subterms_end( ) const
   {
      return ( repr -> subterms. end( ));
   }


   function term::getfunction( ) const
   {
      return ( repr -> func );
   }

   unsigned int term::getnrsubterms( ) const
   {
      return ( repr -> subterms. size( ));
   }


   int term::compare( term t1, term t2 )
   {
      function f1 = t1. getfunction( );
      function f2 = t2. getfunction( );

      if( f1 < f2 ) return -1;
      if( f1 > f2 ) return 1;
      
      std::list< term > :: const_iterator p1 = t1. subterms_begin( );
      std::list< term > :: const_iterator p2 = t2. subterms_begin( );
      while( p1 != t1. subterms_end( ))
      {
         int c = compare( *p1, *p2 );
	 if( c != 0 )
            return c;

	 ++ p1;
	 ++ p2; 
      }
      ASSERT( p2 == t2. subterms_end( ));
      return 0;
   }


   bool term::operator < ( term other ) const
   {
      return compare( *this, other ) == -1;
   }

   bool term::operator == ( term other ) const
   {
      return compare( *this, other ) == 0;
   }

   bool term::operator != ( term other ) const
   {
      return compare( *this, other ) != 0;
   }


   std::ostream& operator << ( std::ostream& stream, const term& t )
   {
      stream << t. getfunction( );
      std::list< term > :: const_iterator p1 = t. subterms_begin( );
      std::list< term > :: const_iterator p2 = t. subterms_end( );

      if( p1 != p2 )
      {
         stream << "( "; 
         for( std::list< term > :: const_iterator p = p1; p != p2; ++ p )
         {
            if( p != p1 )
               stream << ", ";
	    stream << *p;
         }
	 stream << " )"; 
      }
      return stream;
   }

} 



