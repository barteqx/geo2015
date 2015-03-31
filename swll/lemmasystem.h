

// Hans de Nivelle, April 2005.


#ifndef SWLL_LEMMASYSTEM_DEFINED
#define SWLL_LEMMASYSTEM_DEFINED    1


#include <iostream>
#include <vector>
 

#include "../variable.h"

#include "lemma.h"
#include "unsignedsubst.h"


namespace swll
{ 

   // A lemmasystem maintains a set of lemmas that are true relative
   // to a substitution. The substitution is a reference member
   // of the lemmasystem. 
   // In each true lemma, one of the literals is a 'watched literal'. 
   // A lemma is an incomplete data structure. It needs a 
   // std::vector< lemma::atom > to store its atoms.
   // The lemmasystem does not care about this. Maintaining the
   // vector (and ensuring that it lives long enough) is responsibility
   // of the user. 

   class lemmasystem 
   {

   private: 
      mutable std::vector< std::vector< std::vector< lemma > > > watchmatrix;
         // It is mutable, so that findfalselemma can be a const
	 // method. In case the watched atom of a lemma becomes true, it 
	 // will be necessary to start watching another atom. 


   private: 
      std::vector< lemma > * getwatchinglemmas( lemma::atom at ) const;
         // Returns a pointer to the vector of lemmas that watch the atom
	 // at if such a vector exists. Because insert 
	 // creates all entries for all atoms occurring in the lemma,
	 // a result of 0 implies that no lemma is watching the 
	 // assignment (at.v := at.val )
	 // Note that this can be a const method due to the fact that
	 // watchmatrix is a mutable member. 
         // Also note that getwatchinglemmas does not reallocate.
	 // It is ensured by insert that watchmatrix is big enough, 
	 // so that all atoms in a lemma can become watched atom
	 // without reallocation of watchmatrix. 

      const lemma::atom* findfalseatom( const lemma& lem ) const; 
         // Returns a pointer to a false atom in lem if there exists 
         // one. Because lemmas have form A1 /\ ... /\ Ap -> FALSE,
         // a lemma is true as long as it has a false atom. 

      bool ispreferredover( const lemma& lem1, const lemma& lem2 ) const;
         // In case there is more than one false lemma, ispreferredover 
	 // decides which lemma is returned by findfalselemma( ). 
         // preferredover( ) is not a static method because it might look
         // at subst in order to make its decision. 
	 // ispreferredover should be called on false lemmas only. 


      void insert( const lemma& lem, lemma::atom w ) const; 
         // insert lem with watched atom w.


   public:
      const unsignedsubst& subst;
         // The substitution will change a lot, but it will
	 // not be changed by us.

   public:
      explicit lemmasystem( const unsignedsubst& subst ) 
         : subst( subst )
      { } 

      
      void clear( );
         // The watched substitution subst is not cleared.

      void insert( const lemma& lem );
         // Insert lem, which has to be true at the moment of its
	 // insertion. 
         // Remember that you, the user, are responsible for maintaining 
	 // lem. atoms_base.

      const lemma* findfalselemma( unsigned int base ) const; 
         // Returns the best false lemma. This method 
	 // looks at the assignments in subst that were made
	 // after base, and it checks the lemmas that watch 
	 // one of these assigments. 


      bool isfalse( const lemma& lem ) const; 
         // True if the lemma is false. 

      unsigned int nrlemmas( ) const;
         // Number of lemmas in the system. This is a linear time 
	 // operation.

      bool subsumes( const lemma& lem ) const;
         // True if the lemma is subsumed by a lemma in the system.
	 // This is a very expensive operation, but it has shown
	 // its usefulness in debugging. It is linear in the size
	 // of the system. 

      void removebacksubsumed( const lemma& lem ) const;
         // Checker whether there is a lemma in the system backsubsumed by
	 // lem. This operation is not efficient, but I would
	 // like to know whether it happens at all. 

      friend std::ostream& operator << ( std::ostream& , const lemmasystem& ); 

   };

   std::ostream& operator << ( std::ostream & , const lemmasystem& ); 



   inline const lemma::atom* 
   lemmasystem::findfalseatom( const lemma& lem ) const
   {
      for( lemma::atom_iterator 
              p = lem. atoms_begin( );
	      p != lem. atoms_end( );
	      ++ p )
      {
         const unsigned int *val = subst. lookup( p -> v );
	 if( val == 0 || *val != ( p -> val )) 
            return &(*p);
      }
      return 0;
   }


   inline bool lemmasystem::isfalse( const lemma& lem ) const
   {
      return findfalseatom( lem ) == 0;
   }


   void testlemmasystem( ); 

} 


#endif

