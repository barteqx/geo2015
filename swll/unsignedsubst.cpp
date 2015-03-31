

#include "unsignedsubst.h"

namespace swll
{

   unsignedsubst::unsignedsubst( variable v )
      : repr( std::vector< assignment > ( v. getindex( ) + 1 ))  
   {
   } 


   void unsignedsubst::clear( ) 
   {
      repr. clear( );
      assigned. clear( );
   }


   variable unsignedsubst::sup_variables( ) const
   {
      variable res;

      for( std::vector< variable > :: const_iterator 
              p = assigned. begin( );
	      p != assigned. end( );
	      ++ p )
      {
         variable v = *p;
	 ++ v;
	 if( v > res )
            res = v;
      }
      return res;
   }


   unsigned int unsignedsubst::sup_unsigneds( ) const
   {
      unsigned int res = 0;
      for( std::vector< variable > :: const_iterator
              p = assigned. begin( );
	      p != assigned. end( ); 
	      ++ p )
      {
         unsigned int k = repr [ p -> getindex( ) ]. val + 1;
	 if( k > res )
            res = k;
      }
      return res;
   }


   std::ostream& operator << ( std::ostream& stream,
		               const unsignedsubst& s )
   {
      stream << "unsignedsubst( ";
      for( unsigned int i = 0; i < s. size( ); ++ i )
      {
         if( i != 0 )
            stream << ", ";
	 stream << s. nthvar(i) << " := " << * s. lookup( s. nthvar(i) );
      }
      stream << " )";
      return stream;
   }

   
} 


