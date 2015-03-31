

// Code written by Hans de Nivelle, July 2006.


// A binding stack keeps track of bound variables.


#ifndef FIRSTORDER_BINDINGSTACK_INCLUDED
#define FIRSTORDER_BINDINGSTACK_INCLUDED   1


#include <map>


#include "function.h"


namespace firstorder
{

   class bindingstack 
   {
      
      std::map< function, unsigned int > repr; 
         // functions, and how often they are bound. 

   public: 
      void bind( function v );
      void release( function v );
         // Add or release a binding for v. The number of bindings
	 // is counted. 

      bool isbound( function v ) const; 
      bool isempty( ) const;

      friend std::ostream& operator << ( std::ostream& ,
		                         const bindingstack& ); 
   };

   std::ostream& operator << ( std::ostream& stream,
		               const bindingstack& bs );



   inline void bindingstack::bind( function v )
   {
      ASSERT( v. getarity( ) == 0 );
      repr [ v ] ++ ;
   }


   inline void bindingstack::release( function v )
   {
      std::map< function, unsigned int > :: iterator p = repr. find(v);
      
      ASSERT( p != repr. end( ));
      ASSERT( p -> second );
      ( p -> second ) -- ;
      if(( p -> second ) == 0 )
         repr. erase(p);
   }


   inline bool bindingstack::isbound( function v ) const
   {
      std::map< function, unsigned int > :: const_iterator p = repr. find(v);
      return ( p != repr. end( ) && ( p -> second ));
   }


   inline bool bindingstack::isempty( ) const
   {
      return repr. size( ) == 0; 
   }


   inline std::ostream& operator << ( std::ostream& stream,
		                      const bindingstack& bs )
   {
      stream << "{";
      for( std::map< function, unsigned int > :: const_iterator
              p = bs. repr. begin( );
	      p != bs. repr. end( );
	      ++ p )
      {
         if( p != bs. repr. begin( ))
            stream << ",";

         stream << ( p -> first ) << ":" << ( p -> second );
      }
      stream << "}";
      return stream;
   }

} 


#endif


