

// Code written by Hans de Nivelle, November 2005.

#include "forwardreasoning.h"
#include "model.h"
#include "varsubst.h"
#include "varmatching.h"
#include "simplification.h"



bool forwardreasoning::ext_res_possible( 
		                 const false_instance& existential,
	                         const false_instance& lemma,
	                         unsigned int branch_border ) 
{

   if( lemma. sup_prem_match( ) <= branch_border )
      return false; 

   variable existsvar = * ( existential. r. getcreate( ). a1 ); 
      // The quantified variable.

   static varset vars; 
   vars. clear( ); 
      // vars will contain the variables of lemma that are matched
      // against existvar.
      // First step is to collect them: 

      // p goes through the premisses of lemma,
      // nr goes in parallel through the matching, so
      // that *nr reveals where *p was matched.

   std::vector< unsigned int > :: const_iterator 
      nr = lemma. prem_match. begin( );
   for( rule::prem_iterator p = lemma. r. prems_begin( );
           p != lemma. r. prems_end( );
	   ++ p )
   {
      if( *nr >= branch_border )
      {
         // This means that this premisse was matched into
         // one of the atoms that depends on the existential rule.
	 // It is either the body atom, a create #V atom, or a 
	 // a disequality V != W. 
	 // Things are complicated by the fact that the #V atom
	 // need not exist. It exists only in the case that
	 // we are checking a new element.

         if( (*p). p == predicate::create )
         {
            variable v = * ( (*p). a1 );
	    vars. insert(v);
	    goto next; 
         }

         if( existential. r. getbody( ). p == (*p). p )
         {
            // We go in parallel through the body
	    // of existential and this atom,
	    // so that we can read of which variables
	    // are matched against existsvar.

	    std::vector< variable > :: const_iterator 
		    b = existential. r. getbody( ). a1; 

	    for( std::vector< variable > :: const_iterator 
		    a = (*p). a1;
                    a != (*p). a2;
		    ++ a )
            {
               if( *b == existsvar )
                    vars. insert( *a );
	       ++ b;
            }
	    goto next;
         }

         if( (*p). p == predicate::diseq )
            goto next;

	 return false;
      }

   next:
      ++ nr; 
   }
   ASSERT( nr == lemma. prem_match. end( )); 

   if( vars. size( ) == 0 )
      return false;
         // This happens when no premisse depends on the quantification.
	 // In that case, there is no need to make an inference
	 // and we reject.

   // Now we have in varlist the list of all variables
   // that are matched against existsvar. 

   // These variables may occur only in the following forms: 
   // 1. In create atoms
   // 2. In disequalities between a variable in varlist and
   //    a variable not in varlist.
   // 3. In atoms that match against existential. r. getbody( ),
   //    at a position that contains existsvar.

   // We go once more time through the premisses of lemma, 
   // in order to check all the occurrences of variables in varlist.

   nr = lemma. prem_match. begin( );

   for( rule::prem_iterator p = lemma. r. prems_begin( );
        p != lemma. r. prems_end( );
	++ p )
   {
      if( *nr >= branch_border && (*p). p == existential. r. getbody( ). p )
      {
         // We are checking case 3.

         std::vector< variable > :: const_iterator 
               b = existential. r. getbody( ). a1;
	 for( std::vector< variable > :: const_iterator 
		 a = (*p). a1;
                 a != (*p). a2;
	         ++ a ) 
         {
            if( *b != existsvar && vars. contains( *a ))
               return false; 
	    ++ b; 
         }
      }
      else
      {
         // We count the occurrences of variables in varlist
	 // in this premisse.

         unsigned int occurrences = 0;
	 for( std::vector< variable > :: const_iterator
                 a = (*p). a1;
                 a != (*p). a2;
		 ++ a )
         {
            if( vars. contains( *a ))
               occurrences ++ ;
         }
            
         if( (*p). p == predicate::diseq && occurrences == 2 )
         {
            // Actually, this should not happen because lemma
	    // is a closing lemma, but checking is better than
	    // trusting. 

            return false;
         }

	 if( (*p). p != predicate::diseq && 
	     (*p). p != predicate::create )
         {
            if( occurrences != 0 )
               return false;
         }

	 // There are no constraints on create atoms. 
      }

      ++ nr;
   }
   ASSERT( nr == lemma. prem_match. end( ));
   return true; 
}



