

// Code written by Hans de Nivelle, November 2005. 


#include "rule.h"
#include "groundsubst.h"


rule rule::clone_into( std::vector< predicate > & p,
		       std::vector< variable > & a ) const 
{
   ASSERT( & predicates != & p );
   ASSERT( & arguments != & a );

   rule result = rule( 
		p, a, ruletype, 
		p. size( ) + prem_pred1 - prem_pred1,
                p. size( ) + prem_pred2 - prem_pred1,
		a. size( ) + prem_args1 - prem_args1,  
		a. size( ) + prem_args2 - prem_args1, 
		p. size( ) + conc_pred1 - prem_pred1,
		p. size( ) + conc_pred2 - prem_pred1, 
		a. size( ) + conc_args1 - prem_args1, 
		a. size( ) + conc_args2 - prem_args1 );

   for( unsigned int i = prem_pred1; 
        i < conc_pred2;
	++ i )
   {
      p. push_back( predicates [i] );
   }

   for( unsigned int i = prem_args1;
        i < conc_args2;
	++ i )
   {
      a. push_back( arguments [i] );
   }

   return result; 
}


bool rule::addpremisse( varatom prem )
{
   ASSERT( prem_pred2 == predicates. size( )); 
   ASSERT( prem_args2 == arguments. size( )); 
	
   for( prem_iterator p = prems_begin( );
	   p != prems_end( );
	   ++ p )
   {
      if( *p == prem )
         return false;
   }

   predicates. push_back( prem. p );
   prem_pred2 ++ ;
   conc_pred1 ++ ;
   conc_pred2 ++ ;

   for( std::vector< variable > :: const_iterator 
           p = prem. a1;
           p != prem. a2;
	   ++ p )
   {
      arguments. push_back( *p );
      prem_args2 ++ ; 
      conc_args1 ++ ;
      conc_args2 ++ ; 
   }

   return true; 
}


bool rule::addconclusion( varatom conc )
{
   ASSERT( ruletype == type_disj ); 

   ASSERT( conc_pred2 == predicates. size( ));
   ASSERT( conc_args2 == arguments. size( ));

   ASSERT( conc. p != predicate::diseq &&
           conc. p != predicate::create );

   for( conc_iterator p = concs_begin( );
	   p != concs_end( );
	   ++ p )
   {
      if( *p == conc )
         return false;
   }

   predicates. push_back( conc. p );
   conc_pred2 ++ ;

   for( std::vector< variable > :: const_iterator 
           p = conc. a1;
           p != conc. a2;
	   ++ p )
   {
      arguments. push_back( *p );
      conc_args2 ++ ; 
   }
       
   return true; 
}


void rule::setexistential( variable v, varatom at )
{
   ASSERT( ruletype == type_disj );
   ASSERT( conc_pred1 == predicates. size( ));
   ASSERT( conc_pred2 == predicates. size( ));
   ASSERT( conc_args1 == arguments. size( ));
   ASSERT( conc_args2 == arguments. size( )); 

   ASSERT( at. p != predicate::diseq && 
           at. p != predicate::create );

   // Construct atom #v and add it to conclusions:

   predicates. push_back( predicate::create );
   conc_pred2 ++ ;

   arguments. push_back( v );
   conc_args2 ++ ;

   // Add at to conclusions:

   predicates. push_back( at. p );
   conc_pred2 ++ ;

   for( std::vector< variable > :: const_iterator
           p = at. a1;
        p != at. a2;
	++ p )
   {
      arguments. push_back( *p );
      conc_args2 ++ ;
   }

   ruletype = type_exists;
}


unsigned int rule::nr_premisses( ) const
{
   return ( prem_pred2 - prem_pred1 );
}


unsigned int rule::nr_conclusions( ) const
{
   ASSERT( ruletype == type_disj );
   return ( conc_pred2 - conc_pred1 );
}


unsigned int rule::nr_variables( ) const
{
   varset vs;

   for( prem_iterator 
	   p = prems_begin( );
           p != prems_end( );
	   ++ p )
   {
      for( std::vector< variable > :: const_iterator 
              a = (*p). a1;
	      a != (*p). a2;
	      ++ a )
      {
         vs. insert(*a);
      }
   }
   return vs. size( );
}


unsigned int rule::getweight( ) const
{
   unsigned int weight = 0;

   for( prem_iterator 
           p = prems_begin( );
	   p != prems_end( );
	   ++ p )
   {
      ++ weight; 
      for( std::vector< variable > :: const_iterator
              a = (*p). a1;
	      a != (*p). a2;
	      ++ a )
      {
         ++ weight;
      }
   }
   return weight;
}
 

