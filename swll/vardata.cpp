

// Written by Hans de Nivelle,
// April 2006.


#include "vardata.h"


namespace swll
{

   void vardata::clear( )
   {
      repr. clear( );
   }


   const vardata::data & vardata::operator [] ( variable v ) const
   {
      unsigned int k = v. getindex( );
      ASSERT( k < repr. size( ));
      return repr [k];
   }


   vardata::data & vardata::operator [] ( variable v ) 
   {
      unsigned int k = v. getindex( );
      if( k >= repr. size( ))
      {
         repr. insert( repr. end( ), 1 + k - repr. size( ),
		       vardata::data( ) ); 
	    // Expand the vector. 
      }
      return repr [k];
   }


   std::ostream& operator << ( std::ostream& stream,
		               const vardata& vd )
   {
      stream << "vardata:\n"; 
      for( unsigned int v = 0; v < vd. repr. size( ); ++ v )
      {
         if( vd. repr [v]. occurrences. size( ))
         {
            std::cout << variable::constructfromunsigned(v) << "   ";
            stream << "[" << vd. repr [v]. lastupdate << "] : ";
            stream << vd. repr [v]. possiblevalues;
	    stream << " occurrences[ "; 
	    for( std::vector< unsigned int > :: const_iterator
                    p = vd. repr [v]. occurrences. begin( );
		    p != vd. repr [v]. occurrences. end( );
		    ++ p )
            {
               if( p != vd. repr [v]. occurrences. begin( )) 
                  stream << ", "; 
               stream << *p;
            }
	    stream << " ]"; 
	    stream << "\n";
         }
      }
      return stream; 
   }

} 



