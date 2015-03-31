
// Code written by Hans de Nivelle, 2006.

#ifndef SWLL_LEMMA_DEFINED 
#define SWLL_LEMMA_DEFINED    1


#include <vector>
#include <list>
#include <iostream>


#include "../variable.h"


namespace swll
{


   // A lemma has form v1:=t1 /\ ... vn := tn -> False.
   // These are the things that are learnt by swll
   // (subsumption with lemma learning) 

   class lemma
   {
   public: 
      struct atom
      {
         variable v;
	 unsigned int val;

	 atom( variable v, unsigned int val )
            : v(v), val(val)
         {
         }

      };

   private: 
      std::vector< atom > & atoms_base;

      unsigned int atom1, atom2;
         // Indices in atoms_base.

      lemma( std::vector< atom > & atoms_base, 
             unsigned int atom1, unsigned int atom2 ) 
         : atoms_base( atoms_base ), atom1( atom1 ), atom2( atom2 )
      {
      }


   public: 
      explicit lemma( std::vector< atom > & atoms_base );
         // Construct empty lemma.

      lemma( std::vector< atom > & atoms_base,
             const std::list< atom > & p );
         // Construct lemma from the assignments in p.

      lemma( const lemma& other );
         // Construct lemma in same vector. 
      void operator = ( const lemma& other );
         // Copy lemma within same vector.

      lemma clone_into( std::vector< atom > & atoms_base ) const; 
         // Copy lemma into different vector. 

      bool addatom( const atom& at );
         // Add an atom to the lemma. In order to call this method, 
	 // the lemma has to be last in its container.
	 // The container will be possibly extended. 

      bool contains( variable v ) const;
         // True if the lemma contains an atom containing v.

      static lemma resolve( std::vector< atom > & atoms_base, 
		            const std::list< lemma > & parents,
		            const std::list< variable > & hypotheses,
		            const std::list< atom > & conflicts );
         // The hypotheses are the variables resolved upon.
	 // The conflicts are the assignments that cause conflicts
	 // with the other possibilities in the clause. 

      static lemma resolve2( std::vector< atom > & atoms_base,
		             const std::list< lemma > & parents,
			     const std::list< variable > & hypotheses,
			     const std::list< atom > & conflicts );
         // This is the same as resolve, but it prints what it does.
	 // This is useful for debugging the subsumption algorithms. 
	


      bool subsumes( const lemma& other ) const; 
      
      unsigned int size( ) const { return atom2 - atom1; } 


      struct atom_iterator 
      {
         std::vector< atom > :: const_iterator a;

	 explicit atom_iterator( std::vector< atom > :: const_iterator a )
            :a(a)
         {
         }


	 atom_iterator operator ++ ( int )
         {
            return atom_iterator( a ++ );
         }

	 atom_iterator operator ++ (  )
         {
            return atom_iterator( ++ a );
         }


	 bool operator == ( atom_iterator other ) const
         {
            return a == other. a;
         }

	 bool operator != ( atom_iterator other ) const
         {
            return a != other. a;
         }

	 const atom& operator * ( ) const
         {
            return *a;
         }

	 const atom* operator -> ( ) const
         {
            return & (*a);
         }

      }; 


      atom_iterator atoms_begin( ) const
      {
         return atom_iterator( atoms_base. begin( ) + atom1 );
      }

      atom_iterator atoms_end( ) const
      {
         return atom_iterator( atoms_base. begin( ) + atom2 );
      }


   };

   std::ostream& operator << ( std::ostream& , const lemma& );


   inline lemma::lemma( const lemma& other )
      : atoms_base( other. atoms_base ),
        atom1( other. atom1 ),
        atom2( other. atom2 )
   {
   }


   inline void lemma::operator = ( const lemma& other )
   {
      ASSERT( & atoms_base == & other. atoms_base );
      atom1 = other. atom1;
      atom2 = other. atom2;
   }



   inline lemma::lemma( std::vector< atom > & atoms_base )
      : atoms_base( atoms_base ),
        atom1( atoms_base. size( )),
        atom2( atoms_base. size( ))
   {
   }


   inline bool lemma::addatom( const atom& at )
   {
      ASSERT( atoms_base. size( ) == atom2 );
      for( unsigned int i = atom1; i != atom2; ++ i )
      {
         if( atoms_base [i]. v == at. v &&
	     atoms_base [i]. val == at. val ) 
         {
            return false;
         }
      }
      atoms_base. push_back( at );
      ++ atom2;
      return true;
   }


   inline bool lemma::contains( variable v ) const 
   {
      for( unsigned int i = atom1; i != atom2; ++ i )
      {
         if( atoms_base [i]. v == v )
            return true;
      }
      return false;
   }

} 


#endif


