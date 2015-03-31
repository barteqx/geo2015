

// Code written by Hans de Nivelle, February 2006. 
// Added equality simplification in April 2007.


#include "simplification.h"
#include "swll/searchstate.h"
#include "swll/matchcounter.h"
#include "forwardreasoning.h"

#include "statistics/distribution.h"
#include "statistics/timer.h"
#include "statistics/reporter.h"

partial< varmatching >
simplification::findselfsubsumption( const rule& self_rule )
{

   static statistics::distribution times( 0.0, 0.1, 600.0 );
   static unsigned int counter = 0;
   static statistics::reporter r;
   static swll::matchcounter mc;

   if( r. mustreport( ))
   {
      std::cout << "Report from findselfsubsumption:\n";
      std::cout << "counter = " << counter << "\n";
      r. printduration( std::cout );
      std::cout << times << "\n";
      std::cout << mc << "\n\n";
      r. wasreported( );
   }

   statistics::timer t{ [] ( double d ) { times. insert(d); } };
   ++ counter;

   partial< varmatching > res;

   if( self_rule. isdisjunctive( ) &&
       self_rule. nr_conclusions( ) == 0 )
   {
      varset rulevars = self_rule. getvariables( self_rule. prems_begin( ),
		                                 self_rule. prems_end( ));

      for( auto p = rulevars. vars_cbegin( ); 
                p != rulevars. vars_cend( ); ++ p )
      {
         mc. attempted ++ ; 

	 swll::searchstate state;
	 bool b = state. setproblem( self_rule, self_rule, *p );
         
	 if(b)
         {
            mc. constructed ++ ; 
	    b = state. interreduce( );

            if(b)
            {
               mc. interreduced ++ ; 
	       b = state. searchsolution( );

	       if(b)
               {
                  mc. succeeded ++ ;
                  res = state. makevarmatching( );
         	  ASSERT( check_subsumption( res. contents( ), 
      		         		     self_rule, self_rule ));

	          return res; 
               }
	    }
         }
      }
   }
   return res; 
}


partial< varmatching >
simplification::findsubsumption( const rule& from_rule, 
		                 const rule& into_rule )
{

   static statistics::distribution times( 0.0, 0.1, 10.0 );
   static unsigned int counter = 0;
   static statistics::reporter r;
   static swll::matchcounter mc;

   if( r. mustreport( ))
   {
      std::cout << "Report from findsubsumption:\n";
      std::cout << "counter = " << counter << "\n";
      r. printduration( std::cout );  
      std::cout << times << "\n";
      std::cout << mc << "\n\n";
      r. wasreported( );
   }

   statistics::timer t{ [] ( double d ) { times. insert(d); } };
   ++ counter;


   partial< varmatching > res;

#if 0 
   std::cout << "\n"; 
   std::cout << "=================================\n"; 
   std::cout << "Entering findsubsumption\n";
   std::cout << "From: " << from_rule << "\n";
   std::cout << "Into: " << into_rule << "\n";
   std::cout << "\n"; 
#endif


   if( from_rule. isdisjunctive( ) &&
       from_rule. nr_conclusions( ) == 0 )
   {
      mc. attempted ++ ; 

      swll::searchstate state;
      bool b = state. setproblem( from_rule, into_rule, 
                                  partial< variable > ( ));

      if(b)
      {
         mc. constructed ++ ; 
         b = state. interreduce( );

         if(b)
         {
            mc. interreduced ++ ;
	    b = state. searchsolution( );

	    if(b) 
            {
               mc. succeeded ++ ;
	       res = state. makevarmatching( ); 

	       ASSERT( check_subsumption( res.contents( ), 
	         			  from_rule, into_rule )); 
            }
         }
      }
   }
   return res; 
}



