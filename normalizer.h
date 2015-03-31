

// Code written by Hans de Nivelle, November 2005. 


#ifndef NORMALIZER_INCLUDED
#define NORMALIZER_INCLUDED    1

#include <iostream>
#include <vector>

#include "variable.h"
#include "label.h"

class normalizer 
{

private: 
   struct assignment
   {
      label from_lab;
      variable from_var;
      variable var;


      assignment( label from_lab, variable from_var,
		  variable var ) 
         : from_lab( from_lab ), from_var( from_var ),
           var( var ) 
      {
      }

   };


private:
   std::vector< assignment > repr; 
   variable nextvar;

public:

   void clear( );

   void reserve( unsigned int x );
      // Reserve space for x assignments. Note that existing 
      // assignments are counted. If you have 5 assignemts, need 
      // another 10, then you should reserve 15. 
      // It is not required to do any reservation at all.

   unsigned int size( ) const;
      // Returns number of assignments in the substitution, 
      // the result can be used for backtracking. 

   variable lookup( label l, variable v ); 
      // Normalizes the pair ( l, v ).

   void backtrack( unsigned int ); 
    
   friend std::ostream& operator << ( std::ostream&, const normalizer& );

};


inline void normalizer::clear( )
{
   repr. clear( ); 
   nextvar = variable( ); 
}
   

inline void normalizer::reserve( unsigned int x )
{
   repr. reserve( x ); 
}   
    

inline unsigned int normalizer::size( ) const
{
   return repr. size( ); 
}


inline variable normalizer::lookup( label l, variable v ) 
{

   for( std::vector< assignment > :: const_iterator 
	   p = repr. begin( ); 
	   p < repr. end( ); 
	   ++ p ) 
   {
      if( ( p -> from_var ) == v && ( p -> from_lab ) == l ) 
         return ( p -> var );  
   }
   repr. push_back( assignment( l, v, nextvar ));
   return nextvar ++ ;
}


inline void normalizer::backtrack( unsigned int x ) 
{
   ASSERT( x <= repr. size( )); 

   if( x < repr. size( ))
   {
      nextvar = repr [x]. var;
      repr. erase( repr. begin( ) + x, repr. end( )); 
   }
}


#endif


