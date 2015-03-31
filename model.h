

// Code written by Hans de Nivelle, November 2005. 
// Modified February 2007. 
// - Deleted the marking mechanism. (because it has become obsolete in
//   the context of lemma generation) 
// - Added weight and level to atoms, in the hope that these can be used in 
//   rule selection.


#ifndef MODEL_INCLUDED
#define MODEL_INCLUDED   1


#include <vector>
#include <iostream>


#include "element.h"
#include "predicate.h"
#include "groundatom.h"
#include "geometric/index/simple.h"

class model 
{

   std::vector< element > arguments; 
      // Arguments of the atoms. 

   std::vector< unsigned int > parents; 
      // Parents of the atoms. 

   struct atom_in_model
   {
      predicate p; 
      unsigned int args1;
      unsigned int args2;   
         // The arguments are in arguments [ args1 .. args2 ].

      unsigned int rule_nr;
         // Number of the rule with which the atom was constructed.

      unsigned int pars1;
      unsigned int pars2;
        // The indices of the parents are in predicates 
	// [ parents1 .. parents 2 ].       

      unsigned int timestamp;
         // This allows the user to see how old an atom is, 
	 // so that he can have some idea where the prover
	 // is spending/wasting its time. 

      unsigned int level;
         // The level of an atom is the supremum of the set of levels of 
	 // the parents of the atom.  

      unsigned int weight;
         // The weight of an atom equals the sum of the weights
	 // of the parents + 1. I am not sure if the weight remains
	 // below 2^32, but one has to make a first try.

      unsigned int alternative;
      unsigned int nr_alternatives;
         // So that the user can see where we are 
	 // in the search.

      atom_in_model( predicate p, 
            unsigned int args1, unsigned args2,
	    unsigned int rule_nr, 
	    unsigned int pars1, unsigned int pars2,
	    unsigned int timestamp,
	    unsigned int level,
	    unsigned int weight, 
	    unsigned int alternative,
	    unsigned int nr_alternatives )
         : p(p), args1( args1 ), args2( args2 ),
	   rule_nr( rule_nr ), 
	   pars1( pars1 ), pars2( pars2 ),
	   timestamp( timestamp ),
	   level( level ),
	   weight( weight ), 
	   alternative( alternative ),
	   nr_alternatives( nr_alternatives ) 
      {
      }
   };

   std::vector< atom_in_model > atoms; 

public:
   unsigned int closure_counter;

   model( ):
      closure_counter(0)
   {
   }


   bool addatom( groundatom at, unsigned int rule_nr, 
		 const std::vector< unsigned int > & parents,
		 unsigned int alternative = 0,
		 unsigned int nr_alternatives = 1 );
      // The atom is only added when it is unique.
      // addatom returns true when the atom is really added.


   bool canbeadded( groundatom at ) const; 
      // True if the atom at can be added to this model. This is the case
      // when all its variables V occur in a create atom #V.


public: 
   void backtrack( unsigned int s );
      // backtrack( ) backtracks the model. After backtrack(s), 
      // one will have nr_atoms( ) == s. 

   unsigned int nr_atoms( ) const
   {
      return atoms. size( );
   } 

   unsigned int nr_elements( ) const;
      // Number of elements in the model.
      // For example, ( #E0, P(E0), #E1, p(E0,E1) ) has
      // 2 elements. 

   struct atom_iterator
   {
      std::vector< atom_in_model > :: const_iterator a;

      explicit 
      atom_iterator( std::vector< atom_in_model > :: const_iterator a )
         : a(a)
      {
      }


      atom_iterator operator ++ ( int )
      {
         return atom_iterator( a ++ ); 
      }

      atom_iterator operator -- ( int )
      {
          return atom_iterator( a -- );
      }

      atom_iterator operator ++ ( )
      {
         return atom_iterator( ++ a ); 
      }

      atom_iterator operator -- ( ) 
      {
         return atom_iterator( -- a );
      }

      bool operator == ( atom_iterator other ) const 
      {
         return ( this -> a ) == other. a;
      }

      bool operator != ( atom_iterator other ) const 
      {
	 return ( this -> a ) != other. a;
      }

   };


   atom_iterator atoms_begin( ) const 
   {
      return atom_iterator( atoms. begin( )); 
   }

   atom_iterator atoms_end( ) const 
   {
      return atom_iterator( atoms. end( ));
   } 

   atom_iterator make_atom_iterator( unsigned int nr ) const
   {
      ASSERT( nr < atoms. size( ));
      return atom_iterator( atoms. begin( ) + nr );
   }

   groundatom getatom( atom_iterator at ) const 
   {
      return groundatom( at. a -> p, 
         arguments. begin( ) + at. a -> args1,
	 arguments. begin( ) + at. a -> args2 );
   }


   unsigned int getrulenr( atom_iterator at ) const
   {
      return at. a -> rule_nr;
   }

   unsigned int gettimestamp( atom_iterator at ) const
   {
      return at. a -> timestamp;
   }

   unsigned int getlevel( atom_iterator at ) const
   {
      return at. a -> level;
   }

   unsigned int getweight( atom_iterator at ) const
   {
      return at. a -> weight;
   }

   unsigned int getalternative( atom_iterator at ) const
   {
      return at. a -> alternative;
   }

   unsigned int getnralternatives( atom_iterator at ) const
   {
      return at. a -> nr_alternatives;
   }



   ///////////////////////////////////////////////////////////////////
   // A parent_iterator iterates through the parents of an atom.    //
   ///////////////////////////////////////////////////////////////////

   struct parent_iterator
   {
      std::vector< unsigned int > :: const_iterator p;

      explicit 
      parent_iterator( std::vector< unsigned int > :: const_iterator p )
         : p(p)
      {
      }


      parent_iterator operator ++ ( int )
      {
         return parent_iterator( p ++ ); 
      }

      parent_iterator operator ++ ( )
      {
         return parent_iterator( ++ p ); 
      }

      bool operator == ( parent_iterator other ) const 
      {
         return ( this -> p ) == other. p;
      }

      bool operator != ( parent_iterator other ) const 
      {
	 return ( this -> p ) != other. p;
      }

      unsigned int operator * ( ) const
      {
         return *p;
      }

   };


   parent_iterator parents_begin( atom_iterator at ) const 
   {
      return parent_iterator( parents. begin( ) + at.a -> pars1 ); 
   }

   parent_iterator parents_end( atom_iterator at ) const 
   {
      return parent_iterator( parents. begin( ) + at.a -> pars2 );
   } 


   geometric::index::standardsimple<element>
   make_simple_index( geometric::signature& sig ) const;
      // Put the model in a simple index.
};


std::ostream& operator << ( std::ostream& , const model & );


#endif


