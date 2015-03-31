
// Code written by Hans de Nivelle, November 2005.
// Modified in June 2006, so that it can deal with
// breadth-first search. (These modifications were quickly undone) 
// 
// Modified in June 2007. All functions now returns lists
// of closing lemmas instead of single closing lemmas.
// During backtracking, we try to cache and reuse closing lemmas. 


#include "search.h"
#include "forwardreasoning.h"
#include "application.h"
#include "simplification.h"
#include "statistics/reporter.h"
#include "geometric/permutationgenerator.h"

namespace 
{

   // Sort/condense/simplify the new lemma, and add it to rs. 
   // We return the list of lemmas that were added. 
   // In case additional (simplifying) derivations are made, 
   // these will be temporarily added to deriv. For this reason,
   // deriv cannot be const. 


   std::list< unsigned int >
          addnewlemma( const rule& lemma, 
                       std::vector< rulesystem::binary_step > & deriv, 
	               rulesystem& rs,
		       const model& m )
   {

      std::cout << "Created a lemma:  ";
      std::cout << lemma. nr_premisses( ) << ", ";
      std::cout << lemma. nr_variables( ) << "\n";

      std::list< rule > rules;
      std::list< std::vector< rulesystem::binary_step > > derivations;

      std::vector< predicate > predicates_base;
      std::vector< variable > arguments_base;

      rules. push_back( 
	 forwardreasoning::sortlemma( predicates_base, arguments_base,
		                      lemma, 1 )); 
      derivations. push_back( deriv );

      unsigned int nr_rules_before_adding = rs. nr_rules( );

      simplification::addtorulesystem( rules, derivations,
		                       predicates_base, arguments_base,
				       rs, m. closure_counter );


      std::list< unsigned int > result;

      for( unsigned int 
	      nr = nr_rules_before_adding; 
	      nr != rs. nr_rules( );
	      ++ nr )
      {
         if( ! rs. rules [ nr ]. deleted )
         {
            result. push_back( nr ); 
         }
      }

      ASSERT( result. size( ));
      return result; 
   }


   // It must be the case that rs contains a closing lemma for each 
   // of the possible extensions based on disj. 
   
   std::list< unsigned int > lemmasfromdisjunction( 
	          false_instance& disj, 
	          std::vector< rulesystem::binary_step > & deriv, 
	          model& m, 
                  rulesystem& rs )
   {
      ASSERT( disj. r. isdisjunctive( ));

      if( disj. r. nr_conclusions( ) == 0 )
      {
         std::list< unsigned int > nrs = addnewlemma( disj. r, deriv, rs, m ); 
         nrs = search::selectclosing( nrs, m, rs ); 
         ASSERT( nrs. size( ));
         return nrs; 
      }

      unsigned int m_size = m. nr_atoms( );

      // We add the first member of the disjunction in order
      // to catch a lemma.

      rule::conc_iterator at = disj. r. concs_begin( );
      m. addatom( disj. s. apply_on( *at ), 1000000, disj. prem_match );
      partial< application > lemma = 
	          application::findclosinglemma( rs, m, true ); 
      m. backtrack( m_size );

      ASSERT( lemma. isdefined( ) ); 
      ASSERT( lemma. contents( ). f. sup_prem_match( ) > m_size ); 

      std::vector< predicate > predicates_base;
      std::vector< variable > arguments_base;

      deriv. push_back( rulesystem::binary_step(
			   rulesystem::step_disjunctive_res, 
			   lemma. contents( ). rule_nr ));

      false_instance disj2 = forwardreasoning::disjunction_resolvent(
                predicates_base,
	        arguments_base,
	        disj, 0,
	        lemma. contents( ). f, m, m_size );

      return lemmasfromdisjunction( disj2, deriv, m, rs );
   }



