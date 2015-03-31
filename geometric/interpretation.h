
// Written by Hans de Nivelle, February 2015.
// An interpretation is a pair ( E, M ), where E is a set
// of elements, and M is a set of atoms.
// We represent E by a vector, and M by a simple index.

#ifndef GEOMETRIC_INTERPRETATION_INCLUDED
#define GEOMETRIC_INTERPRETATION_INCLUDED   1

#include <vector>
#include <iostream>

#include "age.h"
#include "permutationgenerator.h"
#include "false_instance.h"

namespace geometric
{

   struct interpretation
   {

      // We have a vector of guesses to support backtracking.

      struct guesser 
      {
         size_t at_index;              // Index into interpretation::atoms. 
         std::vector<size_t> choices; 
         size_t j;  
            // Current choice.

         unsigned int closure_counter; 
            // Number in closure counter of interpretation 
            // when this guess was guessed.

         age time;
            // Time on which this guess was guessed. 

         guesser( size_t at_index, 
                  std::vector<size_t> && choices, 
                  unsigned int closure_counter )
            : at_index{ at_index }, 
              choices{ std::move(choices) }, 
              j{0},
              closure_counter{ closure_counter }
         { } 
      };


      std::vector< element > elements;
      index::standardsimple< element > atoms;  
      size_t nrinitials; 
         // The atoms in the current interpretation, and
         // how many of them are initial. 
            
      std::vector< false_instance > justifications;
         // An atom with index < nrinitials has no justification.
         // An atom with index >= nrinitials is based on 
         // justifications[ index - nrinitials ].

      std::vector< guesser > guesses;

      unsigned int closure_counter;

      interpretation( const std::vector< groundatom > & start );

      size_t findcreate( element e ) const;
         // Find the create atom for e.

      size_t find( const varatom& at, const subst<element> & subst ) const;
         // Return an index i, s.t. atoms[i] equals subst(at), or is
         // in conflict with subst(at). Returns atoms. size( ) if no
         // such atom exists. Substitution subst must instantiate at fully. 

      size_t find( variable v, const varatom& at,
                   const subst<element> & subst ) const;
         // Return an index i, s.t. there exists an instantiation v:=e, 
         // for which subst+{v:=e}(at) equals atoms[i].

      size_t findconflict( const varatom& at, 
                           const subst<element> & subst ) const;
      size_t findconflict( const varatom& at,
                           const subst<element> & subst,
                           variable v, element e ) const; 

         // Return index i, st.t. atoms[i] is in conflict with at
         // instantiated by subst, or atoms. size( ) if no such atom exists.
         // If v occurs in subst, then assignment { v:=e } overrides subst.

      size_t conflictlevel( const lemma& lem, 
                            const subst<element> & subst ) const;
         // Return the smallest number s, s.t. at is in conflict with 
         // the atoms in our interpretation with index < s. Returning 
         // atoms. size( ) means that atoms was in conflict, and that
         // atoms [ atoms. size( ) - 1 ] was used. 
         // A return value > atoms. size( ) means that lem is not in
         // conflict. 

      bool conclusionistrue( const lemma& lem, 
                             const subst<element> & subst ) const;
      bool conclusionistrue( const disjunctive& disj, 
                             const subst<element> & subst ) const;
      bool conclusionistrue( const existential& exists,
                             const subst<element> & subst ) const;
         // True if the conclusion of the rule is true.

      bool conclusionconflicts( const lemma& lem,
                                const subst<element> & subst ) const;
      bool conclusionconflicts( const disjunctive& disj,
                                const subst<element> & subst,
                                size_t j ) const;
      bool conclusionconflicts( const existential& exists,
                                const subst<element> & subst,
                                size_t j ) const;
         // True if the conclusion is in conflict. 
         // For example, it is P_T( ), and P_E( ) is present.

      size_t nrchoices( const lemma& lem,
                        const subst<element> & subst ) const;
      size_t nrchoices( const disjunctive& disj,
                        const subst<element> & subst ) const; 
      size_t nrchoices( const existential& exists,
                        const subst<element> & subst ) const;
         // The true number of choices, i.e. those that do not immediately
         // cause a conflict. 
 
      void extend( const lemma& lem, const subst<element> & subst,
                   false_instance&& just );
         // Add the head of lemma to the interpretation. We don't check
         // anything, we just bluntly add the conclusion. 

      void extend( const disjunctive& disj, const subst<element> & subst,
                   false_instance&& just, size_t j );
         // Extend interpretation by the j-th active atom of 
         // disj. Nothing is checked.

      void extend( const existential& exists, const subst<element> & subst,
                   false_instance&& just, size_t j );
         // Extend interpretation by conclusion of exists, instantiated
         // by j-th element. Nothing is checked.

