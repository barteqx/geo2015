
// Code written by Hans de Nivelle, November 2005. 
// Modified in February 2007,
//    - modified the recording of proofs, so that simplification
//      rules are recorded as well. 
//    - I added locks in February 2007, but removed them again in April.
//      The problem is that I don't know how to maintain the locks in
//      case of redundancy simplification. Also, first experiments
//      were somewhat unsatisfactory. I will later experiment with
//      restarts instead. They are easier to implement. 
//    - improved the statistics about the use of the rules. 


#ifndef RULESYSTEM_INCLUDED
#define RULESYSTEM_INCLUDED   1


#include <vector>
#include <iostream>


#include "varatom.h"
#include "rule.h"



class rulesystem
{


public: 
   enum type_of_step 
   {
      step_startwith,          // Specifies first argument of binary
                               // rule. 

      step_disjunctive_res,    
      step_existential_res,

      step_equality_simp,
      step_existential_simp,
      step_disjunctive_simp
   };


   struct binary_step 
   {
      type_of_step derivedwith;
      unsigned int otherparent;

      binary_step( type_of_step derivedwith,
		   unsigned int otherparent )
         : derivedwith( derivedwith ),
           otherparent( otherparent )
      {
      }
   }; 


private: 
   std::vector< predicate > predicates_base; 
   std::vector< variable > arguments_base; 

   std::vector< binary_step > derivation; 


public: 
   struct rule_in_system 
   {
      rule r;

      unsigned int deriv1; 
      unsigned int deriv2; 
         // These are references into derivation.

      bool deleted;
         // True if the rule is deleted. 
      bool inductive;
         // True if the rule is inductive. Inductive rules can be used 
	 // in simplifications, but they should never be applicable. 

      unsigned int nr_forward;
      unsigned int nr_simpl; 
         // How many times the rule has been applied.

      unsigned int last_forward;
      unsigned int last_simpl; 
         // The last time the rule was applied. 

      mutable bool inderivation;
         // True if the flag occurs in a derivation.
	 // This flag is used only inside printderivation.

      rule_in_system( const rule& r, 
		      unsigned int deriv1, unsigned int deriv2,
		      bool inductive ) 
         : r(r), 
	   deriv1( deriv1 ), deriv2( deriv2 ),
	   deleted( false ), 
	   inductive( inductive ), 
	   nr_forward(0), nr_simpl(0), 
	   last_forward(0), last_simpl(0), 
	   inderivation( false ) 
      {
      }

   };

   std::vector< rule_in_system > rules;

public:

   unsigned int addrule( rule r, const std::vector< binary_step > & deriv,
		         bool inductive );
      // Add a rule to the system, and return its index.  
      // Inductive rules can be used in simplifications but should 
      // never be applicable. 

   void updatestatistics( const std::vector< binary_step > & deriv,
                  unsigned int closure_counter );
      // Update the use statistics of the rules that are mentioned in deriv.

   unsigned int nr_rules( ) const
   {
      return rules. size( );
   }


   // A derivation_iterator iterates through the derivation of a rule. 

   struct derivation_iterator
   {
      std::vector< binary_step > :: const_iterator p;

      explicit derivation_iterator( 
		      std::vector< binary_step > :: const_iterator p ) 
         : p(p)
      {
      }


      derivation_iterator operator ++ ( int ) 
      {
         return derivation_iterator( p ++  );
      }

      derivation_iterator operator ++ ( ) 
      {
         return derivation_iterator( ++ p );
      }

      bool operator == ( derivation_iterator other ) const 
      {
         return p == other. p;
      }

      bool operator != ( derivation_iterator other ) const 
      {
         return p != other. p;
      }

      const binary_step operator * ( ) const
      {
         return *p; 
      }

      const binary_step* operator -> ( ) const
      {
         return & (*p);
      }

   }; 

   derivation_iterator derivation_begin( unsigned int rulenumber ) const 
   {
      ASSERT( rulenumber < rules. size( )); 
      return derivation_iterator( 
		  derivation. begin( ) + rules [ rulenumber ]. deriv1 ); 
   }

   derivation_iterator derivation_end( unsigned int rulenumber ) const 
   {
      ASSERT( rulenumber < rules. size( )); 
      return derivation_iterator( 
		  derivation. begin( ) + rules [ rulenumber ]. deriv2 ); 
   }

   void print( std::ostream& stream,
               unsigned int nr_forward, unsigned int nr_simpl,
	       bool inderivation ) const;
      // Print those rules r for which r. nr_forward >= nr_forward,
      // r. nr_simpl >= nr_simpl,   r. inderivation >= inderivation. 

   void printderivation( std::ostream& stream, 
		         unsigned int rulenumber ) const;
      // Print the derivation of the rule with rulenumber.

   bool canbedeleted( unsigned int nr ) const;
      // True if the rule can be in principle deleted (by deleteheaviest)
      // The rule should non-initial, a lemma, and not already deleted.

   int comparerules( unsigned int nr1, unsigned int nr2 ); 
      // Return -1 if nr1 is a better rule than nr2. 
      //         0 if they are the same.
      //         1 is nr1 is a worse rule than nr2. 

   void deleteheaviest( unsigned int closure_counter );
      // Delete the worst 33% of the rules. 
      // Until now, no deletion strategy has been successful.

   friend std::ostream& operator << ( std::ostream& , const rulesystem& ); 
};


std::ostream& operator << ( std::ostream& , const rulesystem & ); 


#endif