false_instance forwardreasoning::existential_resolvent( 
            std::vector< predicate > & predicates_base,
 	    std::vector< variable > & arguments_base, 
            const false_instance& existential,
            const false_instance& lemma,
	    const model& m, 
	    unsigned int branch_border )
{
   false_instance result( rule( predicates_base, arguments_base ));

   ASSERT( result. r. bases_disjoint( existential. r ));
   ASSERT( result. r. bases_disjoint( lemma. r ));

   variable existsvar = * ( existential. r. getcreate( ). a1 );
      // The quantified variable.

   static varsubst s;
   s. clear( );

   label lab_exists = label(1); 
   label lab_lemma = label(2); 

   // We begin by constructing the most general unifier:
   // p goes through the premisses of lemma,
   // nr goes through the numbers of the atoms into which
   // the premisses were matched.

   std::vector< unsigned int > :: const_iterator 
      nr = lemma. prem_match. begin( );
   for( rule::prem_iterator 
	   p = lemma. r. prems_begin( );
           p != lemma. r. prems_end( );
	   ++ p, ++ nr )
   {
      if( *nr >= branch_border )
      {
         if( (*p). p == predicate::create ) 
         {
	    bool b = s. unify( lab_exists, existential. r. getcreate( ), 
			       lab_lemma, *p );
	    ASSERT(b);
         } 
         else
         {
            if( (*p). p != predicate::diseq ) 
            {
               bool b = s. unify( lab_exists, existential. r. getbody( ),
				  lab_lemma, *p );
	       ASSERT(b);
            }
	 }
      }
   }

   // We now have the unifier in s.
   // We copy the premisses from existential to result. 

   static normalizer n;
   n. clear( );

   nr = existential. prem_match. begin( ); 
   for( rule::prem_iterator 
	   p = existential. r. prems_begin( );
           p != existential. r. prems_end( );
           ++ p, ++ nr )
   {
      if( result. r. addpremisse( s. apply_on( n, lab_exists, *p )))
      {
         result. prem_match. push_back(*nr); 
      }

         // addpremisse returns true if the result is really added
	 // (did not occur already).
	 // In that case, we add the nr of the matching
	 // to result. prem_match. 
   }

   // We copy the side atoms from lemma to result.
   // An atom is a side atom if it does not contain 
   // a variable that is unified with existsvar. 

   nr = lemma. prem_match. begin( ); 
   for( rule::prem_iterator 
	   p = lemma. r. prems_begin( );
           p != lemma. r. prems_end( );
	   ++ p, ++ nr )
   {
      for( std::vector< variable > :: const_iterator 
	      a = (*p). a1;
              a != (*p). a2;
	      ++ a )
      {
         if( s. results_equal( lab_lemma, *a, 
			       lab_exists, existsvar ))
         {
            goto notside;
         }
      }

      if( result. r. addpremisse( s. apply_on( n, lab_lemma, *p )))
      {
         result. prem_match. push_back(*nr); 
      }

   notside:
      ;
   }

   // Final step: For each disequality involving a variable
   // that is matched against existsvar, we generate
   // a conclusion:

   for( rule::prem_iterator
           p = lemma. r. prems_begin( ); 
	   p != lemma. r. prems_end( ); 
	   ++ p )
   {
      if( (*p). p == predicate::diseq )
      {
         std::vector< variable > :: const_iterator a = (*p). a1; 
         variable v1 = * a ++ ;
	 variable v2 = * a; 

	 if( s. results_equal( lab_lemma, v1, lab_exists, existsvar ))
         {
	    result. r. addconclusion( 
               s. apply_and_replace( n, 
		                     lab_exists, existential. r. getbody( ),
		                     lab_lemma, v1, lab_lemma, v2 ));
	    goto next;
         }

	 if( s. results_equal( lab_lemma, v2, lab_exists, existsvar ))
         {
            result. r. addconclusion(
               s. apply_and_replace( n, 
		                     lab_exists, existential. r. getbody( ),
		                     lab_lemma, v2, lab_lemma, v1 ));
         }
      next:
         ; 
      }
   }

   bool b = result. computesubst(m);
   ASSERT(b); 
   return result; 
}


