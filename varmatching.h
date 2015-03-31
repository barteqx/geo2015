

// Code written by Hans de Nivelle, February 2006.


// A variable matching assigns variables to variables. 


#ifndef VARMATCHING_INCLUDED
#define VARMATCHING_INCLUDED    1


#include <iostream>
#include <vector>


#include "variable.h"


#include "varatom.h"
#include "varset.h"


class varmatching
{

   struct assignment
   {
      variable v1; 
      variable v2; 

      assignment( variable v1, variable v2 ) 
         : v1( v1 ), v2( v2 ) 
      {
      }

   };


private:
   std::vector< assignment > repr; 

public:

   void clear( );

   unsigned int size( ) const;
      // Returns number of assignments in the substitution, 
      // the result can be used for backtracking. 

   const variable* lookup( variable ) const; 
      // Lookup returns 0 if there is no value. The 
      // pointer remains valid until the next non-const 
      // operation. 

   void backtrack( unsigned int );
      // One can take the size( ), and later backtrack 
      // to size( ). 

   varatom apply_on( varatom va ) const;
      // The result remains valid only until the next call of
      // apply. (because varatom is an incomplete datatype) 
      // apply_on crashes when there are variables in va that
      // are not in the domain of the substitution.

   void assign( variable v1, variable v2 );
      // Assigns v1 := v2. 
      // One should not reassign to a variable that already 
      // has a value. 

   varset getdomain( ) const;
      // Return the domain of the matching as varset.

   varset getrange( ) const;
      // Returns the range of the matching as varset.

   varset getaddedvars( unsigned int x ) const;
      // Get the variables that have been added since the 
      // size( ) was x. 

   bool match( varatom va1, varatom va2 );
      // Try to match va into at. If succesful, then the 
      // variable matching is extended with the necessary
      // assignments and true is returned.
      // When matching fails, false is returned but it will be necessary
      // to restore the variable substitution.
      // match gives no special treatment do disequalities
      // or create atoms. 

   bool consistentwith( const varmatching& other ) const; 
      // True if this varmatching is consistent with the other
      // varmatching.

   friend varmatching operator + ( const varmatching& , 
		                   const varmatching& );

   friend std::ostream& operator << ( std::ostream&, const varmatching& );

};


inline void varmatching::clear( )
{
   repr. clear( ); 
}
   

inline unsigned int varmatching::size( ) const
{
   return repr. size( ); 
}


inline void varmatching::backtrack( unsigned int x ) 
{
   ASSERT( x <= repr. size( ));
   while( x < repr. size( ))
      repr. pop_back( );
}


varmatching operator + ( const varmatching& ,
		         const varmatching& );


#endif


