
// Written by Hans de Nivelle, January 2015.

#ifndef GEOMETRIC_UNIFICATION_INCLUDED
#define GEOMETRIC_UNIFICATION_INCLUDED   1

#include <iostream> 
#include <unordered_map>
#include "atom.h"

namespace geometric
{

   // Label are used to identify the source from which a variable
   // originates. If one self resolves 
   // !p(X), p(s(X)), then the two instances should have different
   // labels. 

   struct label
   {
      int tag;

      explicit label( int tag )
         : tag{ tag }
      {
      }

      bool operator == ( label other ) const { return tag == other. tag; }
      bool operator != ( label other ) const { return tag != other. tag; }
   };

   inline std::ostream& operator << ( std::ostream& stream, label lab )
   {
      stream << "label(" << lab.tag << ")";
      return stream;
   }


   struct labeledvar
   {
      variable var;
      label lab;

      labeledvar( variable var, label lab )
         : var{var}, lab{lab}
      { }

      struct hash
      {
         variable::hash h;

         size_t operator( ) ( labeledvar lv ) const
         {
            return 37 * h( lv.var ) + lv.lab.tag;
         }
      };

      struct equal_to
      {
         bool operator( ) ( labeledvar lv1, labeledvar lv2 ) const
         {
            return lv1.var == lv2.var && lv1.lab == lv2.lab;
         }
      };

   };

   std::ostream& operator << ( std::ostream& out, labeledvar lv ); 

   inline bool operator == ( labeledvar lv1, labeledvar lv2 )
   {
      return lv1.var == lv2.var && lv1.lab == lv2.lab;
   }

   inline bool operator != ( labeledvar lv1, labeledvar lv2 )
   {
      return lv1.var != lv2.var || lv1.lab != lv2. lab;
   }
 

   class normalizer
   {

      std::unordered_map< labeledvar, variable,
                          labeledvar::hash, labeledvar::equal_to > repr;
      variable nextvar;

   public:
      void clear( ) { repr. clear( ); nextvar = variable( ); }

      size_t size( ) const { return repr. size( ); }

      variable lookup( variable v, label l );
      varatom apply( varatom at, label l );
         // Always succeed, because the normalizer is extended.

      bool contains( variable v, label l ) const; 
         
      friend std::ostream& operator << ( std::ostream&, const normalizer& );
   };


   // It is really a union_find, maybe it should be rewritten.

   class unifier
   {
      std::unordered_map< labeledvar, labeledvar,
                          labeledvar::hash, labeledvar::equal_to > repr;
   
      void normalize( labeledvar& lv ) const
      {
         auto p = repr. find( lv );
         while( p != repr. end( ))
         {
            lv = p -> second;
            p = repr. find( lv );
         }
      }
 
   public:
      void clear( ) { repr. clear( ); } 

      size_t size( ) const { return repr. size( ); }

      void unify( variable v1, label l1, variable v2, label l2 );
      void unify_arguments( const varatom& at1, label l1,
                            const varatom& at2, label l2 );
         // Assumes that the predicate symbols have the same arity.
         // Truth values should be checked separately.
         // Because there are only variables involved, unification always
         // succeeds. 
  
      variable apply( normalizer& norm, variable v, label l ) const;
         // Possibly extends the normalizer, but not the unifier.

      varatom apply( normalizer& norm, varatom at, label l ) const;
    
      friend std::ostream& operator << ( std::ostream& , const unifier& );
 
   };

}

#endif