false_instance forwardreasoning::disjunction_resolvent(
        std::vector< predicate > & predicates_base,
	std::vector< variable > & arguments_base,
	const false_instance& disjunction,
	unsigned int alternative,
	const false_instance& lemma,
	const model& m, 
	unsigned int branch_border )
{

#if 0
   std::cout << "resolving:\n";
   std::cout << disjunction << "\n";
   std::cout << lemma << "\n";
#endif 

   false_instance result( rule( predicates_base, arguments_base ));

   ASSERT( result. r. bases_disjoint( disjunction. r ));
   ASSERT( result. r. bases_disjoint( lemma. r ));

   ASSERT( lemma. sup_prem_match( ) > branch_border );

   // Produce iterator of atom that is resolved away
   // in the conclusion: 

   ASSERT( alternative < disjunction. r. nr_conclusions( ));

   rule::conc_iterator upon = disjunction. r. concs_begin( );
   for( unsigned int i = 0; i < alternative; ++ i )
   {
      upon ++ ;
   }

   static varsubst s;
   s. clear( );

   static normalizer n;
   n. clear( );

   label lab_disj = label(1); 
   label lab_lemma = label(2); 


   // Construct the unifier by unifying the premisses that were
   // matched into *upon. 

   std::vector< unsigned int > :: const_iterator 
      nr = lemma. prem_match. begin( );
   for( rule::prem_iterator p = lemma. r. prems_begin( );
        p  != lemma. r. prems_end( );
	++ p )
   {
      if( *nr >= branch_border )
      {
         bool b = s. unify( lab_disj, *upon, lab_lemma, *p );
	 ASSERT(b);
      }
      ++ nr;
   }
   ASSERT( nr == lemma. prem_match. end( )); 

   // Copy side literals from premisse of disjunction:

   nr = disjunction. prem_match. begin( ); 
   for( rule::prem_iterator p = disjunction. r. prems_begin( );
        p != disjunction. r. prems_end( );
	++ p, ++ nr )
   {
      if( result. r. addpremisse( s. apply_on( n, lab_disj, *p )))
         result. prem_match. push_back(*nr); 
   }

   // Copy side literals from premisse of lemma:

   nr = lemma. prem_match. begin( );
   for( rule::prem_iterator p = lemma. r. prems_begin( );
        p != lemma. r. prems_end( );
	++ p, ++ nr )
   {
      if( *nr < branch_border )
      {
         if( result. r. addpremisse( s. apply_on( n, lab_lemma, *p )))
            result. prem_match. push_back(*nr); 
      }
   }

   // Copy side literals from conclusion of disjunction:

   for( rule::conc_iterator q = disjunction. r. concs_begin( );
        q != disjunction. r. concs_end( );
	++ q )
   {
      if( q != upon )
      {
         result. r. addconclusion( s. apply_on( n, lab_disj, *q ));
      }
   }

   bool b = result. computesubst(m);
   ASSERT(b); 
   return result;
}


namespace
{

   // True if *at has form # V and V occurs in another
   // non-disequality atom in lemma. 

   bool isredundantcreate( rule::prem_iterator at,
		           const rule& lemma )
   {
      if( (*at). p == predicate::create )
      {
         variable v = * ( (*at). a1 );

	 for( rule::prem_iterator p = lemma. prems_begin( );
              p != lemma. prems_end( );
	      ++ p )
         {
            if( p != at && (*p). p != predicate::diseq )
            {
               for( std::vector< variable > :: const_iterator a = (*p). a1;
                    a != (*p). a2;
		    ++ a )
               {
                  if( *a == v )
                     return true;   // occurrence in non-disequality. 
               }
            }
         }
      }

      return false;
   }



   // True if all variables of va are contained in v. 

   bool varscontained( varatom va, const varset& v )
   {
     
      for( std::vector< variable > :: const_iterator 
              a = va. a1;
	      a != va. a2;
	      ++ a )
      {
         if( ! v. contains( *a ))
            return false;
      }
      return true;
   }


   // A premisse can be either deleted, copied, or available. 

   enum state_prem 
   {
      state_copied,
      state_deleted,
      state_available 
   }; 



   // Compute the set of variables 
   // ( ( v is in a premisse of r which is not p and
   //     v is available ) or 
   // ( v is in conclusion ) ) and ( v is in relevant )
 

   varset computevarsavailable( 
		const rule& r, 
		const std::vector< state_prem > & available,
		const varset& relevant, 
                rule::prem_iterator p )
   {
      varset res = r. getvariablesconclusion( ) & relevant; 

      std::vector< state_prem > :: const_iterator f = available. begin( );
      for( rule::prem_iterator q = r. prems_begin( );
		               q != r. prems_end( );
			       ++ q, ++ f )
      {
         if( *f == state_available && p != q )
	 {
            varatom va = *q;
	    for( std::vector< variable > :: const_iterator 
		     a = va. a1;
		     a != va. a2;
		     ++ a )
            {
               if( relevant. contains( *a )) 
                  res. insert( *a );
            }
         }
      }

      return res; 
   }


   // Compute all variable in va. 


   varset varsinatom( varatom va )
   {
      varset res;

      for( std::vector< variable > :: const_iterator 
              a = va. a1;
	      a != va. a2;
	      ++ a )
      {
         res. insert( *a );
      }

      return res;
   }


