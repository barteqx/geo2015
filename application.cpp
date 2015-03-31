
// Code written by Hans de Nivelle, February 2006. 
// Revised April 2007. 

#include "application.h"

#include "swll/searchstate.h"
#include "swll/matchcounter.h"
#include "statistics/distribution.h"
#include "statistics/timer.h"
#include "statistics/reporter.h"
#include <stdexcept>

unsigned int application::getweight( const model& m ) const
{ 
   unsigned int weight = 0; 
   for( std::vector< unsigned int > :: const_iterator
           p = f. prem_match. begin( );
	   p != f. prem_match. end( );
	   ++ p )
   {
      model::atom_iterator a = m. make_atom_iterator(*p);
      weight += m. getweight(a);
   }
   return weight;
}


unsigned int application::getlevel( const model& m ) const
{
   unsigned int level = 0;
   for( std::vector< unsigned int > :: const_iterator
           p = f. prem_match. begin( );
	   p != f. prem_match. end( );
	   ++ p )
   {
      model::atom_iterator a = m. make_atom_iterator(*p);
      if( m. getlevel(a) + 1 > level )
         level = m. getlevel(a) + 1;
   }
   return level;
}


unsigned int application::getsupatoms( const model& m ) const
{
   return f. sup_prem_match( );
}


int application::comparerulesnrpremisses( const application& a1,
		                          const application& a2 ) 
{
   unsigned int p1 = a1. f. r. nr_premisses( ); 
   unsigned int p2 = a2. f. r. nr_premisses( );
   if( p1 < p2 )
      return -1;
   if( p1 > p2 )
      return 1;
   return 0;
}


int application::comparerulesweight( const application& a1,
		                     const application& a2 )
{
   unsigned int w1 = a1. f. r. getweight( );
   unsigned int w2 = a2. f. r. getweight( );

   if( w1 < w2 )
      return -1;
   if( w1 > w2 )
      return 1;
   return 0;
}


int application::comparerulesnrvariables( const application& a1,
		                          const application& a2 )
{
   unsigned int v1 = a1. f. r. nr_variables( );
   unsigned int v2 = a2. f. r. nr_variables( );

   if( v1 < v2 )
      return 1;
   if( v1 > v2 )
      return -1;
   return 0;
}


int application::comparerandomly( const application& a1,
		                  const application& a2 )
{
   if( a1. rule_nr == a2. rule_nr )
      return 0;

   unsigned int r = ( a1. rule_nr + a2. rule_nr ) % 2;
   if( r == 1 )
      return -1;
   return 1;
}


int application::comparerulesnr( const application& a1,
		                 const application& a2 )
{
   if( a1. rule_nr < a2. rule_nr )
      return -1;
   if( a1. rule_nr > a2. rule_nr )
      return 1;
   return 0;
}


int application::comparematchingsweight( const model& m,
		                         const application& a1,
					 const application& a2 )
{
   unsigned w1 = a1. getweight(m);
   unsigned w2 = a2. getweight(m);

   if( w1 < w2 )
      return -1;
   if( w1 > w2 )
      return 1;
   return 0;
}


int application::comparematchingslevel( const model& m,
		                        const application& a1,
					const application& a2 )
{
   unsigned int l1 = a1. getlevel(m);
   unsigned int l2 = a2. getlevel(m);

   if( l1 < l2 )
      return -1;
   if( l1 > l2 )
      return 1;
   return 0;
}


int application::comparematchingssupatoms( const model& m,
		                           const application& a1,
			                   const application& a2 )
{
   unsigned int s1 = a1. getsupatoms(m);
   unsigned int s2 = a2. getsupatoms(m);

   if( s1 < s2 )
      return -1;
   if( s1 > s2 )
      return 1;
   return 0;
}


int application::comparematchingssupelements( const model& m,
                                              const application& a1,
                                              const application& a2 )
{
   element e1 = a1. f. sup_elements();
   element e2 = a2. f. sup_elements();

   if( e1 < e2 )
      return -1;
   if( e1 > e2 )
      return 1;
   return 0;
}