   std::list< unsigned int > lemmasfromexistential( 
		  false_instance& existential,
		  std::vector< rulesystem::binary_step > & deriv, 
		  model& m, 
		  variable quantvar, 
		  element fresh_element, 
		  rulesystem& rs )
   {
      ASSERT( existential. r. isexistential( )); 

      unsigned int s_size = existential. s. size( ); 
      unsigned int m_size = m. nr_atoms( );

      existential. s. assign( quantvar, fresh_element );
      m. addatom( existential. s. apply_on( existential. r. getcreate( )),
			          100000, existential. prem_match );
      m. addatom( existential. s. apply_on( existential. r. getbody( )),
			          100000, existential. prem_match );

      partial< application > lemma = 
	         application::findclosinglemma( rs, m, true ); 
      m. backtrack( m_size );
      existential. s. backtrack( s_size );

      ASSERT( lemma. isdefined( ) );
      ASSERT( lemma. contents( ). f. sup_prem_match( ) > m_size ); 

      std::vector< predicate > predicates_base;
      std::vector< variable > arguments_base;

      deriv. push_back( rulesystem::binary_step(
			      rulesystem::step_existential_res,
			      lemma. contents( ). rule_nr ));

      ASSERT( forwardreasoning::ext_res_possible( existential, 
	             lemma. contents( ). f, m_size ));

      // The existential resolvent is a disjunction 
      // the conclusions of which need to be further resolved away.

      false_instance disj = forwardreasoning::existential_resolvent(
           predicates_base,
	   arguments_base,
           existential, lemma. contents( ). f, m, m_size );

      return lemmasfromdisjunction( disj, deriv, m, rs );
   }
}