   // Compute the effect on the intersection when p is 
   // moved from available to copied. 

   varset effect_intersection( const rule& r,
			const std::vector< state_prem > & available,
			const varset& old_intersection,
			rule::prem_iterator p ) 

   {
      varset res = old_intersection; 

      varset inatom = varsinatom( *p );

      varset inavailable = 
	       computevarsavailable( r, available, inatom, p );

      for( auto p = inatom. vars_cbegin( ); p != inatom. vars_cend( ); ++ p )
      {
	 if( inavailable. contains( *p ))
            res. insert( *p );
	 else
            res. remove( *p );
      }

      return res;
   }


   
   unsigned int sum_of_vector( const std::vector< unsigned int > & vect )
   {
      unsigned int sum = 0;
      for( std::vector< unsigned int > :: const_iterator 
              p = vect. begin( );
	      p != vect. end( );
	      ++ p )
      {
         sum += *p;
      }
      return sum;
   }


   // sharing [i] is after choice [i].

   void bestnext( 
      std::vector< rule::prem_iterator > & best_choice,
      std::vector< unsigned int > & best_sharing,
      std::vector< rule::prem_iterator > & choice,
      std::vector< unsigned int > & sharing,
      std::vector< state_prem > & available, 
      const varset& current_intersection, 
      const rule& r, 
      unsigned int lookahead )
   {

#if 0
      std::cout << "Entering bestnext:\n";
      for( std::vector< rule::prem_iterator > :: const_iterator 
              p = choice. begin( );
	      p != choice. end( );
	      ++ p )
         std::cout << *(*p) << "  ";
      std::cout << "\n";

      for( std::vector< unsigned int > :: const_iterator 
              p = sharing. begin( );
	      p != sharing. end( );
	      ++ p )
         std::cout << *p << "  ";
      std::cout << "\n";

      std::cout << "intersection: " << current_intersection << "\n";
#endif 

      if( lookahead == 0 )
      {
	 if( best_choice. size( ) == 0 ||
	     sum_of_vector( sharing ) < sum_of_vector( best_sharing ))
         {
            best_choice = choice;
	    best_sharing = sharing;
         }
      }
      else
      {
  
         if( best_choice. size( ) != 0 &&
             sum_of_vector( sharing ) >= sum_of_vector( best_sharing ))
         {
            return;
         }

         std::vector< state_prem > :: iterator f = available. begin( ); 
         for( rule::prem_iterator p = r. prems_begin( ); 
                                  p != r. prems_end( );
				  ++ p, ++ f )
         {
            if( *f == state_available && 
	        (*p). p == predicate::diseq &&
                varscontained( *p, current_intersection ) )
            {

	       varset next_intersection = effect_intersection(
			       r, available,
			       current_intersection, p );

               *f = state_copied;
	       choice. push_back( p );
	       sharing. push_back( next_intersection. size( )); 

               bestnext( best_choice, best_sharing,
                         choice, sharing, available, 
			 next_intersection, r, 
			 lookahead - 1 );

	       sharing. pop_back( );
	       choice. pop_back( );
	       *f = state_available; 

               return;
	    }
	 }

         // At this point, we try all non-equality premisses of r.

         f = available. begin( );
	 for( rule::prem_iterator p = r. prems_begin( );
			          p != r. prems_end( );
				  ++ p, ++ f ) 
         {
            if( *f == state_available && (*p).p != predicate::diseq )
            {
               varset next_intersection = effect_intersection(
			       r, available, 
			       current_intersection, p ); 

               *f = state_copied; 
	       choice. push_back( p );
	       sharing. push_back( next_intersection. size( )); 

	       bestnext( best_choice, best_sharing,
                         choice, sharing, available, 
			 next_intersection, r,
			 lookahead - 1 );

	       sharing. pop_back( );
	       choice. pop_back( ); 
	       *f = state_available; 
            }
	 }
      }
   }
}