int application::comparematchingsbranchingdegree( const application& a1,
		                                  const application& a2 )
{
   if( a1. branching_degree < a2. branching_degree )
      return -1;
   if( a1. branching_degree > a2. branching_degree )
      return 1;
   return 0;
}

// -1 means that left is better:

int application::compareforward( const model& m,
		                 const application& a1,
			         const application& a2 )
{

   ASSERT( a1. f. r. isexistential( ) || a1. f. r. nr_conclusions( ));
   ASSERT( a2. f. r. isexistential( ) || a2. f. r. nr_conclusions( ));

   int v = 0;

   v = comparematchingsbranchingdegree( a1, a2 );
   if(v) return v;

   v = comparematchingsweight( m, a1, a2 );
   if(v) return v;

   v = comparematchingslevel( m, a1, a2 );
   if(v) return v;

   v = comparerulesnrpremisses( a1, a2 );
   if(v) return v;

   v = comparerulesnrvariables( a1, a2 );
   if(v) return v;

   v = comparerulesnr( a1, a2 );
   if(v) return v;

   return 0;
}


// -1 means that left is better:

int application::compareclosing( const model& m,
		                 const application& a1,
                                 const application& a2 )
{

   ASSERT( a1. f. r. isdisjunctive( ) && a1. f. r. nr_conclusions( ) == 0 );
   ASSERT( a2. f. r. isdisjunctive( ) && a2. f. r. nr_conclusions( ) == 0 );

   int v = 0;

   // Why this distinction is meaningful, is a complete and total
   // mystery to me. I found it empirically. I understand nothing.
   // This could be coincidence of course, e.g. that it depends
   // on a single application where we are just lucky. 

   if( a1. rule_nr == a2. rule_nr )
   {
      v = comparematchingsweight( m, a1, a2 );

      if(v) 
      {
         std::cout << "matching weights differ: \n";
         std::cout << m << "\n";
         std::cout << a1 << "\n";
         std::cout << a2 << "\n";
         std::cout << "v = " << v << "\n\n\n"; 

         if( v == -comparematchingssupatoms( m, a1, a2 ))
         {
            throw std::runtime_error( 
               "supatoms would have been in the other direction" );
         }
         return v;
      }
      v = comparematchingslevel( m, a1, a2 );

      if(v)
      {
         std::cout << "matchings levels differ:\n";
         std::cout << a1 << "\n";
         std::cout << a2 << "\n";
         std::cout << "v = " << v << "\n\n\n";
         return v;
      } 
   }
   else
   {
#if 0 
      // Enabling this makes it totally impossible
      // to find any non-trivial proofs !!

      v = comparerulesnrvariables( a1, a2 );
      if(v) return v;
#endif 

      v = comparematchingssupatoms( m, a1, a2 );
      if(v) return v;

      v = comparerulesnrpremisses( a1, a2 );
      if(v) return v;

      v = comparerulesnrvariables( a1, a2 );
      if(v) return v;

      v = comparerulesnr( a1, a2 );
      if(v) return v;
   }

   return 0;
}


bool application::conclusionistrue( const model& m, const rule& r,
		                    groundsubst& s )  
{


#if 0
   std::cout << "checking conclusion of " << r << " under ";
   std::cout << s << "\n\n";
#endif 

   if( r. isexistential( ))
   {
      unsigned int subst_size = s. size( );
      varatom va = r. getbody( );

      for( model::atom_iterator atom = m. atoms_begin( );
              atom != m. atoms_end( );
	      ++ atom )
      {
         groundatom ga = m. getatom( atom );
	 if( ga.p == va.p )
         {
            if( s. match( va, ga ))
            {
               s. backtrack( subst_size );
	       return true;
            }
	    s. backtrack( subst_size );
         }
      }
      return false;
   }
   else
   {
      // Disjunctive rule. We look for a true disjunct:

      for( rule::conc_iterator c = r. concs_begin( );
              c != r. concs_end( );
	      ++ c )
      {
         groundatom ga = s. apply_on( *c );
	 for( model::atom_iterator atom = m. atoms_begin( );
                 atom != m. atoms_end( );
	         ++ atom )
         {
            if( m. getatom( atom ) == ga )
               return true;
         }
      }
      return false;
   }
}




