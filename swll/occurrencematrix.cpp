

// Code written by Hans de Nivelle, April 2006.


#include "occurrencematrix.h"


namespace swll
{

   const std::vector< occurrencematrix::within_clause > & 
   occurrencematrix::row::operator [] ( unsigned int x ) const
   {
      ASSERT( x < repr. size( ));
      return repr [x];
   }


   std::vector< occurrencematrix::within_clause > & 
   occurrencematrix::row::operator [] ( unsigned int x ) 
   {
      if( x >= repr. size( ))
      {
         repr. insert( repr. end( ), 1 + x - repr. size( ),
                       std::vector< within_clause > ( )); 
      }
      return repr [x];
   }




   const occurrencematrix::row& 
   occurrencematrix::operator [] ( variable v ) const
   {
      unsigned int k = v. getindex( );
      ASSERT( k < repr. size( ));
      return repr [k];
   }


   occurrencematrix::row& 
   occurrencematrix::operator [] ( variable v ) 
   {
      unsigned int k = v. getindex( );
      if( k >= repr. size( ))
      {
         repr. insert( repr. end( ), 1 + k - repr. size( ), row( ) ); 
      }
      return repr [k];
   }

 
   void occurrencematrix::clear( )
   {
      repr. clear( );
   }


   void occurrencematrix::insertoccurrence(
                std::vector< within_clause > & w,
		unsigned int clausenr, unsigned int offset )
   {
      for( std::vector< within_clause > :: iterator
              p = w. begin( );
	      p != w. end( );
	      ++ p )
      {
         if( p -> clausenr == clausenr )
         {
            p -> offsets. push_back( offset );
	    return;
         }
      }
      w. push_back( within_clause( clausenr ));
      w. back( ). offsets. push_back( offset );
   }


   std::ostream& operator << ( std::ostream& stream,
		               const occurrencematrix& m ) 
   {
      stream << "occurrence matrix:\n";

      for( unsigned int v = 0; v < m. repr. size( ); ++ v )
      {
         for( unsigned int x = 0; x < m. repr [v]. repr. size( ); ++ x )
         {
            const std::vector< occurrencematrix::within_clause > & 
	       occ = m. repr [v]. repr [x];

	    if( occ. size( ))
            {
	       stream << "assignment( ";
	       stream << "( " << variable::constructfromunsigned(v);
	       stream << " := ";
	       stream << x << " ): ";

	       stream << "[ "; 
	       for( std::vector< occurrencematrix::within_clause > :: 
	                     const_iterator
                       p = occ. begin( );
		       p != occ. end( );
		       ++ p )
               {
                  if( p != occ. begin( ))
                     stream << ", "; 

		  stream << p -> clausenr << ":[";
		  for( std::list< unsigned int > :: const_iterator
                          q = p -> offsets. begin( );
			  q != p -> offsets. end( );
			  ++ q )
                  {
                     if( q != p -> offsets. begin( ))
                        stream << ",";
		     stream << *q;
                  }
		  stream << "]"; 
               }
	       stream << " ]\n"; 
            }
         }
      }
   
      return stream; 
   }
} 