rule forwardreasoning::sortlemma(
         std::vector< predicate > & predicates_base,
         std::vector< variable > & arguments_base,
         const rule& lemma, 
	 unsigned int lookahead )
{

   rule result( rule( predicates_base, arguments_base ));

   ASSERT( result. bases_disjoint( lemma ));
   ASSERT( lemma. isdisjunctive( ) ); 
   ASSERT( lemma. nr_conclusions( ) == 0 );

   ASSERT( lookahead != 0 && lookahead < 10 );

#if 0
   std::cout << "Entering sortlemma[" << lookahead << "] " << lemma << "\n\n";
#endif

   std::vector< state_prem > available;

   unsigned int nr_remaining = 0;
   for( rule::prem_iterator p = lemma. prems_begin( );
        p != lemma. prems_end( );
	++ p )
   {
      if( ! isredundantcreate( p, lemma ))
      {
         available. push_back( state_available );
	 ++ nr_remaining; 
      }
      else
         available. push_back( state_deleted ); 
   }

   varset current_intersection;

   while( nr_remaining != 0 ) 
   {

      if( lookahead > nr_remaining )
         lookahead = nr_remaining;

      std::vector< rule::prem_iterator > best_choice;
      std::vector< unsigned int > best_sharing;
      std::vector< rule::prem_iterator > choice;
      std::vector< unsigned int > sharing;

      bestnext( best_choice, best_sharing,
                choice, sharing,
		available, 
		current_intersection, lemma, 
		lookahead );

      ASSERT( choice. size( ) == 0 );
      ASSERT( sharing. size( ) == 0 ); 
      ASSERT( best_choice. size( ) != 0 );
      ASSERT( best_sharing. size( ) != 0 );

      std::vector< state_prem > :: iterator f = available. begin( ); 

      for( rule::prem_iterator 
              p = lemma. prems_begin( );
              p != lemma. prems_end( );
	      ++ p, ++ f )
      {
         if( p == best_choice. front( ))
         {
            result. addpremisse( *p );
	    ASSERT( *f == state_available ); 
            *f = state_copied; 

            current_intersection = effect_intersection( 
			    lemma,
			    available, 
			    current_intersection,
			    p ); 
         }
      }

      -- nr_remaining;
   }

#if 0
   std::cout << "Leaving sortlemma\n\n";
#endif

   return result;
}






rule forwardreasoning::condensation(
	 std::vector< predicate > & predicates_base,
	 std::vector< variable > & arguments_base,
	 const rule& orig )
{
   ASSERT( orig. bases_disjoint( predicates_base, arguments_base )); 
   ASSERT( orig. isdisjunctive( ));
   ASSERT( orig. nr_conclusions( ) == 0 );

#if 0
   std::cout << "Entering condensation: \n";
   std::cout << orig << "\n";
#endif

   partial< varmatching > res = simplification::findselfsubsumption( orig ); 

   if( res. isdefined( ))
   {

      std::vector< predicate > predicates_step; 
      std::vector< variable > arguments_step; 

      rule step( predicates_step, arguments_step );

      for( rule::prem_iterator
              p = orig. prems_begin( );
	      p != orig. prems_end( ); 
	      ++ p ) 
      {
         varatom va = res. contents( ). apply_on( *p );
         step. addpremisse( va ); 
      }

      std::cout << "TRUE CONDENSATION\n"; 
      return condensation( predicates_base, arguments_base, step ); 
         // We iterate, because there may exist further condensations. 

   }

   // We found no condensation. orig has to be copied into result.

   return orig. clone_into( predicates_base, arguments_base ); 
}


void forwardreasoning::existential_split( 
		          const rule& lemma,
		          varatom va,
		          rule& r1, 
		          std::vector< predicate > & predicates_base1,
                          std::vector< variable > & arguments_base1,
		          rule& r2,
		          std::vector< predicate > & predicates_base2,
		          std::vector< variable > & arguments_base2 )
{

   ASSERT( lemma. isdisjunctive( ) && lemma. nr_conclusions( ) == 0 );

   ASSERT( va. p != predicate::diseq && va. p != predicate::create );
      // The atom that we are splitting on, should be not a create,
      // and not a disequality atom.
 
   // Actually, we don't need the variables. We only check that
   // va contains exactly one atom that does not occur in lemma.

   varset vs; 
   for( rule::prem_iterator 
           p = lemma. prems_begin( );
	   p != lemma. prems_end( );
	   ++ p )
   {
      for( std::vector< variable > :: const_iterator
              q = (*p). a1;
	      q != (*p). a2;
	      ++ q )
      {
         vs. insert( *q );
      }
   }

   varset fresh;
   for( std::vector< variable > :: const_iterator 
           q = va. a1;
	   q != va. a2;
	   ++ q )
   {
      if( ! vs. contains( *q ))
         fresh. insert( *q );
   }

   ASSERT( fresh. size( ) == 1 );

   ASSERT( r1. predicates == predicates_base1 &&
           r1. arguments == arguments_base1 );
   ASSERT( lemma. bases_disjoint(r1));

   for( rule::prem_iterator 
           p = lemma. prems_begin( );
	   p != lemma. prems_end( );
	   ++ p )
   {
      r1. addpremisse( *p );
   }
   r1. setexistential( * ( fresh. vars_cbegin( )), va );

   ASSERT( r2. predicates == predicates_base2 &&
           r2. arguments == arguments_base2 );
   ASSERT( lemma. bases_disjoint(r2));

   for( rule::prem_iterator
           p = lemma. prems_begin( );
	   p != lemma. prems_end( );
	   ++ p )
   {
      r2. addpremisse( *p );
   }
   r2. addpremisse(va);
}



