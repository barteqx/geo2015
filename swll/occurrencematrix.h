

// Written by Hans de Nivelle,
// April/May 2006. 


#ifndef SWLL_OCCURRENCEMATRIX_DEFINED
#define SWLL_OCCURRENCEMATRIX_DEFINED    1


#include <iostream>
#include <list>
#include <vector>

#include "../variable.h"


namespace swll
{ 

   // An occurrencematrix is a matrix of which
   // the columns are indexed by integers and the
   // rows are indexed by values. 
   // M(v,x) contains the vector of lists of occurrences. 
   // The occurrences that are in one clause are grouped into
   // one list. 

   class occurrencematrix
   {
   public:

      // Contains for a single clause all the occurrences
      // of some assignment:

      struct within_clause
      {
         unsigned int clausenr; 
            // The number of the clause.

	 std::list< unsigned int > offsets;
            // The offsets of the alternatives in the clause 
	    // where the assignment occurs. 

         within_clause( unsigned int clausenr ) 
            : clausenr( clausenr )
         {
         }

      };

   private: 
      struct row
      {
         std::vector< std::vector< within_clause > > repr;

	 const std::vector<within_clause> & operator[] ( unsigned int ) const;
         std::vector< within_clause > & operator[] ( unsigned int );
      };
        // A row defines for a given variable a vector which is 
	// indexed by the possible values, and which contains
	// for each possible value a vector of occurrences of the
	// corresponding assignment.

   private: 
      std::vector< row > repr;

   public: 
      void clear( );
      const row& operator [ ] ( variable v ) const; 
      row& operator [ ] ( variable v ); 
         // This one is guaranteed to exist, because
	 // it will increase the datastructure when required. 

      
      static void insertoccurrence( 
		             std::vector< within_clause > & w,
		             unsigned int clausenr, unsigned int offset );
         // The occurrences are grouped according to their clauses.

      friend std::ostream& operator << ( std::ostream& , 
		                         const occurrencematrix& ); 

   };

} 


#endif

