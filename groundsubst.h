

// Code written by Hans de Nivelle, May 2005. 
// Modified February 2005.
// - Removed relevance marking. (because it does not help) 

// A ground substitution assigns elements to variables. 

#ifndef GROUNDSUBST_INCLUDED
#define GROUNDSUBST_INCLUDED    1


#include <iostream>
#include <vector>


#include "variable.h"
#include "element.h"

#include "varatom.h"
#include "groundatom.h"

#include "varset.h"


class groundsubst
{

   struct assignment
   {
      variable v;
      element e;

      assignment( variable v, element e ) 
         : v(v), e(e)  
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

   const element* lookup( variable ) const; 
      // Lookup returns 0 if there is no value. The 
      // pointer remains valid until the next non-const 
      // operation. 

   void backtrack( unsigned int );
      // One can take the size( ), and later backtrack 
      // to size( ). 

   groundatom apply_on( varatom va ) const;
      // The result remains valid only until the next call of
      // apply. (because groundatom is an incomplete datatype) 
      // apply_on crashes when there are variables in va that
      // are not in the domain of the substitution.

   void assign( variable, element );
      // Assigns variable := element. 
      // One should not assign multiple values to the same
      // variable. 

   variable sup_variables( ) const;
      // Supremum of variables occurring in domain of substitution.

   element sup_elements( ) const;
      // Supremum of elements occurring in range of substitution.

   varset getdomain( ) const;
      // Returns the domain of the groundsubst. 

   varset getaddedvars( unsigned int x ) const;
      // Returns the variables that have been added after 
      // the moment on which the size( ) was x. 

   bool match( varatom va, groundatom at );
      // Try to match va into at. If succesful, then the 
      // groundsubstitution is extended with the necessary
      // assignments and true is returned.
      // When matching fails, false is returned but it will be necessary
      // to restore the ground substitution.
      // In case, the failure is caused by a conflict with an existing
      // assignment, its used flag will be set to true. 

   bool consistentwith( const groundsubst& other ) const; 
      // True if this ground substitution is consistent with 
      // the other groundsubst. This is the case when this
      // and other contain no conflict assignments. 

   friend groundsubst operator + ( const groundsubst& ,
		                   const groundsubst& ); 


   groundsubst restrict_to( const varset& s ) const;
      // Returns the substitution that one obtains when 
      // the domain is restricted to s. 


   friend std::ostream& operator << ( std::ostream&, const groundsubst& );


private:
   const assignment* nextassignment( variable v ) const;
      // Returns pointer to the assignment ( w, e ) with minimal
      // w, for which w >= v. 

   static int compare( const groundsubst& s1, const groundsubst& s2 );
      // Returns -1 if s1 < s2,
      // Return 0 if s1 == s2 (modulo extensionality)
      // Return 1 if s1 > s2.

public: 
   bool operator == ( const groundsubst& s ) const 
   {
      return compare( *this, s ) == 0;
   }

   bool operator != ( const groundsubst& s ) const 
   {
      return compare( *this, s ) != 0;
   }

   bool operator < ( const groundsubst& s ) const 
   {
      return compare( *this, s ) == -1;
   }

   bool operator > ( const groundsubst& s ) const 
   {
      return compare( *this, s ) == 1;
   }

   bool operator <= ( const groundsubst& s ) const 
   {
      return compare( *this, s ) <= 0;
   }

   bool operator >= ( const groundsubst& s ) const 
   {
      return compare( *this, s ) >= 0;
   }

};


inline void groundsubst::clear( )
{
   repr. clear( ); 
}
   

inline unsigned int groundsubst::size( ) const
{
   return repr. size( ); 
}


inline void groundsubst::backtrack( unsigned int x )
{
   ASSERT( x <= repr. size( ));
   while( x < repr. size( ))
      repr. pop_back( );
}


groundsubst operator + ( const groundsubst& ,
		         const groundsubst& );



#endif


