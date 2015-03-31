
// Discrimination tree and discrimination forest.
// Written by Hans de Nivelle, September 2014. 

// I wait with the implementation of discrimination tree, because
// I first have to measure its chance of success from realistic runs.
// (10.09.2014)
// The effectiveness of discrimination trees essentially depends on the
// effectiveness of filtering, which must be measured first. 


#ifndef GEOMETRIC_DISCTREE_INCLUDED
#define GEOMETRIC_DISCTREE_INCLUDED  1

#include <vector>
#include "atom.h"


namespace geometric
{

   class discforest
   {

      // A forest is a list of solutions combined with a set of
      // trees. A tree has a unique top node. We distinguish
      // three types of tree, dependent on the type of top node.
      // Details are in the paper. 

      struct positive_tree;
      struct negative_tree;
      struct existential_tree;

      unsigned int maxsubstsize;
         // This is the maximal size up to which we allow a substqueue 
         // to grow when retrieving. When the substitution 
         // gets longer, we remove variables from the front.
         // Visited substqueues are cached. In this way, we hope to 
         // avoid exponentiality.

      std::vector< unsigned int > solutions;

      std::vector< positive_tree > positive_sub;
      std::vector< negative_tree > negative_sub;
      std::vector< existential_tree > existential_sub;


   public:
      unsigned int size( ) const; 
         // Total size of the forest. 

   };


   // A positive tree has form A(f), where A is an atom that
   // must be matched, and f is a forest.

   struct discforest::positive_tree
   {
      varatom at;

      discforest subforest;
      unsigned int size( ) const;
         // Total size of the tree. 
   };


   // A negative tree has form (! A)(f), denoting that atom 
   // A should not be matchable.

   struct discforest::negative_tree
   {
      varatom at;

      discforest subforest;
      unsigned int size( ) const;
         // Total size of the tree.
   };


   // An existential tree has form (Exists v. A)(f) denoting
   // that for all instances d of v, the atom A[ v := d ] should not 
   // be matchable. Variable v must not occur in at.

   struct discforest::existential_tree
   {
      variable v;
      varatom at;

      discforest subforest;
      unsigned int size( ) const;
         // Total size of the tree.
   };


}

#endif

