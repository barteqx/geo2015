

// Code written by Hans de Nivelle, Jan 2006.
// Added const_iterator in February 2007. 


#ifndef VARSET_INCLUDED
#define VARSET_INCLUDED  1


#include <vector>
#include <iostream>


#include "variable.h"
#include "varatom.h"



class varset 
{

   std::vector< variable > repr;

public: 
   bool contains( variable v ) const;
   unsigned int size( ) const;
   void clear( ); 

   void insert( variable v );
   void remove( variable v );

   variable supremum( ) const; 


   typedef std::vector< variable > :: const_iterator const_iterator;

   const_iterator vars_cbegin( ) const { return repr. begin( ); } 
   const_iterator vars_cend( ) const { return repr. end( ); } 


   friend varset operator | ( const varset& , const varset& );
   friend varset operator & ( const varset& , const varset& );
   friend varset operator - ( const varset& , const varset& );

};


varset operator | ( const varset& v1, const varset& v2 );
varset operator & ( const varset& v1, const varset& v2 );
varset operator - ( const varset& v1, const varset& v2 );

std::ostream& operator << ( std::ostream& stream, const varset& vs );

#endif