void forwardreasoning::disjunctive_split( 
		            const rule& disj, 
		            varatom va,
		            rule& r1,  
		            std::vector< predicate > & predicates_base1,
                            std::vector< variable > & arguments_base1,
		            rule& r2,
		            std::vector< predicate > & predicates_base2,
		            std::vector< variable > & arguments_base2 )
{

   ASSERT( disj. isdisjunctive( ));

   ASSERT( va. p != predicate::diseq && va. p != predicate::create );
      // The atom that we are splitting on, should be not a create,
      // and not a disequality atom.
 
   // We are going to collect the variables that occur in va, but 
   // not in the premisses of disj. In case there are such variables,
   // we need to insert create atoms for them in r1. 

   varset vs; 
   for( std::vector< variable > :: const_iterator 
           q = va. a1;
	   q != va. a2;
	   ++ q )
   {
      vs. insert( *q );
   }

   for( rule::prem_iterator 
           p = disj. prems_begin( );
	   p != disj. prems_end( );
	   ++ p )
   {
      for( std::vector< variable > :: const_iterator
              q = (*p). a1;
	      q != (*p). a2;
	      ++ q )
      {
         vs. remove( *q );
      }
   }

   ASSERT( r1. predicates == predicates_base1 &&
           r1. arguments == arguments_base1 );
   ASSERT( disj. bases_disjoint(r1));

   for( auto p = disj. prems_begin( );
	   p != disj. prems_end( );
	   ++ p )
   {
      r1. addpremisse( *p );
   }

   for( auto p = vs. vars_cbegin( ); p != vs. vars_cend( ); ++ p )
   {
      std::vector< variable > args;
      args. push_back( *p );
      r1. addpremisse( varatom( predicate::create, args ));
   }

   for( rule::conc_iterator 
           p = disj. concs_begin( );
	   p != disj. concs_end( );
	   ++ p )
   {
      r1. addconclusion( *p);
   }
   r1. addconclusion(va);


   ASSERT( r2. predicates == predicates_base2 &&
           r2. arguments == arguments_base2 );
   ASSERT( disj. bases_disjoint(r2));

   for( rule::prem_iterator
           p = disj. prems_begin( );
	   p != disj. prems_end( );
	   ++ p )
   {
      r2. addpremisse( *p );
   }
   r2. addpremisse( va );
}



void forwardreasoning::disequality_split( 
		             const rule& lemma, 
		             variable v1, variable v2, 
		             rule& r1, 
		             std::vector< predicate > & predicates_base1,
                             std::vector< variable > & arguments_base1,
		             rule& r2,
		             std::vector< predicate > & predicates_base2,
		             std::vector< variable > & arguments_base2 )
{
   ASSERT( lemma. isdisjunctive( ) &&  lemma. nr_conclusions( ) == 0 );

   // Actually, we don't need the variables. We check
   // that v1 and v2 occur in the premisse of lemma. 

   varset vs;
   for( rule::prem_iterator
           p = lemma. prems_begin( );
           p != lemma. prems_end( );
           ++ p )
   {
      for( std::vector< variable > :: const_iterator
              q = (*p). a1;
              q != (*p). a2;
              ++ q )
      {
         vs. insert( *q );
      }
   }

   ASSERT( vs. contains(v1) && vs. contains(v2) );

   ASSERT( r1. predicates == predicates_base1 &&
           r1. arguments == arguments_base1 );
   ASSERT( lemma. bases_disjoint(r1));

   // Copy premisses from r: 

   for( rule::prem_iterator 
           p = lemma. prems_begin( );
	   p != lemma. prems_end( );
	   ++ p )
   {
      r1. addpremisse( *p );
   }

   // and add the disequality v1 != v2:

   {
      std::vector< variable > args;
      args. push_back(v1);
      args. push_back(v2);

      r1. addpremisse( varatom( predicate::diseq, args ));
   }

   ASSERT( r2. predicates == predicates_base2 &&
           r2. arguments == arguments_base2 );
   ASSERT( lemma. bases_disjoint(r2));

   for( rule::prem_iterator
           p = lemma. prems_begin( );
	   p != lemma. prems_end( );
	   ++ p )
   {
      std::vector< variable > args;
      for( std::vector< variable > :: const_iterator
              q = (*p). a1;
	      q != (*p). a2;
	      ++ q )
      {
         if( *q != v1 )
            args. push_back(*q);
	 else
            args. push_back(v2);
      }
      r2. addpremisse( varatom( (*p). p, args ));
   }

}



