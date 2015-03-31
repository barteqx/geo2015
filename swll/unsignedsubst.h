

// Written by Hans de Nivelle, April 2006.


#ifndef SWLL_UNSIGNEDSUBST_DEFINED 
#define SWLL_UNSIGNEDSUBST_DEFINED   1


#include <iostream>
#include <vector>


#include "../variable.h"


namespace swll
{
 
   // An unsignedsubst assigns unsigned integers to variables.

   class unsignedsubst 
   {

      struct assignment 
      {
         unsigned int val;
	 bool isdefined; 

	 assignment( ) 
            : val(0), isdefined(false)
         {
         }

      };

      std::vector< assignment > repr;

      std::vector< variable > assigned;
         // These are the assigned variables in the order
	 // in which they were assigned. 

      
   public:
      unsignedsubst( ) { } 

      explicit unsignedsubst( variable v );
         // Produce an unsignedsubst and reserve space up to v.



      void clear( );

      unsigned int size( ) const; 
         // Returns nr of assignments in the substitution. 

      variable nthvar( unsigned int x ) const;
         // Returns the variable to which the x-th assignment
	 // was made.


      void assign( variable v, unsigned int x );
         // Assign v := x. Overwriting is not allowed.

      const unsigned int* lookup( variable v ) const; 
         // Returns 0 if there is no assignment. 
         // The value remains valid until the next non-const
	 // operation.

      void backtrack( unsigned int x );
         // One can take the size( ) and later backtrack to it. 

      variable sup_variables( ) const;
         // Supremum of variables occurring in the domain of the
	 // substitution.

      unsigned int sup_unsigneds( ) const;
         // Supremum of variables occurring in the range of the
	 // substitution.



   };


   std::ostream& operator << ( std::ostream&, const unsignedsubst& );


   inline unsigned int unsignedsubst::size( ) const
   {
      return assigned. size( );
   }


   inline variable unsignedsubst::nthvar( unsigned int x ) const 
   {
      ASSERT( x < assigned. size( ));
      return assigned [x];
   }

   
   inline void unsignedsubst::assign( variable v, unsigned int x )
   {
      unsigned int k = v. getindex( );
      if( k >= repr. size( ))
         repr. insert( repr. end( ), 1 + k - repr. size( ), assignment( ));

      ASSERT( ! repr [k]. isdefined );
         // Overwriting is not allowed.

      repr [k]. val = x;
      repr [k]. isdefined = true;
      assigned. push_back(v);
   }


   inline const unsigned int* unsignedsubst::lookup( variable v ) const 
   {
      unsigned int k = v. getindex( );
      if( k >= repr. size( ) || ! repr [k]. isdefined )
         return 0;
      return & ( repr [k]. val );
   }

          
   inline void unsignedsubst::backtrack( unsigned int x )
   {
      ASSERT( x <= assigned. size( ));
      while( x < assigned. size( ))
      {
         unsigned int k = assigned. back( ). getindex( );
	 assigned. pop_back( );
	 repr [k]. isdefined = false;
      }
   }


} 


#endif



