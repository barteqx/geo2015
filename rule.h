
// Code written by Hans de Nivelle, November 2005.


#ifndef RULE_INCLUDED
#define RULE_INCLUDED  1


#include <vector>
#include <iostream>


#include "predicate.h"
#include "variable.h"
#include "varatom.h"
#include "varset.h"
#include "geometric/index/simple.h"


// Rules can have the following forms:
// P1 /\ ... /\ Pn -> # y /\ Q.
// P1 /\ ... /\ Pn -> Q1 \/ ... \/ Qm,   m >= 0.



class rule
{

public: 
   std::vector < predicate > & predicates;
   std::vector < variable > & arguments;

   const static unsigned int type_disj    = 1;
   const static unsigned int type_exists  = 2;
   unsigned int ruletype;
      // One of two previous.

   unsigned int prem_pred1, prem_pred2;
   unsigned int prem_args1, prem_args2; 
      // Premisses. 

   unsigned int conc_pred1, conc_pred2;
   unsigned int conc_args1, conc_args2;
      // Conclusions.


private:
   rule( std::vector< predicate > & predicates_base,
         std::vector< variable > & arguments_base,
	 unsigned int ruletype,
         unsigned int prem_pred1, unsigned int prem_pred2,
	 unsigned int prem_args1, unsigned int prem_args2,

	 unsigned int conc_pred1, unsigned int conc_pred2,
	 unsigned int conc_args1, unsigned int conc_args2 )
   :
      predicates( predicates_base ),
      arguments( arguments_base ), 
      ruletype( ruletype ),
      prem_pred1( prem_pred1 ),
      prem_pred2( prem_pred2 ),
      prem_args1( prem_args1 ),
      prem_args2( prem_args2 ),
      conc_pred1( conc_pred1 ),
      conc_pred2( conc_pred2 ),
      conc_args1( conc_args1 ),
      conc_args2( conc_args2 )
   {
   }


public:

   // Construct empty rule.

   rule( std::vector< predicate > & predicates_base,
         std::vector< variable > & arguments_base )
      : ruletype( type_disj ),
        predicates( predicates_base ),
	arguments( arguments_base ), 
        prem_pred1( predicates. size( )),
	prem_pred2( predicates. size( )),
	prem_args1( arguments. size( )),
	prem_args2( arguments. size( )),
	conc_pred1( predicates. size( )),
	conc_pred2( predicates. size( )),
	conc_args1( arguments. size( )),
	conc_args2( arguments. size( ))
   {
   }


   // Construct copy in same vectors: 

   rule( const rule& r ) 
   : 
      predicates( r. predicates ),
      arguments( r. arguments ),
      ruletype( r. ruletype ),
      prem_pred1( r. prem_pred1 ),
      prem_pred2( r. prem_pred2 ),
      prem_args1( r. prem_args1 ),
      prem_args2( r. prem_args2 ),
      conc_pred1( r. conc_pred1 ),
      conc_pred2( r. conc_pred2 ),
      conc_args1( r. conc_args1 ),
      conc_args2( r. conc_args2 )
   {
   }

   // Copy rule within same vectors: 

   void operator = ( const rule& r ) 
   {
      ASSERT( & predicates == & r. predicates );
      ASSERT( & arguments == & r. arguments );

      ruletype = r. ruletype;
      prem_pred1 = r. prem_pred1;
      prem_pred2 = r. prem_pred2;
      prem_args1 = r. prem_args1;
      prem_args2 = r. prem_args2;
      conc_pred1 = r. conc_pred1;
      conc_pred2 = r. conc_pred2;
      conc_args1 = r. conc_args1;
      conc_args2 = r. conc_args2;
   }

   // Copies rule into different vectors:

   rule clone_into( std::vector< predicate > & p,
		    std::vector< variable > & a ) const; 


public: 
   struct prem_iterator 
   {
      std::vector< predicate > :: const_iterator p;
      std::vector< variable > :: const_iterator a; 


      explicit prem_iterator( std::vector< predicate > :: const_iterator p,
		              std::vector< variable > :: const_iterator a )
         : p(p), a(a)
      {
      }