void forwardreasoning::testing( ) 
{
   variable v0;
   variable v1 = v0; v1 ++ ;
   variable v2 = v1; v2 ++ ;
   variable v3 = v2; v3 ++ ;
   variable v4 = v3; v4 ++ ;
   variable v5 = v4; v5 ++ ;

   element e0;
   element e1 = e0; e1 ++ ;
   element e2 = e1; e2 ++ ;
   element e3 = e2; e3 ++ ;
   element e4 = e3; e4 ++ ;
   element e5 = e4; e5 ++ ;

   predicate aa = predicate( "a", 1 );
   predicate bb = predicate( "b", 1 );  
   predicate pp = predicate( "p", 2 );
   predicate qq = predicate( "q", 3 ); 

   std::vector< unsigned int > parents;
   std::vector< element > eeee;

   model m; 

   eeee. push_back(e0); 
   m. addatom( groundatom( predicate::create, eeee ), 0, parents );
   eeee. clear( ); eeee. push_back(e2);
   m. addatom( groundatom( predicate::create, eeee ), 0, parents );
   eeee. clear( ); eeee. push_back(e3);
   m. addatom( groundatom( predicate::create, eeee ), 0, parents );
   eeee. clear( ); eeee. push_back(e4);
   m. addatom( groundatom( predicate::create, eeee ), 0, parents );
   eeee. clear( ); eeee. push_back(e5);
   m. addatom( groundatom( predicate::create, eeee ), 0, parents );

   eeee. clear( ); eeee. push_back(e2); 
   m. addatom( groundatom( bb, eeee ), 0, parents );


   eeee. clear( ); eeee. push_back(e0); eeee. push_back(e3);
   m. addatom( groundatom( pp, eeee ), 0, parents );
   eeee. clear( ); eeee. push_back(e5);
   m. addatom( groundatom( aa, eeee ), 0, parents );


   eeee. clear( ); eeee. push_back(e1);
   m. addatom( groundatom( predicate::create, eeee ), 0, parents );

   eeee. clear( ); eeee. push_back(e0); eeee. push_back(e1);
   m. addatom( groundatom( pp, eeee ), 0, parents );

   std::cout << m << "\n";

   std::vector< predicate > predicates_base;
   std::vector< variable > arguments_base;


   std::cout << "\n\n"; 

   false_instance existential( rule( predicates_base, arguments_base ));

   std::vector< variable > vvvv; 
   vvvv. push_back(v0);
   existential. r. addpremisse( varatom( predicate::create, vvvv ));

   vvvv. clear( );
   vvvv. push_back(v2);
   existential. r. addpremisse( varatom( bb, vvvv ));

   vvvv. clear( ); vvvv. push_back(v0); vvvv. push_back(v1);
   existential. r. setexistential( v1, varatom( pp, vvvv ));

   existential. prem_match. push_back(0);
   existential. prem_match. push_back(5);
   bool b1 = existential. computesubst(m);
   ASSERT(b1);

   std::cout << "existential rule: " << existential << "\n";

   false_instance lemma( rule( predicates_base, arguments_base ));

   vvvv. clear( );
   vvvv. push_back(v0); vvvv. push_back(v1);
   lemma. r. addpremisse( varatom( pp, vvvv ));

   vvvv. clear( );
   vvvv. push_back(v0); vvvv. push_back(v3);
   lemma. r. addpremisse( varatom( pp, vvvv ));

   vvvv. clear( );
   vvvv. push_back(v5);
   lemma. r. addpremisse( varatom( aa, vvvv ));

   vvvv. clear( );
   vvvv. push_back(v0); vvvv. push_back(v1);
   lemma. r. addpremisse( varatom( predicate::diseq, vvvv ));

   vvvv. clear( );
   vvvv. push_back(v3); vvvv. push_back(v5);
   lemma. r. addpremisse( varatom( predicate::diseq, vvvv ));

   lemma. prem_match. push_back(9);
   lemma. prem_match. push_back(6);
   lemma. prem_match. push_back(7);
   lemma. prem_match. push_back(8);
   lemma. prem_match. push_back(4); 
   bool b2 = lemma. computesubst(m);
   ASSERT(b2); 
   std::cout << "lemma: " << lemma << "\n";

   {
      std::vector< predicate > predicates_base;
      std::vector< variable > arguments_base;

      false_instance 
	      r = existential_resolvent( 
		      predicates_base,
		      arguments_base,
                      existential,
                      lemma,
		      m, 8 );
      std::cout << "existential resolvent = " << r << "\n\n\n";
   }

   false_instance disjunction( rule( predicates_base, arguments_base ));

   vvvv. clear( );
   vvvv. push_back(v0); vvvv. push_back(v1);
   disjunction. r. addpremisse( varatom( pp, vvvv ));
   vvvv. clear( ); vvvv. push_back(v2);
   disjunction. r. addpremisse( varatom( predicate::create, vvvv ));

   vvvv. clear( ); vvvv. push_back(v1); vvvv. push_back(v0);
   disjunction. r. addconclusion( varatom( pp, vvvv ));
   vvvv. clear( ); vvvv. push_back(v0); vvvv. push_back(v2);
   disjunction. r. addconclusion( varatom( pp, vvvv ));
   vvvv. clear( ); vvvv. push_back(v0); vvvv. push_back(v1);
   disjunction. r. addconclusion( varatom( pp, vvvv ));

   disjunction. prem_match. push_back(6);
   disjunction. prem_match. push_back(8);

   bool b3 = disjunction. computesubst(m);
   std::cout << "disjunction = " << disjunction << "\n";
   ASSERT(b3);

   {
      std::vector< predicate > predicates_base;
      std::vector< variable > arguments_base;

      false_instance
         r = disjunction_resolvent( 
                 predicates_base,
		 arguments_base,
		 disjunction,
		 1, 
		 lemma,
		 m, 9 ); 
      std::cout << "disjunction resolvent = " << r << "\n";
   }

   //  We test simplify:

   false_instance simpl( rule( predicates_base, arguments_base ));

   vvvv. clear( );
   vvvv. push_back(v1); vvvv. push_back(v2); 
   simpl. r. addpremisse( varatom( predicate::diseq, vvvv ));
   simpl. prem_match. push_back(9);

   vvvv. clear( );
   vvvv. push_back(v1);  vvvv. push_back(v2); vvvv. push_back(v4); 
   simpl. r. addpremisse( varatom( qq, vvvv ));
   simpl. prem_match. push_back(1);

   vvvv. clear( );
   vvvv. push_back(v2); vvvv. push_back(v1); vvvv. push_back(v3);
   simpl. r. addpremisse( varatom( qq, vvvv ));
   simpl. prem_match. push_back(1);

   std::cout << simpl << "\n";

   groundsubst s; 
   s. assign( v1, e0 );
   s. assign( v2, e0 );

   {
      std::vector< predicate > predicates_base;
      std::vector< variable > arguments_base;

      rule r = sortlemma( predicates_base, arguments_base, lemma. r, 2 );

      std::cout << "simplification = " << r << "\n";
   }

}

