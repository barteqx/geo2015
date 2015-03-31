

// Code written by Ravinder Indarapu and Hans de Nivelle, April 2003.


#ifndef PROLOG_PARSESTACK_INCLUDED
#define PROLOG_PARSESTACK_INCLUDED   1


#include <iostream>

#include "../assert.h"

#include "context.h"
#include "token.h"


namespace prolog 
{

   class parsestack
   {

   private: 
      std::vector< context > contextvec;
      std::vector< token > tokenvec;

   public: 
      parsestack( );
         // Constructs parsestack containing one token, namely
         // token::start in context::toplevel.

      void shift( const token& t );
     
      void reduce( unsigned int position, const token& lhs, 
                   unsigned int rhs_length );

	
      const token& gettoken( unsigned int i ) const
      { return tokenvec [i]; }

      const context& getcontext( unsigned int i ) const
      { return contextvec [i]; }


      unsigned int size( ) const
      { return tokenvec. size( ); }
         // Number of symbols on the parsestack. 


      static const unsigned int dontcare = 88888888; 
         // Used by patternfits to match any token.
      static const unsigned int next = 99999999;
         // Used by patternfits to mark the end of a sequence
	 // of alternatives. 

      bool patternfits( const unsigned int* pa ) const;

      friend std::ostream& operator << ( std::ostream&, const parsestack & );

   private:
      void computecontexts( unsigned int position );
         // Computes the contexts of the tokens from position on 
         // upwards. position should be >= 0. 

   };

} 


#endif