      prem_iterator operator ++ ( int )
      {
	 prem_iterator res = *this;
	 a += p -> getarity( );
	 ++ p ;
	 return res;
      }

      prem_iterator operator ++ ( )
      {
	 a += p -> getarity( );
	 ++ p ;
	 return *this;
      }

      bool operator == ( prem_iterator a ) const 
      {
	 return ( this -> p ) == a. p;
      }

      bool operator != ( prem_iterator a ) const 
      {
	 return ( this -> p ) != a. p;
      }

      varatom operator * ( ) const
      {
         return varatom( *p, a, a + ( p -> getarity( )) );
      }

   };


   prem_iterator prems_begin( ) const
   {
       return prem_iterator( 
		       predicates. begin( ) + prem_pred1, 
		       arguments. begin( ) + prem_args1 );
   }

   prem_iterator prems_end( ) const
   {
      return prem_iterator( 
		      predicates. begin( ) + prem_pred2, 
		      arguments. begin( ) + prem_args2 );
   }


public: 

   // A conclusion iterator iterates through the conclusions of a 
   // disjunctive rule.

   struct conc_iterator 
   {
      std::vector< predicate > :: const_iterator p;
      std::vector< variable > :: const_iterator a; 


      explicit conc_iterator( std::vector< predicate > :: const_iterator p,
		              std::vector< variable > :: const_iterator a )
         : p(p), a(a)
      {
      }


      conc_iterator operator ++ ( int )
      {
	 conc_iterator res = *this;
	 a += p -> getarity( );
	 ++ p ;
	 return res;
      }

      conc_iterator operator ++ ( )
      {
	 a += p -> getarity( );
	 ++ p ;
	 return *this;
      }

      bool operator == ( conc_iterator c )
      {
	 return ( this -> p ) == c. p;
      }

      bool operator != ( conc_iterator c )
      {
	 return ( this -> p ) != c. p;
      }

      varatom operator * ( ) const
      {
         return varatom( *p, a, a + ( p -> getarity( )) );
      }

   };


   conc_iterator concs_begin( ) const
   {
      ASSERT( ruletype == type_disj );
      return conc_iterator( 
		       predicates. begin( ) + conc_pred1, 
		       arguments. begin( ) + conc_args1 );
   }

   conc_iterator concs_end( ) const
   {
      ASSERT( ruletype == type_disj );
      return conc_iterator( 
		      predicates. begin( ) + conc_pred2, 
		      arguments. begin( ) + conc_args2 );
   }


public: 

   bool addpremisse( varatom prem );
      // The premisse is only added when it is new. 
      // We return true if the premisse was really added.

   bool addconclusion( varatom conc ); 
      // The conclusion is only added when it is new. 
      // We return true if the conclusion was really added.

   void setexistential( variable v, varatom at );
      // Set the conclusion equal to # v /\ at.

   unsigned int nr_premisses( ) const;
      // Number of premisses.

   unsigned int nr_conclusions( ) const;
      // Number of conclusions in case of disjunctive rule. 

   unsigned int nr_variables( ) const;
      // Number of variables in the rule.

   unsigned int getweight( ) const;
      // Weight of the rule.

   bool iswellformed( ) const;
      // Check whether rule is well-formed. 

   bool isdisjunctive( ) const;
   bool isexistential( ) const;

   varatom getcreate( ) const;
      // Get the # V of an existential rule.

   varatom getbody( ) const;
      // Get the body of an existential rule.

   void print( std::ostream& ) const;
      // Ugly debugging print. 

   bool bases_disjoint( const rule& r ) const; 
   bool bases_disjoint( const std::vector< predicate > & predicates_base,
		        const std::vector< variable > & arguments_base ) const;


   varset getvariables( prem_iterator prem1, prem_iterator prem2 ) const;
      // Get the set of free variables in the range prem1 .. prem2.

   varset getvariablesconclusion( ) const;
      // Get the set of free variables that occur in the 
      // conclusion. 

   geometric::index::standardsimple< variable >
   make_simple_index( geometric::signature& sig ) const;
      // Put the premisses in a simple index.

}; 


std::ostream& operator << ( std::ostream& , const rule& );


#endif