partial< varmatching >
simplification::findsubsumptionwithoutdiseq( const rule& from_rule, 
		                             const rule& into_rule )
{

   static statistics::distribution times( 0.0, 0.1, 30.0 );
   static unsigned int counter = 0;
   static statistics::reporter r;
   static swll::matchcounter mc;

   if( r. mustreport( ))
   {
      std::cout << "Report from findsubsumptionwithoutdiseq:\n";
      std::cout << "counter = " << counter << "\n";
      r. printduration( std::cout );  
      std::cout << times << "\n";
      std::cout << mc << "\n\n";
      r. wasreported( );
   }

   statistics::timer t{ [] ( double d ) { times. insert(d); } };
   ++ counter;

   partial< varmatching > res;

#if 0 
   std::cout << "\n"; 
   std::cout << "==================================\n"; 
   std::cout << "Entering findsubsumptionwithoutdiseq\n";
   std::cout << "From: " << from_rule << "\n";
   std::cout << "Into: " << into_rule << "\n";
   std::cout << "\n"; 
#endif

   if( from_rule. isdisjunctive( ) && from_rule. nr_conclusions( ) == 0 )
   {
      mc. attempted ++ ; 

      swll::searchstate state;
      bool b = state. setproblemwithoutdiseq( from_rule, into_rule ); 

      if(b)
      {
         mc. constructed ++ ; 
         b = state. interreduce( );

         if(b)
         {
            mc. interreduced ++ ;
	    b = state. searchsolution( );

	    if(b) 
            {
               mc. succeeded ++ ;
	       res = state. makevarmatching( ); 
            }
         }
      }
   }
   return res; 
}


bool simplification::check_subsumption( const varmatching& subst, 
	                                const rule& from_rule,
	                                const rule& into_rule )
{
   if( ! from_rule. isdisjunctive( ))
      return false;
   if( ! from_rule. nr_conclusions( ) == 0 )
      return false;

   for( rule::prem_iterator p = from_rule. prems_begin( );
                            p != from_rule. prems_end( );
			    ++ p )
   {
      varatom at = subst. apply_on( *p );

#if 0 
      std::cout << "( " << *p << ", " << at << " ) " << "\n";
#endif

      if( at. p == predicate::create )
      {
         variable v = * ( at. a1 );

	 // We only demand that v is a variable of into_rule:

	 for( rule::prem_iterator q = into_rule. prems_begin( );
			          q != into_rule. prems_end( );
				  ++ q )
         {
            for( std::vector< variable > :: const_iterator b = (*q). a1;
                    b != (*q). a2;
		    ++ b )
             {
                if( *b == v ) 
                   goto found;
             }
         }
	 return false; 
      }

      if( at. p == predicate::diseq )
      { 

         // Disequalities can match in two orientations. 

         std::vector< variable > :: const_iterator a = at. a1;
	 variable v1 = * ( a ++ );
	 variable v2 = * a;

         for( rule::prem_iterator q = into_rule. prems_begin( );
                                  q != into_rule. prems_end( );
				  ++ q )
         {
            if( (*q). p == predicate::diseq )
            {
               std::vector< variable > :: const_iterator b = (*q). a1;
	       variable z1 = * ( b ++ );
	       variable z2 = * b;

	       if( v1 == z1 && v2 == z2 ) goto found;
	       if( v1 == z2 && v2 == z1 ) goto found;

	    }
         }
	 return false; 
      }

      if( at. p != predicate::create && at. p != predicate::diseq ) 
      {
         for( rule::prem_iterator q = into_rule. prems_begin( );
                                  q != into_rule. prems_end( );
			          ++ q )
         {
            if( *q == at )
               goto found;
         }
         return false;
      }

   found:
      ;
   }
   return true;
}



namespace
{

   // For each ( V != W ) in r1 for which neither of 
   // ( V m != W m ) or ( W m != V m ) occurs in r2,
   // we add V m, W m to the result. 

   std::list< variable > getunmatcheddisequalities( const rule& r1,
		                                    const rule& r2,
					            const varmatching& m )
   {
      std::list< variable > result;

      for( rule::prem_iterator 
              p = r1. prems_begin( );
	      p != r1. prems_end( );
	      ++ p )
      {
         if( (*p). p == predicate::diseq )
         {
            varatom at = m. apply_on( *p );
            std::vector< variable > :: const_iterator a = at. a1;
            variable v1 = * ( a ++ );
            variable v2 = * ( a ++ );

            for( rule::prem_iterator 
                    q = r2. prems_begin( );
	            q != r2. prems_end( );
	            ++ q )
            {
               if( (*q). p == predicate::diseq )
               {
                  std::vector< variable > :: const_iterator b = (*q). a1;
	          variable w1 = * ( b ++ );
	          variable w2 = * ( b ++ );

	          if( v1 == w1 && v2 == w2 ) goto matched;
	          if( v1 == w2 && v2 == w1 ) goto matched;
               }
            }

	    // We have found no match for v1 != v2.

	    result. push_back(v1);
	    result. push_back(v2);
         matched:
	    ;
         }
      }

      return result;
   }