partial< application >
application::bestforward( const rulesystem& rs, const model& m ) 
{

   partial< application > best;

   unsigned int rule_nr = 0; 
   for( std::vector< rulesystem::rule_in_system > :: const_iterator
	   rule = rs. rules. begin( );
           rule != rs. rules. end( );
	   ++ rule, ++ rule_nr )
   {
      if( ! rule -> deleted && 
          ( rule -> r. isexistential( ) || 
	    rule -> r. nr_conclusions( ) != 0 )) 
      {

#if 0
         std::cout << "Considering forward rule: " << rule_nr << ": "; 
	 std::cout << rule -> r << "\n";
#endif

         application appl( false_instance( rule -> r ), rule_nr, m ); 

         appl. improve( m, best );
      }
   }

#if 0
   std::cout << "===============================\n\n";

   std::cout << "returning: "; 
   std::cout << best << "\n\n"; 
   std::cout << m << "\n"; 
   std::cout << best. contents( ). getsupatoms(m) << "\n"; 
#endif

   return best; 
}



void application::improve( const model& m, partial< application > & best )
{

   static statistics::distribution times( 0.0, 0.1, 10.0 );
   static unsigned int counter = 0;
   static statistics::reporter r;

   if( r. mustreport( ))
   {
      std::cout << "Report from improve:\n";
      std::cout << "counter = " << counter << "\n";
      r. printduration( std::cout ); 
      std::cout << times << "\n";
      r. wasreported( ); 
   }

   statistics::timer t{ [] ( double d ) { times. insert(d); } };
   ++ counter;

 
   unsigned int subst_size = f. s. size( );
   unsigned int prem_match_size = f. prem_match. size( );

   improve( m, f. r. prems_begin( ), f. r. prems_end( ), best );

   ASSERT( f. s. size( ) == subst_size );
   ASSERT( f. prem_match. size( ) == prem_match_size );
}



void application::improve( const model& m,
		           rule::prem_iterator p1,
			   rule::prem_iterator p2,
			   partial< application > & best )
{

#if 0
   std::cout << "Entering improve for premisse " << *p1 << ":\n"; 
   std::cout << *this << "\n";
#endif

   if( p1 == p2 )
   {
      if( f. r. isdisjunctive( ) && f. r. nr_conclusions( ) == 0 )
      {
         if( best. isdefined( ))
         {
            if( compareclosing( m, *this, best. contents( )) == -1 )
               best = *this;  
         }
         else
            best = *this; 
      }
      else
      {
         if( ! conclusionistrue( m, f. r, f. s ))
         {
            if( best. isdefined( ))
            {
               if( compareforward( m, *this, best. contents( )) == -1 )
                  best = *this; 
            }
            else
               best = *this; 
         }
      }
   }
   else
   {
      unsigned int subst_size = f. s. size( );
      varatom va = * ( p1 ++ );

      if( va. p != predicate::diseq )
      { 
         unsigned int atom_nr = 0;  
         for( model::atom_iterator atom = m. atoms_begin( );
                 atom != m. atoms_end( );
	         ++ atom, ++ atom_nr )
         {
            groundatom ga = m. getatom( atom );
            if( va. p == ga. p && f. s. match( va, ga ))
            {
               f. prem_match. push_back( atom_nr ); 
               improve( m, p1, p2, best ); 
	       f. prem_match. pop_back( ); 
            }
            f. s. backtrack( subst_size );
         }
      }
      else
      {
         std::vector< variable > :: const_iterator a = va. a1;
	 variable v1 = *(a++);
	 variable v2 = *a;

	 const element* e1 = f. s. lookup(v1);
	 const element* e2 = f. s. lookup(v2);

	 ASSERT( e1 );
	 ASSERT( e2 );

         if( *e1 != *e2 )
         {
            element e = *e1;
	    if( *e2 > *e1 )
               e = *e2;
	    
            unsigned int atom_nr = 0; 
	    for( model::atom_iterator atom = m. atoms_begin( );
                    atom != m. atoms_end( );
	            ++ atom, ++ atom_nr ) 
            {
               groundatom ga = m. getatom( atom );
	       if( ga. p == predicate::create && *( ga. a1 ) == e )
               {
                  f. prem_match. push_back( atom_nr ); 
                  improve( m, p1, p2, best ); 
                  f. prem_match. pop_back( );
               }
            }
         }
      }
   }
}