std::list< unsigned int > 
search::extendstomodel( model& m, rulesystem& rs )
{
   static statistics::reporter r;

#if 0
   std::cout << "========================\n"; 
   std::cout << "Entering extendstomodel \n";
   std::cout << m << "\n";
#endif

   // In case there already exists a closing lemma, we return it. 

   partial< application > existinglemma = 
	           application::findclosinglemma( rs, m, false );

   if( existinglemma. isdefined( ))
   {
      ASSERT( !rs. rules [ existinglemma. contents( ). rule_nr ]. inductive );
         // Inductive rules should never be applicable. 

#if 1 
      if( m. closure_counter % 10 == 0 )
      {
         std::cout << "closure  " <<  m. closure_counter << ",  ";
	 std::cout << "rule number  ";
	 std::cout << existinglemma. contents( ). rule_nr << ",  ";
	 std::cout << "atoms  " << m. nr_atoms( ) << ",  "; 
         std::cout << "elements  " << m. nr_elements( ) << ".\n"; 
      }
#endif 

      if( r. mustreport( ))
      {
         geometric::signature sig;

	 std::cout << rs << "\n\n\n";
         std::cout << "-------------------------------------\n";
	 std::cout << m;
         std::cout << m. make_simple_index(sig) << "\n";
         std::cout << "-------------------------------------\n\n";

         std::cout << "The closing lemma is " << existinglemma << "\n\n"; 
         std::cout << existinglemma.contents( ).f.r.make_simple_index(sig);
         std::cout << "\n";
         r. printduration( std::cout );  
         std::cout << "-------------------------------------\n\n";

         r. wasreported( ); 
      }

      ++ m. closure_counter; 

      std::list< unsigned int > result;
      result. push_back( existinglemma. contents( ). rule_nr ); 
      return result; 
   }

   partial< application > forward = application::bestforward( rs, m );
  
   if( forward. isundefined( ))
   {
      // This means that there is no applicable forward rule. 
      // Let's congratulate ourselves. We have found a model! 

      return std::list< unsigned int > ( ); 
   }
 
   application forw = forward. contents( );

   // Starting with a random number is sometimes better:

   static geometric::signature sig; 
   auto simpl = m. make_simple_index( sig );
 
   static geometric::permutationgenerator 
      gen{ geometric::permutationgenerator::strategy::left2right, 3 };
 
   if( forw. f. r. isexistential( ))
   {
      std::list< unsigned int > cached;
         // We store the closing rules that we have collected,
         // in the hope that we can reuse them. The cache is not
         // used when lemmas are created. The cache is only used  
         // to check if a lemma that closes a branch would also close
         // the next branch. 

      unsigned int m_nr_elements = m. nr_elements( );

      // There are two stages: In the first stage, we ensure that every branch 
      // has a closing lemma. In the second stage, the lemmas are looked
      // up one more time, and a new lemma is generated.
      // The second stage is carried out inside lemmasfromexistential(   ). 

      geometric::permutation perm = gen. makepermutation( m_nr_elements );

      for( unsigned int altcnt = 0; altcnt <= m_nr_elements; ++ altcnt )
      {

         std::list< unsigned int > closing = 
	       extendexistential( forw, m, 
                 ( altcnt < m_nr_elements ) ? perm( altcnt ) : m_nr_elements,
                 altcnt, m_nr_elements, 
                 rs, cached );
                        
         if( closing. size( ) == 0 ) 
            return closing;

         // It is an essential invariant of the for loop that there 
         // exists no closing lemma of m. 
         // If there are members of closing that close m, we return 
         // those members.

         std::list< unsigned int > closing2 = selectclosing( closing, m, rs );  

	 if( closing2. size( ))
         {
	    // std::cout << "sharpening!\n";
	    return closing2; 
         }

         mergewith( cached, closing ); 
      }

      // The existential rule is the first parent of the derivation
      // that we are going to make:

      std::vector< rulesystem::binary_step > deriv;
      deriv. push_back( rulesystem::binary_step(
                           rulesystem::step_startwith,
			   forw. rule_nr )); 

      element newelement = element::constructfromunsigned( m_nr_elements );

      variable quantvar = * ( forw. f. r. getcreate( ). a1 );

      return lemmasfromexistential( forw. f, deriv, 
		                    m, quantvar, newelement, rs );
   }
   else
   {
      unsigned int nr_conclusions = forw. f. r. nr_conclusions( ); 

      std::list< unsigned int > cached;
         // It is probably not helpful with disjunctive rules.

      geometric::permutation perm = gen. makepermutation( nr_conclusions );

      for( unsigned int altcnt = 0; altcnt < nr_conclusions; ++ altcnt ) 
      {
         std::list< unsigned int > closing = 
               extenddisjunction( forw, m, perm( altcnt ), 
                                  altcnt, nr_conclusions, 
                                  rs, cached ); 

         if( closing. size( ) == 0 ) 
            return closing;

         // It is an invariant of this for loop that there exists no
         // closing lemma of m. If closing contains some, we return
         // the closing lemmas in closing. 

         std::list< unsigned int > closing2 = selectclosing( closing, m, rs );

	 if( closing2. size( ))
         {
            // std::cout << "sharpening!\n";
	    return closing2; 
         }

         mergewith( cached, closing ); 
      }

      // The disjunction is first parent of the derivation 
      // that we are about to construct: 

      std::vector< rulesystem::binary_step > deriv; 
      deriv. push_back( rulesystem::binary_step(
                           rulesystem::step_startwith,
			   forw. rule_nr )); 

      return lemmasfromdisjunction( forw. f, deriv, m, rs );
   }
}


std::list< unsigned int >
search::extenddisjunction( application& disj, 
                           model& m, 
			   unsigned int alt,
                           unsigned int altcnt,
                           unsigned int nr_conclusions, 
			   rulesystem& rs, 
                           const std::list< unsigned int > & preferred )
{

   ASSERT( disj. f. r. isdisjunctive( ));
   ASSERT( nr_conclusions == disj. f. r. nr_conclusions( ));

   ASSERT( alt < nr_conclusions );
   ASSERT( altcnt < nr_conclusions );

   rule::conc_iterator right = disj. f. r. concs_begin( );
   for( unsigned int i = 0; i < alt; ++ i )
      ++ right;

   unsigned int m_nr_atoms = m. nr_atoms( );

   m. addatom( disj. f. s. apply_on( *right ),
               disj. rule_nr, disj. f. prem_match,
	       altcnt, nr_conclusions );

   std::list< unsigned int > closing = selectclosing( preferred, m, rs );
   if( closing. size( ) == 0 ) 
      closing = extendstomodel( m, rs );

   if( closing. size( ) == 0 ) 
      return closing;
         // This means that we found a model.

   m. backtrack( m_nr_atoms );
   return closing;
}


