

// Written by Hans de Nivelle,
// April 2006. 


#ifndef SWLL_VARDATA_DEFINED
#define SWLL_VARDATA_DEFINED    1


#include <iostream>
#include <vector>

#include "../variable.h"
#include "valueset.h"


namespace swll
{ 

   // Associates to each variable the nr of its last update, 
   // the set of clauses (as indices) in which it occurs, 
   // and the set of its possible values. 


   class vardata 
   {
   public: 
      struct data 
      {
         std::vector< unsigned int > occurrences;
	    // The clauses in which the variable occurs, represented
	    // as indices.

         unsigned int lastupdate; 
	    // The moment when the set of possible values was updated
	    // for the last time. 

         valueset possiblevalues;
            // Set of possible values of the variable. 

	 data( )
            : occurrences( std::vector< unsigned int > ( ) ),
	      lastupdate(0), 
	      possiblevalues( valueset( ))
         {
         }

      }; 

   private: 
      std::vector< data > repr;

   public: 
      void clear( );
      const data& operator [] ( variable v ) const; 
      data& operator [] ( variable v ); 
         // This one is guaranteed to exist, because
	 // it will increase the datastructure if required. 


      friend std::ostream& operator << ( std::ostream & , const vardata & );

   };


} 


#endif

