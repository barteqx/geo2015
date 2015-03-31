

// Code written by Hans de Nivelle, April 2006.
// Modified for Bliksem in August 2006. 


#ifndef PROLOG_FUNCPRED_INCLUDED
#define PROLOG_FUNCPRED_INCLUDED   1


#include <vector> 
#include <map>
#include <string>
#include <list>
#include <iostream>


#include "../assert.h"


namespace prolog
{

   class funcpred 
   {

      static std::map < std::string, std::list< unsigned int > > lookuptable;
         // Associates to each string a list of unsigned ints. 
         // The unsigned ints are indices into properties. 
         // There it can be seen with which arities the string can occur,
         // and what the representations are. 

      struct prop 
      { 
         std::string stringrepr;
         unsigned int arity;

         prop( const std::string& stringrepr, unsigned int arity )
            : stringrepr( stringrepr ),
	      arity( arity )
         {
         }

      };  

      static std::vector< prop > properties;

   unsigned int repr; 

      funcpred( );
         // Has no definition. There is no default predicate or function.

      explicit funcpred( unsigned x ) :
         repr(x)
      { }


   public: 
      funcpred( const std::string& stringrepr, unsigned int arity );
       
      static bool isdefined( const std::string& stringrepr, 
		             unsigned int arity );
         // True if the string is known with the given arity.

      const std::string& getstring( ) const;
      unsigned int getarity( ) const; 

      unsigned int getindex( ) const;
         // Get the index (which is an unsigned int).
      static funcpred fromindex( unsigned int x );
         // Make a function or predicate from an index.

      friend std::ostream& operator << ( std::ostream& , const funcpred fp ); 

      static void printtable( std::ostream& stream );
         // Prints the table of all functions/predicates. 

      friend bool operator == ( funcpred, funcpred ); 
      friend bool operator != ( funcpred, funcpred ); 
      friend bool operator < ( funcpred, funcpred ); 
      friend bool operator > ( funcpred, funcpred );
      friend bool operator <= ( funcpred, funcpred );
      friend bool operator >= ( funcpred, funcpred );

      friend std::ostream& operator << ( std::ostream& , const prop& ); 

   };


   inline const std::string& funcpred::getstring( ) const
   {
      return properties [ repr ]. stringrepr; 
   }

   inline unsigned int funcpred::getarity( ) const
   {
      return properties [ repr ]. arity; 
   }

   inline unsigned int funcpred::getindex( ) const 
   {
      return repr;
   }

   inline funcpred funcpred::fromindex( unsigned int x )
   {
      ASSERT( x < properties. size( ));
      return funcpred(x);
   }


   inline std::ostream& operator << ( std::ostream& stream,
		                      const funcpred fp )
   {
      stream << fp. getstring( );
      return stream;
   }


   inline bool operator == ( funcpred fp1, funcpred fp2 )
   {
      return fp1. repr == fp2. repr;
   }

   inline bool operator != ( funcpred fp1, funcpred fp2 )
   {
      return fp1. repr != fp2. repr;
   }

   inline bool operator < ( funcpred fp1, funcpred fp2 )
   {
      return fp1. repr < fp2. repr;
   }

   inline bool operator > ( funcpred fp1, funcpred fp2 )
   {
      return fp1. repr > fp2. repr;
   }

   inline bool operator <= ( funcpred fp1, funcpred fp2 )
   {
      return fp1. repr <= fp2. repr;
   }

   inline bool operator >= ( funcpred fp1, funcpred fp2 )
   {
      return fp1. repr >= fp2. repr;
   }

}

#endif