   // Make the disequality cuts, and append them to result. 
   // As an example, 
   //    V1, V2, W1, W2   ( which denotes V1 != V2, W1 != W2 ),
   // together with 
   //    p( V1, V2, W1, W2 ) -> False, 
   // will result in 
   //    p( V, V, W1, W2 ) -> False,
   //    V1 != V2, p( V1, V2, W, W ) -> False


   void makediseqcuts( 
      std::list< variable > :: const_iterator p1,
      std::list< variable > :: const_iterator p2,
      const rule& r, 
      std::list< rule > & result, 
      std::vector< predicate > & predicates_base,
      std::vector< variable > & arguments_base )
   {

      if( p1 != p2 )
      {
         variable v1 = * ( p1 ++ );
	 variable v2 = * ( p1 ++ );
#if 1 
	 std::cout << "splitting on " << v1 << " != " << v2 << "\n";
#endif
	 std::vector< predicate > predicates_base1;
	 std::vector< predicate > predicates_base2;
	 std::vector< variable > arguments_base1;
	 std::vector< variable > arguments_base2; 

         rule r1( predicates_base1, arguments_base1 );
	 rule r2( predicates_base2, arguments_base2 );

	 forwardreasoning::disequality_split( r, v1, v2,
			      r1, predicates_base1, arguments_base1,
			      r2, predicates_base2, arguments_base2 );

	 // r1 contains a new inequality V1 != V2,
	 // r2 is obtained my merging V1 := V2.

         result. push_back( r2. clone_into( predicates_base, arguments_base )); 
	 makediseqcuts( p1, p2, r1, result, predicates_base, arguments_base );
	    // One can also continue with r itself here. This may be
	    // worth trying out, because it results in more general
	    // lemmas. On the other hand, less general lemmas have
	    // a bigger chance of being subsumed. 
      }

      // When there are no disequalities left, the resulting lemma 
      // is subsumed, so we do nothing. 
   }



   rule simplifylemma( const rule& lemma,
		       std::vector< predicate > & predicates_base,
		       std::vector< variable > & arguments_base ) 
   {

      std::cout << "Condensing lemma:  ";
      std::cout << lemma. nr_premisses( ) << ", ";
      std::cout << lemma. nr_variables( ) << "\n";


      std::vector< predicate > predicates_base_cond;
      std::vector< variable > arguments_base_cond;

#if 1
      rule cond = forwardreasoning::condensation( predicates_base_cond,
                                                  arguments_base_cond,
                                                  lemma );
#else
      rule cond = lemma. clone_into( predicates_base_cond, 
		                     arguments_base_cond );

#endif

      ASSERT( cond. iswellformed( ));

      return cond. clone_into( predicates_base, arguments_base );
   }



   // Return true if r1 can be used in the simplification of r2. 

   inline bool canbeused( const rule& r1, const rule& r2 )
   {
      if( r1. nr_premisses( ) > r2. nr_premisses( ))
         return false;
      //if( r1. nr_variables( ) > r2. nr_variables( ))
      //   return false;
      // if( r1. getweight( ) > r2. getweight( ))
      //   return false;

      return true; 
   }

}