      void pop_deterministic( );
         // Remove an atom that must have been obtained by extending
         // with a lemma.

      void pop_nondeterministic( );
         // Remove an atom (or two atoms) that have been obtained by
         // extending with a guess.

      false_instance& lastjustification( ) { return justifications. back( ); }

      guesser& addguesser( std::vector< size_t> && choices );
        // Add a guesser (backtracker). No atoms or elements are added.

      void pop_guesser( );
         // Remove last guesser. First the associated atoms
         // have to be withdrawn (using pop_nondeterministic.) 

      guesser& lastguesser( ) { return guesses. back( ); }
      const guesser& lastguesser( ) const { return guesses. back( ); }

      element lastelement( ) const { return elements. back( ); }

      size_t nrguesses( ) const { return guesses. size( ); }
      size_t extension_size( ) const { return justifications. size( ); }
      size_t atoms_size( ) const { return atoms. size( ); }
      size_t nrelements( ) const { return elements. size( ); }
 
      // User should not call this:

      void pop_atom( )
      {
   
         size_t at = atoms. size( ) - 1;
         if( atoms[at]. p. iscreate( ) && atoms[at]. t == truthval::T )
         {
            element e = atoms[at][0];
            if( elements. empty( ) || e != elements. back( ))
               throw std::runtime_error( "create does not fit" );
            elements. pop_back( );
         }
         atoms. restore( at );
         justifications. pop_back( );
      }
         
      
      set< size_t > getassumptions( size_t at ) const
      {
         return ( at >= nrinitials ) ? 
            justifications[ at - nrinitials ]. assumptions : 
            set< size_t > ( {} ); 
      } 
         // Returns the justifications that are associated to the at-th atom. 

      set< size_t > getassumptions( const rule& r, 
                                    const subst<element> & inst ) const;
         // Collect the assumptions of the atoms that the passive atoms 
         // of rule r are in conflict with. 

   };

   std::ostream& operator << ( std::ostream& out, const interpretation& i );
   std::ostream& operator << ( std::ostream& out, 
                               const interpretation::guesser& );


   ///////////////////////////////////////////////////////
   //  A few inline functions:

   inline bool 
   interpretation::conclusionistrue( const lemma& lem,
                                     const subst<element> & subst ) const
   {
      return lem. hasactive( ) &&
             find( lem. getactive( ), subst ) != atoms. size( );
   }


   inline bool 
   interpretation::conclusionistrue( const disjunctive& disj,
                                     const subst<element> & subst ) const
   {
      for( const varatom& at : disj. getactive( ))
      {
         if( find( at, subst ) != atoms. size( ))
            return true;
      }

      return false;
   }


   inline bool 
   interpretation::conclusionistrue( const existential& exists,
                                     const subst<element> & subst ) const
   {
      return find( exists. getvariable( ), exists. getatom( ), subst )
                              != atoms. size( );
   }


   inline bool
   interpretation::conclusionconflicts( const lemma& lem,
                                        const subst<element> & subst ) const
   {
      return !lem.hasactive( ) ||
             findconflict( lem. getactive( ), subst ) != atoms. size( );
   }

   inline bool
   interpretation::conclusionconflicts( const disjunctive& disj,
                                        const subst<element> & subst,
                                        size_t j ) const
   {
      return findconflict( disj[j], subst ) != atoms. size( );
   }

   inline bool
   interpretation::conclusionconflicts( const existential& ex,
                                        const subst<element> & subst,
                                        size_t j ) const
   {
      if( j < elements. size( ))
         return findconflict( ex. getatom( ), subst, 
                    ex.getvariable( ), elements[j] ) != atoms. size( );

      return false;
         // Extending never conflicts, because the fresh variable 
         // must occur in the atom. 
   }


   inline size_t 
   interpretation::nrchoices( const lemma& lem,
                              const subst<element> & subst ) const
   {
      if( conclusionconflicts( lem, subst ))
         return 0;
      else
         return 1;
   }
 
   inline size_t 
   interpretation::nrchoices( const disjunctive& disj,
                              const subst<element> & subst ) const
   {
      size_t choices = 0;
      for( size_t j = 0; j != disj. nractive( ); ++ j )
      {
         if( !conclusionconflicts( disj, subst, j ))
            ++ choices;
      }
      return choices;
   }

   inline size_t 
   interpretation::nrchoices( const existential& exists,
                              const subst<element> & subst ) const
   {
      size_t choices = 0;
      for( size_t j = 0; j < nrelements( ); ++ j )
      {
         if( !conclusionconflicts( exists, subst, j ))
            ++ choices;
      }
      return choices + 1;  // Because a new element is always possible.
   }
 
}


#endif