partial< application >
application::findclosinglemma( const rulesystem& rs, const model& m, 
                               bool best ) 
{
   partial< application > lemma; 

   unsigned int rule_nr = 0; 
   for( std::vector< rulesystem::rule_in_system > :: const_iterator
	   rule = rs. rules. begin( );
           rule != rs. rules. end( );
	   ++ rule, ++ rule_nr )
   {
      if( ! rule -> deleted && 
          rule -> r. isdisjunctive( ) && 
	  rule -> r. nr_conclusions( ) == 0 ) 
      {

#if 0
         std::cout << "Considering lemma: " << rule_nr << ": "; 
	 std::cout << rule -> r << "\n";
#endif

         partial< application > appl = makeclosurewith( m, rule->r, rule_nr );
         if( appl. isdefined( ))
         {
            if( lemma. isdefined( )) 
            {
               if( compareclosing( m, appl. contents( ), 
                                      lemma. contents( )) == -1 )
                  lemma = appl; 
            }
	    else
            {
               lemma = appl; 
	       if( ! best )
                  return lemma;
            }
         }
      }
   }

#if 0
   std::cout << "returning: "; 
   std::cout << lemma << "\n\n"; 
#endif

   return lemma; 
}




partial< application >
application::makeclosurewith( const model& m, const rule& lem, unsigned int nr ) 
{

   static statistics::distribution times( 0.0, 0.1, 10.0 );
   static unsigned int counter = 0;
   static statistics::reporter r;
   static swll::matchcounter mc;

   if( r. mustreport( ))
   {
      std::cout << "Report from makeclosurewith:\n";
      std::cout << "counter = " << counter << "\n";
      r. printduration( std::cout );  
      std::cout << times << "\n";
      std::cout << mc << "\n\n";
      r. wasreported( );
   }

   statistics::timer t{ [] ( double d ) { times. insert(d); } };
   ++ counter;


   ASSERT( lem. isdisjunctive( ) && lem. nr_conclusions( ) == 0 );

   if( lem. nr_premisses( ) >= 5 )
   {
      mc. attempted ++ ; 

      static swll::searchstate state;
      bool b = state. setproblem( lem, m );
  
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
               application appl = application( false_instance( lem ), nr, m ); 

               appl. f. s = state. makegroundsubst( );
       	       bool acc = appl. f. computematching(m);
               ASSERT( acc );
               return appl;
            }
         }
      }
      return partial< application > ( ); 
   }
   else
   {
      partial< application > best; 
      application appl = application( false_instance( lem ), nr, m );

      appl. improve( m, best );

      return best; 
   }
}



void application::test_swll_match( const model& m, const rulesystem& rs )
{
   ASSERT( rs. nr_rules( ) == 1 );
   rule r = rs. rules [0]. r;

   std::cout << m << "\n";
   std::cout << r << "\n";

   ASSERT( r. isdisjunctive( ));
   ASSERT( r. nr_conclusions( ) == 0 );

   swll::searchstate state;

   bool b = false;
   for( unsigned int i = 0; i < 1; ++ i )
      b = state. setproblem( r, m );

   if(b)
   {
      for( unsigned int i = 0; i < 1; ++ i )
         state. interreduce( );

      std::cout << state << "\n";

      for( unsigned int i = 0; i < 1; ++ i )
         b = state. searchsolution( );
      if(b)
      {
         std::cout << "found solution: ";
	 std::cout << state. makegroundsubst( ) << "\n"; 
      }
      else
         std::cout << "found no solution\n"; 
   }
   else
      std::cout << "failed in setproblem\n"; 

}


std::ostream& operator << ( std::ostream& stream,
		            const application& appl )
{
   stream << "application( ";
   stream << appl. f << ", ";
   stream << "rule number = " << appl. rule_nr << ", "; 
   stream << "branching degree = " << appl. branching_degree << " )";
   return stream;
}


