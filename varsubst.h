

// Code written by Hans de Nivelle, November 2005. 


#ifndef VARSUBST_INCLUDED
#define VARSUBST_INCLUDED    1


#include <iostream>
#include <vector>


#include "variable.h"
#include "varatom.h"
#include "label.h"
#include "normalizer.h"
#include "groundsubst.h"


class varsubst
{


private: 
   struct assignment
   {
      label from_lab;
      variable from_var;
      label into_lab;
      variable into_var;


      assignment( label from_lab, variable from_var,
		  label into_lab, variable into_var ) 
         : from_lab( from_lab ), from_var( from_var ),
           into_lab( into_lab ), into_var( into_var ) 
      {
      }

   };


private:
   std::vector< assignment > repr; 

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

   void normalize( label& l, variable& v ) const; 
      // Normalizes the pair ( l, v ) under the substitution.

   void backtrack( unsigned int ); 
    
   void assign( label, variable, label, variable );
      // Assigns variable := element.
      // One should attempt to assign multiple label-variable pairs 
      // to the same variable. 


   bool unify( label, varatom, label, varatom );
      // Attempt unify the two varatoms and extend the substitution with
      // the necessary assignments in case of success. 
      // In case unification fails,
      // the substitution may still be extended. 

   varatom apply_on( normalizer& n, label lab, varatom va ) const;
      // Apply the substitution on varatom. The normalizer is
      // needed for normalizing the variables in the result. 
      // It should be reset before a rule or clause
      // is constructed. The result will remain valid only until
      // the next call of apply_on( ). 

   bool results_equal( label lab1, variable v1,
		       label lab2, variable v2 ) const; 

      // True if (lab1,v1) == (lab2,v2) after the substitution.

   varatom apply_and_replace( normalizer& n, label lab, varatom va,
		              label lab1, variable var1,
			      label lab2, variable var2 ) const;
      // Apply the substitution on varatom. The normalizer is
      // necessary for normalizing the variables in the result.
      // In the resulting varatom, the result of (lab1,var1) is 
      // replaced by (lab2,var2). 
      // (This method is needed for paramodulation) 

   friend std::ostream& operator << ( std::ostream&, const varsubst& );

};


inline void varsubst::clear( )
{
   repr. clear( ); 
}
   

inline void varsubst::reserve( unsigned int x )
{
   repr. reserve( x ); 
}   
    

inline unsigned int varsubst::size( ) const
{
   return repr. size( ); 
}


inline void varsubst::normalize( label& l, variable& v ) const
{

   for( std::vector< assignment > :: const_iterator 
	   p = repr. begin( ); 
	   p < repr. end( ); 
	   ++ p ) 
   {
      if( ( p -> from_var ) == v && ( p -> from_lab ) == l ) 
      {
          l = ( p -> into_lab );
	  v = ( p -> into_var );
      }
   }
}


inline void varsubst::backtrack( unsigned int x ) 
{
   ASSERT( x <= repr. size( )); 

   repr. erase( repr. begin( ) + x, repr. end( )); 
}


inline void varsubst::assign( label from_lab,
                              variable from_var,
	                      label into_lab,
		              variable into_var ) 
{
   repr. push_back( assignment( from_lab, from_var, into_lab, into_var ));
}


#endif