void simplification::addtorulesystem( 
	  std::list< rule > & rules,
	  std::list< std::vector< rulesystem::binary_step >> & derivations,
	  std::vector< predicate > & predicates_base,
	  std::vector< variable > & arguments_base,
	  rulesystem& rs,
          unsigned int closure_counter )
{

restart:
   ASSERT( rules. size( ) == derivations. size( ));

   while( rules. size( ) != 0 )
   {

#if 0
      {
         // We print the state. The simplest way to do this is
	 // by creating a rulesystem.

	 std::cout << "#######################################\n";
	 std::cout << "state of addtorulesystem\n\n";

	 rulesystem rsprint;

	 std::list< rule > :: const_iterator p = rules. begin( );
	 std::list< std::vector< rulesystem::binary_step > > :: const_iterator
		 q = derivations. begin( );

	 while( p != rules. end( ))
         {
            rsprint. addrule( *p, *q, false );
	    ++ p;
	    ++ q;
         }
	 std::cout << rsprint << "\n";
	 std::cout << "##################################\n";
      }
#endif

      for( unsigned int nr = 0; nr < rs. nr_rules( ); ++ nr )
      {
         if( ! rs. rules [nr]. deleted &&
             rs. rules [nr]. r. isdisjunctive( ) &&
	     rs. rules [nr]. r. nr_conclusions( ) == 0 &&
             canbeused( rs. rules[nr]. r, rules. front( ) ))
         {
            partial< varmatching > m =
	                findsubsumptionwithoutdiseq( rs. rules [nr]. r, 
					             rules. front( ));

            if( m. isdefined( )) 
            {

#if 0
               std::cout << "constructed partial matching: ";
	       std::cout << rs. rules [nr]. r << "\n";
	       std::cout << rules. front( ) << "\n";
	       std::cout << "\n\n";
#endif

               // This means that we matched the non-disequality
	       // atoms of rs. rules[nr]. r into rules. front( ). 

               std::list< variable > diseqs =
                             getunmatcheddisequalities( rs. rules[nr]. r,
                                                        rules. front( ), 
							m. contents( ) );

               // diseqs contains the variables of the unmatched
               // disequalities, simply piled on a list.

               // We produce the cuts:

               std::list< rule > cuts;
               std::vector< predicate > predicates_base_cuts;
               std::vector< variable > arguments_base_cuts;

               makediseqcuts( diseqs. begin( ), diseqs. end( ),
                              rules. front( ), cuts,
                              predicates_base_cuts,
                              arguments_base_cuts );

               // We construct the list of derivations of the
               // cuts. They obtain their derivation from 
	       // derivations. front( ), 
               // plus the added cut, which was based on nr.

               std::list< std::vector< rulesystem::binary_step > >
                  derivations_cuts;
               for( unsigned int i = 0; i < cuts. size( ); ++ i )
               {
                  derivations_cuts. push_back( derivations. front( ));
                  derivations_cuts. back( ). push_back(
                        rulesystem::binary_step(
                              rulesystem::step_equality_simp, nr ));
               }

	       rules. pop_front( );
	       derivations. pop_front( );

	       while( cuts. size( ))
               {
                  rules. push_front( 
			     cuts. back( ). clone_into( predicates_base,
				                        arguments_base ));

		  derivations. push_front( derivations_cuts. back( ));

		  cuts. pop_back( );
		  derivations_cuts. pop_back( );
               }

	       // We have replaced rules. front( ) by its
	       // simplifications, and we restart.
	    
	       goto restart;
            }
         }
      }

      // When we reach this point, the rule cannot be further
      // simplified. The rule will be added, but we first check
      // for backward simplifications. 

      rules. front( ) = simplifylemma( rules. front( ),
		                       predicates_base,
				       arguments_base );

         // Sort the lemma, condense it, sort it again. It should
	 // be OK to reuse the bases. 

#if 0
      std::cout << "\n"; 
      std::cout << "Adding[";
      std::cout << rs. nr_rules( ) << "]: ";
      std::cout << rules. front( ) << "\n\n";
#endif

      for( unsigned int nr = 0; nr < rs. nr_rules( ); ++ nr )
      {
         if( ! rs. rules [nr]. deleted )
         {
            if( rs. rules [ nr ]. r. isdisjunctive( ) &&
                rs. rules [ nr ]. r. nr_conclusions( ) == 0 )
            {
               partial< varmatching > m = 
                  findsubsumptionwithoutdiseq( rules. front( ),
			                       rs. rules [ nr ]. r );
	       if( m. isdefined( ))
                     //  rules. front( ). nr_premisses( ) < 
		     //  rs. rules [ nr ]. r. nr_premisses( )) 
               {
                  std::list< variable > diseqs =
			      getunmatcheddisequalities( rules. front( ),
					                 rs. rules [ nr ]. r, 
							 m. contents( )); 

		  if( diseqs. size( ) != 0 ) 
                  {

                     // We follow the lazy man's solution. We simply
	             // append the rule to rules, so that it will be 
	             // processed at a later point. 

#if 0
                     std::cout << "Backward simplifying[";
	             std::cout << nr << "]: ";
	             std::cout << rs. rules [ nr ]. r << "\n\n";
#endif

                     rules. push_back( 
			   rs. rules [ nr ]. r. clone_into( predicates_base,
			                                    arguments_base ));

	             rs. rules [ nr ]. deleted = true;

	             derivations. push_back( 
		        std::vector< rulesystem::binary_step > ( ));
                     derivations. back( ). push_back(
                        rulesystem::binary_step(
                           rulesystem::step_startwith, nr ));
                     
                  }
		  else
                  {
                     // Otherwise, the rule is simply backward subsumed.

#if 0
	             std::cout << "Deleting[";
	             std::cout << nr;
	             std::cout << "]: ";
	             std::cout << rs. rules [ nr ]. r << "\n\n";
#endif
		     rs. rules [ nr ]. deleted = true;
                  }
               }
            }
	    else
            {
               partial< varmatching > m =
                  findsubsumption( rules. front( ), rs. rules [ nr ]. r );

	       if( m. isdefined( ))
               {
#if 0
                  std::cout << "Deleting[";
		  std::cout << nr;
		  std::cout << "]: ";
                  std::cout << rs. rules [ nr ]. r << "\n\n";
#endif
		  rs. rules [ nr ]. deleted = true;
               }
            }
         }
      }

      rs. addrule( rules. front( ), derivations. front( ), false );
      rs. updatestatistics( derivations. front( ), closure_counter );

      rules. pop_front( );
      derivations. pop_front( );

      std::cout << "Kept lemma nr " << rs. nr_rules( ) << "\n\n"; 
   }
}




