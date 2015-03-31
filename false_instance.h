

// Code written by Hans de Nivelle, November 2005. 


#ifndef FALSE_INSTANCE_INCLUDED
#define FALSE_INSTANCE_INCLUDED   1


#include <vector>
#include <iostream>


#include "groundsubst.h"
#include "rule.h"
#include "model.h"


// A false_instance is an instance that falsifies a rule. 
//
// It would probably be a good idea to add an iterator
// that would simultaneously walk through the premisses
// of the rule and the prem_match.


struct false_instance 
{

   rule r;

   groundsubst s;
      // Instantiation that makes r false. 

   std::vector < unsigned int > prem_match; 
      // List of numbers of atoms in the model that make the 
      // premisse true. The number of a disequality
      // V != W is the number of the #-atom for the greater
      // of V and W. 
      // The atom_nrs are in the same order as the premisses
      // that they correspond to. 

   false_instance( const rule& r,
		   const groundsubst& s,
		   const std::vector< unsigned int > & prem_match ) 
   : r(r), 
     s(s),
     prem_match( prem_match )
   {
   }


   explicit false_instance( const rule& r )
   : r(r)
   {
   }

   bool computesubst( const model& m );
      // Compute substitution s from prem_match;
      // It returns false if something is wrong
      // in prem_match. This is checked pretty well.

   bool computematching( const model& m );
      // Computes prem_match from s. 
      // false is returned if something goes wrong. 


   unsigned int sup_prem_match( ) const; 
      // Supremum of integers in prem_match.

   element sup_elements( ) const
   {
      return s. sup_elements( );
   }
      // Supremum of elements in the range of s.

};


std::ostream& operator << ( std::ostream& , const false_instance & );


#endif