bool rule::iswellformed( ) const
{
   // We want to see that every variable occurs in a 
   // # V or usual atom.

   groundsubst s;
   element e;

   // We assign variables that we encounter in the groundsubst s.
   // This makes it easy to check for disequalities, that the
   // variables occurred before.

   for( prem_iterator prem = prems_begin( );
        prem != prems_end( );
	++ prem )
   {
      if( ( * prem ). p == predicate::diseq )
      { 
         // The variables in a disequality should be distinct.

         std::vector< variable > :: const_iterator a = ( *prem ). a1;
         variable v1 = *(a++);
         variable v2 = *(a++);
         if( v1 == v2 )
            return false;

         // The variables should have earlier occurrences.

	 if( ! s. lookup(v1) || ! s. lookup(v2) )
            return false;
      }
      else
      {
         for( std::vector< variable > :: const_iterator a = ( * prem ). a1;
              a != ( * prem ). a2; 
	      ++ a )
         {
            if( ! s. lookup( *a ))      
               s. assign( *a, e );
         }
      }
   }

   if( isdisjunctive( ))
   {
      for( conc_iterator conc = concs_begin( );
           conc != concs_end( );
	   ++ conc )
      {
         if( ( * conc ). p == predicate::diseq )
            return false;
         if( ( * conc ). p == predicate::create )
            return false;

         for( std::vector< variable > :: const_iterator a = ( * conc ). a1;
              a != ( * conc ). a2;
	      ++ a )
         {
            if( ! s. lookup( *a ))
               return false;
         }
      }
   }
   else
   {
      ASSERT( isexistential( ));

      varatom create = getcreate( ); 
      if( create. p != predicate::create )
         return false; 
      if( s. lookup( * ( create. a1 )))
         return false; 
      s. assign( * ( create. a1 ), e );

      varatom body = getbody( );

      for( std::vector< variable > :: const_iterator a = body. a1;
           a != body. a2;
	   ++ a )
      {
         if( ! s. lookup( *a ))
            return false;
      }
   }
   return true; 
}


bool rule::isdisjunctive( ) const
{
   return ( ruletype == type_disj );
}


bool rule::isexistential( ) const
{
   return ( ruletype == type_exists );
}

varatom rule::getcreate( ) const
{
   ASSERT( ruletype == type_exists );
   return varatom( predicate::create, 
		   arguments. begin( ) + conc_args1,
		   arguments. begin( ) + conc_args1 + 1 );
}


bool rule::bases_disjoint( const rule& r ) const 
{
   return & predicates != & r. predicates && 
          & arguments != & r. arguments;
}


bool rule::bases_disjoint( 
	      const std::vector< predicate > & predicates_base,
	      const std::vector< variable > & arguments_base ) const
{
   return & predicates != & predicates_base && 
          & arguments != & arguments_base;
}


varatom rule::getbody( ) const
{
   ASSERT( ruletype == type_exists );
   return varatom( predicates[ conc_pred1 + 1 ],
		   arguments. begin( ) + conc_args1 + 1,
		   arguments. begin( ) + conc_args2 );
}


void rule::print( std::ostream& stream ) const
{
   stream << "Rule: ";
   if( ruletype == type_disj )
      stream << "disj";
   if( ruletype == type_exists )
      stream << "exists";
   stream << "\n";
   stream << "   " << prem_pred1 << ", " << prem_pred2 << "\n";
   stream << "   " << prem_args1 << ", " << prem_args2 << "\n";
   stream << "\n";
   stream << "   " << conc_pred1 << ", " << conc_pred2 << "\n";
   stream << "   " << conc_args1 << ", " << conc_args2 << "\n";
}


varset rule::getvariables( prem_iterator prem1,
		           prem_iterator prem2 ) const
{
   varset result;

   for( prem_iterator prem = prem1; prem != prem2; ++ prem )
   {
      varatom va = *prem;
      for( std::vector< variable > :: const_iterator 
              a = va. a1;
	      a != va. a2;
	      ++ a )
      {
         result. insert(*a);
      }
   }
   return result;
}


varset rule::getvariablesconclusion( ) const
{
   varset result;

   if( isexistential( ))
   {
      varatom va = getbody( ); 
      variable extvar = * (getcreate( ). a1 ); 

      for( std::vector< variable > :: const_iterator 
              a = va. a1;
	      a != va. a2;
	      ++ a )
      {
         if( *a != extvar )
            result. insert(*a);
      }
   }
   else
   {
      for( conc_iterator conc = concs_begin( );
           conc != concs_end( );
	   ++ conc )
      {
         varatom va = *conc;

         for( std::vector< variable > :: const_iterator 
                 a = va. a1;
		 a != va. a2;
		 ++ a )
         {
            result. insert(*a);
         }
      }
   }

   return result;
}


geometric::index::standardsimple< variable >
rule::make_simple_index( geometric::signature& sig ) const
{
   geometric::index::standardsimple<variable> res;

   for( auto at = prems_begin( ); at != prems_end( ); ++ at )
   {
      varatom gr = *at;

      // We convert to new representation:

      geometric::atom< variable > gr2 =
         geometric::atom<variable>( sig. lookup( { gr.p. getstring( ) },
                                                  gr.p. getarity( )),
                                geometric::truthval::T );

      unsigned int i = 0;
      for( auto arg = gr.a1; arg != gr.a2; ++ arg )
      {
         gr2 [i++] = *arg;
      }
      res. insert( gr2 );
   }
   return res;
}


std::ostream& operator << ( std::ostream& stream, const rule& r )
{

   for( rule::prem_iterator p = r. prems_begin( );
        p != r. prems_end( );
	++ p )
   {
      if( p != r. prems_begin( ))
         stream << " /\\ "; 
      stream << *p;
   }

   stream << " -> ";

   ASSERT( r. ruletype == rule::type_disj || 
	   r. ruletype == rule::type_exists );

   if( r. ruletype == rule::type_disj )
   {
      if( r. concs_begin( ) != r. concs_end( ))
      {
         for( rule::conc_iterator p = r. concs_begin( );
              p != r. concs_end( );
	      ++ p )
         {
            if( p != r. concs_begin( ))
               stream << " \\/ ";

	    stream << *p;
         }
      }
      else
         stream << "FALSE";
   }
   else
   {
      stream << r. getcreate( ); 
      stream << " /\\ ";
      stream << r. getbody( ); 
   }

   if( ! r. iswellformed( ))
      stream << " ( the rule is not well-formed ! )"; 

   return stream;
}