void simplification::test_swll_subsumption( const rulesystem& rs )
{
   std::cout << "Testing subsumption: \n";

   ASSERT( rs. nr_rules( ) == 2 );
   
   rule from = rs. rules [0]. r;
   rule into = rs. rules [1]. r;

   std::cout << "from: " << from << "\n";
   std::cout << "into: " << into << "\n";

   ASSERT( from. isdisjunctive( ));
   ASSERT( from. nr_conclusions( ) == 0 );

   swll::searchstate state;

   varmatching vm;

   bool b = false;
   for( unsigned int i = 0; i < 1; ++ i )
      b = state. setproblemwithoutdiseq( from, into );

   if(b)
   {
      state. interreduce( );

      std::cout << state << "\n";

      b = state. searchsolution( );

      if(b)
      {
	 varmatching res = state. makevarmatching( ); 
         std::cout << "found solution: ";
	 std::cout << res << "\n";
	 ASSERT( check_subsumption( res, from, into ));
      }
      else
         std::cout << "found no solution\n";
   }
   else
      std::cout << "failed in setproblem\n";
}

  
void simplification::testsimplification( rulesystem& rs )
{
   ASSERT( rs. nr_rules( ) >= 1 );

   variable v0;
   variable v1 = v0; v1 ++ ;
   variable v2 = v1; v2 ++ ;
   variable v3 = v2; v3 ++ ;
   variable v4 = v3; v4 ++ ;
   variable v5 = v4; v5 ++ ;
   variable v6 = v5; v6 ++ ;

   varmatching m;
   m. assign( v0, v1 );
   m. assign( v1, v0 );
   m. assign( v2, v3 );
   m. assign( v3, v2 );
   m. assign( v4, v4 );

   std::list< rule > rules;
   std::list< std::vector< rulesystem::binary_step > > derivations;

   std::vector< predicate > predicates_base;
   std::vector< variable > arguments_base;

   unsigned int last = rs. nr_rules( ) - 1;
   rules. push_back( rs. rules [ last ]. r. clone_into(
			      predicates_base, arguments_base ));
   rs. rules [ last ]. deleted = true; 

   derivations. push_back( std::vector< rulesystem::binary_step > ( ));

   derivations. back( ). push_back( 
		   rulesystem::binary_step( 
			     rulesystem::step_equality_simp, 0 ));

   addtorulesystem( rules, derivations, 
		    predicates_base, arguments_base, rs, 9999 );

   std::cout << rs << "\n";

}