std::list< unsigned int > 
search::extendexistential( application& existential, 
	                   model& m,
                           unsigned int alt, 
                           unsigned int altcnt, 
			   unsigned int m_nr_elements,
			   rulesystem& rs,
                           const std::list< unsigned int > & preferred )
{
   ASSERT( existential. f. r. isexistential( ));
   ASSERT( m. nr_elements( ) == m_nr_elements );
  
   ASSERT( alt <= m_nr_elements );
   ASSERT( altcnt <= m_nr_elements );

   unsigned int s_size = existential. f. s. size( );
   unsigned int m_nr_atoms = m. nr_atoms( );

   variable quantvar = * ( existential. f. r. getcreate( ). a1 );
      // The quantified variable.

   if( alt < m_nr_elements )
   {
      // Existing element:

      element e = element::constructfromunsigned( alt );

      existential. f. s. assign( quantvar, e );
      m. addatom( existential. f. s. apply_on( existential. f. r. getbody( )),
                  existential. rule_nr, existential. f. prem_match,
		  altcnt, m_nr_elements + 1 );

      std::list< unsigned int > closing = selectclosing( preferred, m, rs );

      // We try to reuse an earlier closing lemma. If that doesn't
      // succeed, we recursively call extendstomodel.

      if( ! closing. size( ))
         closing = extendstomodel( m, rs );

      if( closing. size( ) == 0 )
         return closing;
            // This means m is now a model. 

      m. backtrack( m_nr_atoms );
      existential. f. s. backtrack( s_size );

      return closing; 
   }
   else
   {
      // New element:

      element e = element::constructfromunsigned( m_nr_elements );

      existential. f. s. assign( quantvar, e );
      m. addatom( existential. f. s. apply_on( existential. f. r. getcreate( )),
                  existential. rule_nr, existential. f. prem_match,
		  altcnt, m_nr_elements + 1 );
      m. addatom( existential. f. s. apply_on( existential. f. r. getbody( )),
                  existential. rule_nr, existential. f. prem_match,
		  altcnt, m_nr_elements + 1 );

      std::list< unsigned int > closing = selectclosing( preferred, m, rs ); 
      if( closing. size( ) == 0 )
         closing = extendstomodel( m, rs );

      if( closing. size( ) == 0 ) 
         return closing;
            // This means that m is now a model.

      m. backtrack( m_nr_atoms );
      existential. f. s. backtrack( s_size );
      
      return closing;
   }
}


void search::mergewith( std::list< unsigned int > & list1,
                        const std::list< unsigned int > & list2 )
{
   for( std::list< unsigned int > :: const_iterator
           q = list2. begin( );
           q != list2. end( );
           ++ q )
   {
      for( std::list< unsigned int > :: const_iterator
              p = list1. begin( );
              p != list1. end( );
              ++ p )
      {
         if( *p == *q )
            goto dontadd;
      }
      list1. push_back( *q );
   dontadd:
      ;
   }
}


void search::printlist( std::ostream& stream,
                        const std::list< unsigned int > & list )
{
   stream << "{ "; 
   for( std::list< unsigned int > :: const_iterator
           p = list. begin( );
           p != list. end( );
           ++ p )
   {
      if( p != list. begin( ))
         stream << ", ";
      stream << *p;
   }
   stream << " }\n";
}

 
std::list< unsigned int > search::selectclosing( 
              const std::list< unsigned int > & closing,
              const model& m,
              const rulesystem& rs )
{
   std::list< unsigned int > result;
   for( std::list< unsigned int > :: const_iterator
           p = closing. begin( );
           p != closing. end( );
           ++ p )
   {
      if( /* ! rs. rules [ *p ]. deleted && */
          application::makeclosurewith( 
                            m, rs. rules [ *p ]. r, *p ). isdefined( ))
      {
         result. push_back( *p );
            // Note that it is possible that a rule has been deleted in 
            // the main time. We are not checking this. 
            // This should be OK, however, because a rule is deleted, 
            // only when it is redundant, i.e. implied by other clauses.
      }
   }
   return result;
}