void forwardreasoning::testsplits( )
{
   variable v0;
   variable v1 = v0; v1 ++ ;
   variable v2 = v1; v2 ++ ;
   variable v3 = v2; v3 ++ ;
   variable v4 = v3; v4 ++ ;
   variable v5 = v4; v5 ++ ;

   predicate aa = predicate( "a", 1 );
   predicate bb = predicate( "b", 1 );
   predicate pp = predicate( "p", 2 );
   predicate qq = predicate( "q", 3 );
   predicate rr = predicate( "r", 2 );

   std::cout << "\n\n";
   std::cout << "Testing Simplifications:\n\n";

   std::vector< predicate > predicates_base;
   std::vector< variable > variables_base;

   rule r = rule( predicates_base, variables_base );

   std::vector< variable  > args;
   args. push_back(v0);
   args. push_back(v1);

   r. addpremisse( varatom( pp, args ));

   args. clear( );
   args. push_back(v1);
   args. push_back(v3);

   r. addpremisse( varatom( rr, args ));

   std::cout << r << "\n";

   std::vector< predicate > predicates_base1;
   std::vector< predicate > predicates_base2;

   std::vector< variable > variables_base1;
   std::vector< variable > variables_base2;

   rule r1 = rule( predicates_base1, variables_base1 );
   rule r2 = rule( predicates_base2, variables_base2 );


   args. clear( );
   args. push_back(v0);
   args. push_back(v2); 
   args. push_back(v4);

   disjunctive_split( r, varatom( qq, args ), 
		      r1, predicates_base1, variables_base1,
		      r2, predicates_base2, variables_base2 );

   std::cout << r1 << "\n";
   std::cout << r2 << "\n";

}







