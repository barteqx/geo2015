

// Code written by Hans de Nivelle, April 2006.
// Revised February 2007. Removed functionality, because
// it will become explicit in the rule system.


#ifndef PREDICATE_INCLUDED
#define PREDICATE_INCLUDED   1


#include <vector> 
#include <map>
#include <string>
#include <list>
#include <iostream>


#include "assert.h"


class predicate
{

   static std::map < std::string, std::list< unsigned int > > lookuptable;
      // Assigns to each string the list of its representations. 
      // The distinct representations correspond to distinct arities.

   struct prop 
   { 
      std::string stringrepr;
      unsigned int arity;

      prop( const std::string& stringrepr,
            unsigned int arity ) 
         : stringrepr( stringrepr ),
	   arity( arity )
      {
      }

   };  

   static std::vector< prop > properties;

   unsigned int repr; 

   predicate( );
      // Has no definition. There is no default predicate. 

public: 
   predicate( const std::string& stringrepr, unsigned int arity );
       
   static bool isdefined( const std::string& stringrepr, unsigned int arity );
      // True if the string is known as a predicate with the given arity.

   const std::string& getstring( ) const;
   unsigned int getarity( ) const; 

   friend std::ostream& operator << ( std::ostream& ,
		                      const predicate p ); 

   static void printtable( std::ostream& stream );
      // Prints the table of all predicates. 

   friend bool operator == ( predicate, predicate );
   friend bool operator != ( predicate, predicate );
   friend bool operator < ( predicate, predicate ); 
   friend bool operator > ( predicate, predicate );
   friend bool operator <= ( predicate, predicate );
   friend bool operator >= ( predicate, predicate );


   static predicate diseq; 
      // Predicate for disequality. 
   static predicate create;
      // Predicate for creating a new object in model. 


   friend std::ostream& operator << ( std::ostream& , const prop& ); 

};


inline const std::string& predicate::getstring( ) const
{
   return properties [ repr ]. stringrepr; 
}


inline unsigned int predicate::getarity( ) const
{
   return properties [ repr ]. arity; 
}


inline std::ostream& operator << ( std::ostream& stream,
		                   const predicate p )
{
   stream << p. getstring( );
   return stream;
}


inline bool operator == ( predicate p1, predicate p2 )
{
   return p1. repr == p2. repr;
}

inline bool operator != ( predicate p1, predicate p2 )
{
   return p1. repr != p2. repr;
}

inline bool operator < ( predicate p1, predicate p2 )
{
   return p1. repr < p2. repr;
}

inline bool operator > ( predicate p1, predicate p2 )
{
   return p1. repr > p2. repr;
}

inline bool operator <= ( predicate p1, predicate p2 )
{
   return p1. repr <= p2. repr;
}

inline bool operator >= ( predicate p1, predicate p2 )
{
   return p1. repr >= p2. repr;
}


#endif



