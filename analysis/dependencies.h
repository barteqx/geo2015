
// Code written by Hans de Nivelle, June 2007.


#ifndef ANALYSIS_DEPENDENCIES_INCLUDED
#define ANALYSIS_DEPENDENCIES_INCLUDED   1

#include <list>

#include "../assert.h"
#include "../predicate.h"
#include "../rule.h"


namespace analysis
{

   // The dependencies are either the set of dependencies for a
   // single predicate symbol, or the set of dependencies in the
   // complete premisse of a rule. 
   // 
   // A dependency_system attaches to predicate symbols a 
   // set of dependencies within that symbol. 
   // 
   // When computing inherited dependencies, one gets stronger results
   // when all the dependencies of all premisses are merged into a single
   // big dependency. 
   // This happens for example in a(x,y) /\ b(y,z) -> c(x,z) if one knows 
   // that a/0 = a/1 and b/0 = b/1. 
   // 
   // It is assumed that arguments are numbered 0, ..., a - 1.
   // Positions in the premisses of a rule are numbered 0, ..., n - 1,
   // and the atom positions are not counted. 


   struct dependencies 
   {

      struct equation
      {
         unsigned int lhs;
         unsigned int rhs;

         equation( unsigned int lhs, unsigned int rhs )
            : lhs( lhs ), rhs( rhs )
         {
         }

      };


      struct skolem
      {
         unsigned int lhs;
         predicate f; 
         std::list< unsigned int > rhs;

   
         skolem( unsigned int lhs, predicate f, 
                 const std::list< unsigned int > & rhs )
            : lhs( lhs ), f(f), rhs( rhs )
         {
            ASSERT( rhs. size( ) < f. getarity( ));
         }
      }; 


   private:
      std::list< equation > equations;
      std::list< skolem > skolems;       


   public:
      void addequation( unsigned int lhs, unsigned int rhs );
      void addskolem( unsigned int lhs, predicate f,
                      const std::list< unsigned int > & rhs ); 
 

      void reduce( );
         // Reduce the dependencies. This means that we view the
         // dependencies as rewrite system, and do a Knuth-Bendix completion. 
     
 
      static dependencies intersection( const dependencies& dep1,
                                        const dependencies& dep2 );
         // This function computes the set of dependencies that are implied
         // by both dep1 and dep2. (The disjunction in some sense) 
         // Both of the premisses should be reduced. 
         // The result will be reduced again. 


      static dependencies merging( const dependencies& dep1,
                                   const dependencies& dep2 );
         // This function computes the set of dependencies that are implied
         // by either dep1 or by dep2. In some sense this is the 
         // conjunction. I would have called it union, if that would
         // not have been a reserved word. 
         // The result is not reduced, and the premisses need to be reduced. 

      
      void mergewith( const dependencies& dep );
         // Merge the present system with dep. Neither of the 
         // dependency systems involved needs to be reduced. 

      void mergewith( const dependencies& dep, 
                      rule::prem_iterator p1, rule::prem_iterator p2 );
         // Merge the present system with dep on the assumption 
         // that it represents the dependencies for the atom on 
         // p2. Arguments in dep are looked up, and then replaced by 
         // the index of the first occurrence in p1 ... p2.
         // Note that the range p1 ... p2 does not have the usual
         // semantics, because *p2 is included in the range. 
         // Neither of the dependency systems involved needs to be
         // reduced. 

      void project( rule::prem_iterator p1, rule::prem_iterator p2,
                    varatom at );

         // Project the system to those positions whose contents occurs
         // in at. This is more than just deleting positions with contents
         // outside at, because two positions with contents inside at,
         // may be equal through a position inside at. 

      void transfertoconclusion( rule::prem_iterator p1,
                                 rule::prem_iterator p2,
                                 varatom conc );
         // Carry the set of dependencies over to the conclusion. 
         // The current set of dependencies need not be normalized. 
         // However, it should speak only about positions whose
         // contents occur in conc. This is automatically guaranteed
         // when the dependencies have been project upon at before. 

      bool implies( const dependencies& dep ) const;
         // True if the present system implies the other system.
         // The present system must be reduced, the other one 
         // need not.


      std::list< rule > axioms( std::vector< predicate > & predicates_base,
                                std::vector< variable > & arguments_base,
                                predicate p ) const;
         // Assuming that the dependencies concern predicate p, 
         // generate a set of axioms.
         // An equation generates an axiom of form
         //    p( X1, ..., Xn ) /\ Xi != Xj -> False.
         // A Skolem dependency Y = f(X1,..., Xn) generates an axiom of form
         //    p( X1, ..., Xn, Y1, Z1, ..., Zm ) /\ 
         //    p( X1, ..., Xn, Y2, U1, ..., Um ) /\ 
         //    Y1 != Y2 -> False.  
         // I have also considered implementing a test function,
         // which would test whether the dependencies are true for a given
         // predicate in a given model m, but this can be tested more
         // easily by generating the axioms and ensuring that none of them 
         // is applicable in m. 
         // It is a good idea to reduce the dependency system before 
         // generating the axioms because the axioms will be simpler. 



      static dependencies readequations( varatom at );
         // Read the equations from the equal arguments of at. 

      static dependencies readskolem( variable ex, varatom at );
         // Read the functional dependencies that would
         // arise from # ex /\ at.
         // In case ex occurs more than once, we generate only the 
         // first Skolem expression. Generating more is not necessary, 
         // as normally also the equations between the distinct 
         // occurrences of ex are generated. 


      friend std::ostream& operator << ( std::ostream& , const dependencies& );


   private:
      static int commonrepresentative( const dependencies& dep1, 
                                       unsigned int x1,
                                       const dependencies& dep2,
                                       unsigned int x2 ); 
         // Find a term x, s.t. dep1 |-   ( x = x1 ),
         //                     dep2 |-   ( x = x2 ).
         // If such term exists, we return it, otherwise -1.

      static int firstoccurrence( variable v, varatom at ); 
         // If variable v occurs in at, then return the index of the first
         // occurrence, otherwise -1. 

      static int firstoccurrence( variable v, 
                                  rule::prem_iterator p1, 
                                  rule::prem_iterator p2 );
         // If the variable v occurs in p1 ... p2 then return the 
         // index of the first occurrence, otherwise -1. 

      static variable contents( unsigned int x, varatom at );
         // Returns the x-th argument of at. 

      static variable contents( unsigned int x, 
                                rule::prem_iterator p1, 
                                rule::prem_iterator p2 );  
         // Return the variable on the x-th position of the sequence
         // p1 ... p2. 

      unsigned int normalization( unsigned int x ) const;
         // Produce the normalization of x. When the set of dependencies
         // is reduced, it is a confluent rewrite system, so that 
         // normalization makes sense.

      void replace( unsigned int x1, unsigned int x2 );
         // Replace all occurrences of x1 to x2. 
         // This method is used by project. 
   };


   std::ostream& operator << ( std::ostream& , dependencies::equation );
   std::ostream& operator << ( std::ostream& , const dependencies::skolem& );
   std::ostream& operator << ( std::ostream& , const dependencies& );

}


#endif

