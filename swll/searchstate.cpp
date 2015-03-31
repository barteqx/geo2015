
// Written by Hans de Nivelle, May 2006. 


#include "searchstate.h"

namespace swll
{

   searchstate::searchstate( ) 
      : lemmas( lemmasystem( solution ))
   {
   }

      

   bool searchstate::setproblem( const rule& r, const model& m )
   {
#if 0 
      std::cout << "=====================\n"; 
      std::cout << "initializing problem \n";
      std::cout << r << "\n";
      std::cout << m << "\n";
#endif

      iiii. clear( );
      vvvv. clear( );
      clauses. clear( );

      ASSERT( r. isdisjunctive( ) && r. nr_conclusions( ) == 0 );
         // We think that the other rules need to be handled by
	 // simple matching.

      groundsubst subst;
      unsigned int subst_size = 0;

      for( rule::prem_iterator 
	      p = r. prems_begin( );
              p != r. prems_end( );
	      ++ p )
      {

	 varatom va = *p; 

	 // If va is a 0-arity atom, we don't generate a clause for
	 // it. Either it is there, then we accept it, or it is not
	 // there, then we return false.
     
	 if( va. p. getarity( ) == 0 )
         {
            for( model::atom_iterator
                    atom = m. atoms_begin( );
		    atom != m. atoms_end( );
		    ++ atom )
            {
               if( m. getatom( atom ). p == va. p )
                  goto found;
            }
	    return false;
         found:
	    ;
         }
         else
         {
	    unsigned int v1 = vvvv. size( );
	    addvars(va); 
            unsigned int v2 = vvvv. size( ); 

	    unsigned int i1 = iiii. size( );
	    if( (*p). p != predicate::diseq )
            {
               for( model::atom_iterator 
                       atom = m. atoms_begin( );
		       atom != m. atoms_end( );
		       ++ atom )
               {
                  groundatom ga = m. getatom( atom );

	          if( va. p == ga. p && subst. match( va, ga ))
                  {
                     // If matching succeeds, we push the values
		     // that are assigned to the variables onto vvvv.

		     for( unsigned int v = v1; v != v2; ++ v )
                     {
                        iiii. push_back( 
			   subst. lookup( vvvv[v] ) -> getindex( ) );
                     }
                  }
	          subst. backtrack( subst_size ); 
               }
            }
            else
            {
               // Disequality. We go through the create atoms,
	       // and produce all pairs of distinct elements.
               // This code is only correct when the variables
               // in the disequality are distinct.

               for( model::atom_iterator
                       atom1 = m. atoms_begin( );
		       atom1 != m. atoms_end( );
		       ++ atom1 )
               {
                  if( m. getatom(atom1). p == predicate::create )
                  {
                     element e1 = * ( m. getatom( atom1 ). a1 ); 
                     for( model::atom_iterator
                             atom2 = m. atoms_begin( );
			     atom2 != m. atoms_end( );
			     ++ atom2 )
                     {
                        if( m. getatom( atom2 ). p == predicate::create )
                        {
                           element e2 = * ( m. getatom(atom2). a1 );
			   if( e1 != e2 )
                           {
                              iiii. push_back( e1. getindex( ));
			      iiii. push_back( e2. getindex( )); 
                           }
                        }
                     }
                  }
               }
            }
	    unsigned int i2 = iiii. size( );

	    if( i1 == i2 )
               return false;

	    if( ( v2 - v1 ) == ( i2 - i1 ))
            {
               // This means that there exists only one possibility.

               for( unsigned int v = 0; v != v2 - v1; ++ v )
               {
                  if( subst. lookup( vvvv [ v1 + v ] ) == 0 )
                  {
                     subst. assign( vvvv [ v1 + v ],
                         element::constructfromunsigned( iiii [ i1 + v ] ));
                  }
               }
	       subst_size = subst. size( );
            }

	    clauses. push_back( clause( v1, v2, i1, i2 ));
         }
      }

      set_occ_vd( ); 

      return true; 
   }


   groundsubst searchstate::makegroundsubst( ) const
   {
      groundsubst s;

      for( unsigned int i = 0; i < solution. size( ); ++ i )
      {
         variable v = solution. nthvar(i);
	 const unsigned int* x = solution. lookup(v);
	 ASSERT(x);

	 s. assign( v, element::constructfromunsigned( *x ));
      }
      return s;
   }


   bool searchstate::setproblem( const rule& from, const rule& into,
		                 partial< variable > avoid ) 
   {
      
#if 0
      std::cout << "Entering setproblem with\n";
      std::cout << "from: " << from << "\n";
      std::cout << "into: " << into << "\n";
      if( avoid. isdefined( ))
         std::cout << "avoid: " << avoid << "\n";
#endif

      iiii. clear( );
      vvvv. clear( );
      clauses. clear( );

      ASSERT( from. isdisjunctive( ) && from. nr_conclusions( ) == 0 );

      varmatching match;
      unsigned int match_size = 0; 

      for( rule::prem_iterator
              p = from. prems_begin( );
	      p != from. prems_end( );
	      ++ p )
      {
         varatom from_at = *p;
       
	 // In case arity is 0, we can check immediately whether
	 // there is an atom to match into. We don't need to generate
	 // a clause. 

	 if( from_at. p. getarity( ) == 0 )
         {
            for( rule::prem_iterator 
                    q = into. prems_begin( );
		    q != into. prems_end( );
		    ++ q )
            {
               if( from_at. p == (*q). p )
                  goto found;
            }
	    return false;
         found:
            ;
         }
	 else
         {
	    unsigned int v1 = vvvv. size( );
	    addvars( from_at );
	    unsigned int v2 = vvvv. size( );

	    unsigned int i1 = iiii. size( );

            if( from_at. p != predicate::diseq &&
	        from_at. p != predicate::create	)
            {
	       for( rule::prem_iterator
                       q = into. prems_begin( );
		       q != into. prems_end( );
		       ++ q )
               {
                  varatom into_at = *q;

	          if( from_at. p == into_at. p && 
		      avoids( into_at, avoid ) &&
	              match. match( from_at, into_at ))
                  {
                     for( unsigned int v = v1; v != v2; ++ v )
                     {
                        iiii. push_back( 
                           match. lookup( vvvv[v] ) -> getindex( ) );
                     }
                  }
		  match. backtrack( match_size );
	       }
            }
	    else
            {
               if( from_at. p == predicate::diseq )
               {
                  for( rule::prem_iterator
                          q = into. prems_begin( );
		          q != into. prems_end( );
		          ++ q )
                  {
                     varatom into_at = *q;
		     if( into_at. p == predicate::diseq &&
                         avoids( into_at, avoid ))
                     {
                        std::vector< variable > :: const_iterator 
			        a = into_at. a1;
		        variable v1 = *(a++);
		        variable v2 = *a;

		        iiii. push_back( v1. getindex( ) );
		        iiii. push_back( v2. getindex( ) );

		        iiii. push_back( v2. getindex( ) );
		        iiii. push_back( v1. getindex( ) );
                     }
                  }
	       }
	       else
               {
                  // It must be a create atom.
                  // We collect all the variables of the into clause
		  // as alternatives.

		  for( rule::prem_iterator 
                          q = into. prems_begin( );
			  q != into. prems_end( );
			  ++ q )
                  {
                     varatom into_at = *q;
		     if( avoids( into_at, avoid ))
                     {
		        for( std::vector< variable > :: const_iterator
                                a = into_at. a1;
			        a != into_at. a2;
			        ++ a )
                        {
                           for( unsigned int i = i1; i != iiii. size( ); ++ i )
                           {
                              if( iiii[i] == (*a). getindex( ))
                                 goto varoccurs;
                           }
			   iiii. push_back( (*a). getindex( ));
                        varoccurs:
			   ; 
                        }
		     }
                  }
               }
            }
	    unsigned int i2 = iiii. size( );
	    if( i1 == i2 )
               return false;

	    if( ( v2 - v1 ) == ( i2 - i1 ))
            {
                for( unsigned int v = 0; v != v2 - v1; ++ v )
                {
                   if( match. lookup( vvvv [ v1 + v ] ) == 0 )
                   {
                      match. assign( vvvv[ v1 + v ], 
	                 variable::constructfromunsigned( iiii [ i1 + v ] )); 
                   }
                }
		match_size = match. size( );
            }

	    clauses. push_back( clause( v1, v2, i1, i2 ));
         }
      }

      set_occ_vd( );
      return true;
   }


   bool searchstate::setproblemwithoutdiseq( const rule& from, 
		                             const rule& into )  
   {
#if 0
      std::cout << "Entering setproblemwithoutdiseq with\n";
      std::cout << "from: " << from << "\n";
      std::cout << "into: " << into << "\n";
#endif

      iiii. clear( );
      vvvv. clear( );
      clauses. clear( );

      ASSERT( from. isdisjunctive( ) && from. nr_conclusions( ) == 0 );

      varmatching match;
      unsigned int match_size = 0; 

      varset into_vars = into. getvariables( into. prems_begin( ),
		                             into. prems_end( ));

      for( rule::prem_iterator
              p = from. prems_begin( );
	      p != from. prems_end( );
	      ++ p )
      {
         varatom from_at = *p;
       
	 // In case arity is 0, we can check immediately whether
	 // there is an atom to match into. We don't need to generate
	 // a clause. 

	 if( from_at. p. getarity( ) == 0 )
         {
            for( rule::prem_iterator 
                    q = into. prems_begin( );
		    q != into. prems_end( );
		    ++ q )
            {
               if( from_at. p == (*q). p )
                  goto found;
            }
	    return false;
         found:
            ;
         }
	 else
         {
	    unsigned int v1 = vvvv. size( );
	    addvars( from_at );
	    unsigned int v2 = vvvv. size( );

	    unsigned int i1 = iiii. size( );

	    if( from_at. p != predicate::create && 
                from_at. p != predicate::diseq )
            {
	       for( rule::prem_iterator
                       q = into. prems_begin( );
		       q != into. prems_end( );
		       ++ q )
               {
                  varatom into_at = *q;

	          if( from_at. p == into_at. p && 
	              match. match( from_at, into_at ))
                  {
                     for( unsigned int v = v1; v != v2; ++ v )
                     {
                        iiii. push_back( 
                           match. lookup( vvvv[v] ) -> getindex( ) );
                     }
                  }
		  match. backtrack( match_size );
	       }
            }
	    else
            {
               if( from_at. p == predicate::diseq )
               {
                  for( auto q1 = into_vars. vars_cbegin( );
			    q1 != into_vars. vars_cend( ); ++ q1 )
                  {
                     for( auto q2 = into_vars. vars_cbegin( );
			       q2 != into_vars. vars_cend( ); ++ q2  )
                     {
                        if( *q1 != *q2 )
                        {
                           iiii. push_back( q1 -> getindex( ));  
			   iiii. push_back( q2 -> getindex( )); 
                        }
                     }
                  }
               }
	       else
               {
		  for( auto q = into_vars. vars_cbegin( ); 
			    q != into_vars. vars_cend( ); ++ q )
                  {
		     iiii. push_back( q -> getindex( ));
                  }
               }
            }

	    unsigned int i2 = iiii. size( );
	    if( i1 == i2 )
               return false;

	    if( ( v2 - v1 ) == ( i2 - i1 ))
            {
               for( unsigned int v = 0; v != v2 - v1; ++ v )
               {
                  if( match. lookup( vvvv [ v1 + v ] ) == 0 )
                  {
                     match. assign( vvvv[ v1 + v ], 
	                variable::constructfromunsigned( iiii [ i1 + v ] )); 
                  }
               }
	       match_size = match. size( );
            }

	    clauses. push_back( clause( v1, v2, i1, i2 ));
         }
      }

      set_occ_vd( );
      return true;
   }



   varmatching searchstate::makevarmatching( ) const
   {
      varmatching s;

      for( unsigned int i = 0; i < solution. size( ); ++ i )
      {
         variable v = solution. nthvar(i);
	 const unsigned int* x = solution. lookup(v);
	 ASSERT(x);

	 s. assign( v, variable::constructfromunsigned( *x ));
      }
      return s;
   }


   void searchstate::addvars( varatom va )
   {
      unsigned int base = vvvv. size( );
      for( std::vector< variable > :: const_iterator
              a = va. a1;
	      a != va. a2;
	      ++ a )
      {
         for( unsigned int i = base; i < vvvv. size( ); ++ i )
         {
            if( vvvv [i] == *a ) goto found;
         }
	 vvvv. push_back( *a );
      found:
	 ;
      }
   }



   void searchstate::set_occ_vd( )
   {

      // We collect for each variable the set 
      // of clauses in which it occurs and its possible values.

      vd. clear( );

      for( unsigned int cl = 0; cl < clauses. size( ); ++ cl )
      {
	 unsigned int nrvars = clauses [ cl ]. v2 - clauses [ cl ]. v1; 
         for( unsigned int i = 0; i != nrvars; ++ i )
         {
            variable v = vvvv [ clauses [ cl ]. v1 + i ];

	    // If we encounter v for the first time, we know that 
	    // vd [v]. possible values is empty. In that case, we fill it
	    // with the set of possible values.
	    // Otherwise, we intersect the set of possible values
	    // that we have collected from previous occurrences with the 
	    // set of possible values that we encounter now. 

            if( vd [v]. occurrences. size( ) == 0 )
            {
               for( unsigned int m = clauses [ cl ]. i1 + i;
                    m < clauses [ cl ]. i2; m += nrvars )
               {
                  vd [v]. possiblevalues. insert( iiii[m] );
               }
            }
	    else
            {
               for( unsigned int m = clauses [ cl ]. i1 + i;
                    m < clauses [ cl ]. i2; m += nrvars )
               {
                  vd [v]. possiblevalues. mark( iiii[m] );
               }
	       vd [v]. possiblevalues. restricttomarked( );
            }

	    vd [v]. occurrences. push_back( cl ); 
         }	
      }

      // We go one more time through the clauses and for each variable,
      // for each of its possible values, we fill in the list of occurrences.

      occ. clear( );
      for( unsigned int cl = 0; cl < clauses. size( ); ++ cl )
      {
         unsigned int nrvars = clauses [ cl ]. v2 - clauses [ cl ]. v1;
	 for( unsigned int i = 0; i < nrvars; ++ i )
         {
            variable v = vvvv [ clauses [ cl ]. v1 + i ];
	    for( unsigned int 
		    m = clauses [ cl ]. i1;
		    m < clauses [ cl ]. i2; 
		    m += nrvars )
            {
               unsigned int val = iiii [ m + i ];
          
	       occurrencematrix::insertoccurrence( occ [v] [val], 
			                           cl, m - clauses [cl]. i1 );
            }
         }
      }
   }




   // We check if the set of possiblevalues ( in vd [v] )
   // can be further reduced taking into account an update
   // of a neighbour variable which took place at time neighbourtime.
 
   void searchstate::updatepossiblevalues( variable v, 
		                           unsigned int updateofneighbour,
		                           unsigned int& nrupdatesmade )
   {
#if 0
      std::cout << "updating " << v << " due to " << updateofneighbour << "\n";
      std::cout << "nrupdatesmade = " << nrupdatesmade << "\n";
#endif

      if( vd [v]. lastupdate < updateofneighbour )
      {

         bool changed = false;
            // Will become true if set of possible assignments to 
            // v changes. (Every change is always a restriction.)

         const std::vector< unsigned int > & occ = vd [v]. occurrences;

	 for( auto p = occ. begin( ); p != occ. end( ); ++ p )
         {
            unsigned int cl = *p;
               // Number of a clause containing variable v.

	    unsigned int nrvars = clauses[ cl ]. v2 - clauses[ cl ]. v1;

            // We find the position of v in the clause (in varnr): 

	    unsigned int varnr = 0;
	    while( vvvv [ clauses [cl]. v1 + varnr ] != v )
               ++ varnr;

	    ASSERT( varnr < nrvars );   // Otherwise something went wrong.

	    // We now go through the clause cl. Check if in each atom
	    // all assignments to the variables are possible. For those 
	    // where all other assignments are possible, we mark
	    // the assignment to v as possible.

            for( unsigned int 
		    alt = clauses [cl]. i1;
                    alt < clauses [cl]. i2;
	            alt += nrvars )
            { 
               unsigned int v1 = clauses [cl]. v1;
	       unsigned int i1 = alt; 
                  // Index in vvvv of first variable of literal,
                  // index in iiii of first possible value of literal.

               // We check if all values for the variables are possible.

	       while( v1 != clauses [cl]. v2 )
               {
                  if( ! vd[ vvvv[ v1 ] ]. possiblevalues. contains(
                         iiii [ i1 ] )) goto impossible; 

		  ++ v1;
		  ++ i1; 
               }

               vd [v]. possiblevalues. mark( iiii[ alt + varnr ] );
            impossible:
	       ; 
            }

            if( vd[v]. possiblevalues. restricttomarked( ))
               changed = true;
         }
         
         if( changed )
         {
            // std::cout << "checking variables from a clause containing ";
	    // std::cout << v << "\n";

            unsigned int thisupdate = nrupdatesmade;
            vd [v]. lastupdate = thisupdate;
            ++ nrupdatesmade;

	    for( std::vector< unsigned int > :: const_iterator
                    p = occ. begin( );
		    p != occ. end( );
		    ++ p )
            {
               unsigned int cl = *p;
	       for( unsigned int 
		       i = clauses [cl]. v1;
                       i != clauses [cl]. v2;
		       ++ i )
               {
                  // std::cout << vvvv[i] << " in clause " << *p << "\n"; 
                  updatepossiblevalues( vvvv[i], thisupdate, nrupdatesmade );
               }
            }
         }
         else
         {
            ASSERT( nrupdatesmade > 0 ); 
            vd [v]. lastupdate = nrupdatesmade - 1;
         }
      }
   }


   bool searchstate::interreduce( ) 
   {
      // If you switch this on, there will be no interreduction:

#if 0
      return true;  
#endif 

      unsigned int nrupdatesmade = 2;

      for( unsigned int i = 0; i < vvvv. size( ); ++ i )
         vd [ vvvv[i] ]. lastupdate = 0;

      for( unsigned int i = 0; i < vvvv. size( ); ++ i )
         updatepossiblevalues( vvvv[i], 1, nrupdatesmade );

#if 0
      // This is a test that checks the completeness of the algorithm.
      // I wrote this in June 2014. 
      // I have tested this extensively, and all evidence suggests
      // that the algorithm is complete without this test.
      // Hence, it can be safely switched off. 

      for( unsigned int k = 0; k < 2; ++ k ) 
      {
         for( unsigned int i = 0; i < vvvv. size( ); ++ i )
            updatepossiblevalues( vvvv[i], 1, nrupdatesmade );
      }
#endif

      // I added this on 28.06.2014, because I want to be
      // able to measure how effective the filter is.

      for( unsigned int i = 0; i < vvvv. size( ); ++ i )
      {
         variable v = vvvv[i];
         unsigned int s = vd [v]. possiblevalues. nrelements(2);
         if( s == 0 )
            return false;
      }

      return true; 
   }



   bool searchstate::searchsolution( )
   {
      solution. clear( );

      // We first go through the variables and check if there
      // are variables for which only 0 or 1 assignment is possible.

      for( unsigned int i = 0; i < vvvv. size( ); ++ i )
         vd [ variable::constructfromunsigned(i) ]. lastupdate = 0;

      for( unsigned int i = 0; i < vvvv. size( ); ++ i )
      {
         variable v = vvvv[i];
	 if( vd [v]. lastupdate == 0 )
         {
            vd [v]. lastupdate = 1;
	    unsigned int s = vd [v]. possiblevalues. nrelements(2);
	    if( s == 0 )
               return false;
            if( s == 1 )
               solution. assign( v, vd [v]. possiblevalues. someelement( ));
         }
      }

      // Next, we reset the lemma system:

      lemmas. clear( );
      lemma_container. clear( );

      // We start searching with 0 clauses treated and 0 variables assigned.

      partial< lemma > lem = guessclause( 0, 0 );
      if( lem. isdefined( ))
      {
         ASSERT( lem. contents( ). size( ) == 0 );

	 // std::cout << lemmas. nrlemmas( ) << "\n";
	 // std::cout << lemma_container. size( ) << "\n";

	 return false;
      }
      else
      {
         ASSERT( verifysolution( )); 
         return true; 
      }
   }


   bool searchstate::verifysolution( ) const
   {
      for( unsigned int 
	      clausenr = 0; 
	      clausenr < clauses. size( ); 
	      ++ clausenr )
      {
         if( ! clauseistrue( clausenr ))
            return false; 
      }
      return true;
   }



   partial< lemma > searchstate::forward( 
       unsigned int solutionbase,
       std::vector< occurrencematrix::within_clause > :: const_iterator p1,
       std::vector< occurrencematrix::within_clause > :: const_iterator p2, 
       unsigned int clausenr,
       unsigned int varnr )
   {
      
#if 0
      std::cout << "Entering forward/5 with ";
      std::cout << "clausenr = " << clausenr << ", ";
      std::cout << "varnr = " << varnr << "\n";

      std::cout << solution << "   " <<  
	           "solutionbase = " << solutionbase << "\n";

#endif 

      ASSERT( solutionbase < solution. size( ));

      if( p1 == p2 )
      {
         return forward( solutionbase + 1, clausenr, varnr ); 
      }

#if 0
      std::cout << "considering clause ";
      std::cout << p1 -> clausenr; 
      std::cout << " for forward reasoning\n";
#endif

      if( clauseistrue( p1 -> clausenr ))
      {
         p1 ++ ; 
         return forward( solutionbase, p1, p2, clausenr, varnr );
      }

      unsigned int vars = clauses [ p1 -> clausenr ]. v1;
      unsigned int length = clauses [ p1 -> clausenr ]. v2 -
	                    clauses [ p1 -> clausenr ]. v1;

      // We now check whether the number of possibilities equals 0 or 1:

      unsigned int nrpossible = 0;
      unsigned int the_only_alternative;

      for( std::list< unsigned int > :: const_iterator
              q = p1 -> offsets. begin( );
	      q != p1 -> offsets. end( );
	      ++ q )
      {
         unsigned int alt = clauses [ p1 -> clausenr ]. i1 + *q;
         
         for( unsigned int i = 0; i < length; ++ i )
         {
            variable v = vvvv [ vars + i ];
	    unsigned int x = iiii [ alt + i ];

	    if( vd [v]. possiblevalues. contains(x) == false )
               goto alt_failed;

	    const unsigned int* s = solution. lookup(v);
	    if( s && *s != x )
               goto alt_failed;
         }

	 ++ nrpossible;
	 the_only_alternative = alt;
      alt_failed:
	 ;
      }

      if( nrpossible >= 2 )
      {
         // No determinism at this level, we continue trying on 
	 // the next level. 

         p1 ++ ;
	 return forward( solutionbase, p1, p2, clausenr, varnr );
      }


      std::list< lemma > sublemmas;
      std::list< variable > resolvedvariables;
      std::list< lemma::atom > conflictassignments;

      {
         variable v = solution. nthvar( solutionbase );
	 const unsigned int* x = solution. lookup(v);
	 ASSERT(x);

         conflictassignments. push_back( lemma::atom( v, *x )); 

	 // This is necessary because the assignment v := *x justified
	 // the fact that we limited our attention to the alternatives
	 // in p1 -> offsets. 
         // In case that p1 -> offsets contains all alternatives
	 // in the clauses, this might be somewhat inefficient.
      }

      // If there is one alternative, then we try to obtain a lemma
      // for it: 

      if( nrpossible == 1 )
      {
         unsigned int solution_size = solution. size( );

         for( unsigned int i = 0; i < length; ++ i )
         {
            variable v = vvvv [ vars + i ];
	    unsigned int x = iiii [ the_only_alternative + i ];
	       
	    const unsigned int* s = solution. lookup(v);

	    ASSERT( !s || *s == x );
	    if( !s )
            {
               solution. assign( v, x );
	       resolvedvariables. push_back(v);
            }
	 }

	 // We check if there already exists a lemma:

	 const lemma* f = lemmas. findfalselemma( solution_size );
	 if(f)
         {
	    sublemmas. push_back(*f);
	    solution. backtrack( solution_size );
         }
	 else
         {
            // Otherwise we generate a lemma in a recursive call:

            ++ p1; 
            partial< lemma > lem = forward( solutionbase,
			                    p1, p2, clausenr, varnr );
	    -- p1; 
	    if( lem. isundefined( ))
               return lem;

	    solution. backtrack( solution_size );
	    if( lemmas. isfalse( lem. contents( )))
               return lem;
      
	    sublemmas. push_back( lem. contents( )); 

	    lemmas. insert( lem. contents( ));
	       // We keep the lemma as souvenir. 
         }
      }

      for( std::list< unsigned int > :: const_iterator
              q = p1 -> offsets. begin( );
	      q != p1 -> offsets. end( );
	      ++ q ) 
      {
         unsigned int alt = clauses [ p1 -> clausenr ]. i1 + *q; 

         if( nrpossible == 0 || alt != the_only_alternative )
         {
            for( unsigned int i = 0; i < length; ++ i )
            {
               variable v = vvvv [ vars + i ];
	       unsigned int x = iiii [ alt + i ];

	       if( vd[v]. possiblevalues. contains(x) == false )
                  goto alt_complete; 
            }

	    for( unsigned int i = 0; i < length; ++ i )
            {
               variable v = vvvv [ vars + i ];
	       unsigned int x = iiii [ alt + i ];
	       const unsigned int* s = solution. lookup(v);

	       if( s && *s != x )
               {
                  conflictassignments. push_back( lemma::atom( v, *s ));
		  goto alt_complete; 
               }
            }
	    ASSERT(false);
	       // Because it would have been a possible alternative. 
         }
            
      alt_complete:
	 ;
      }

      lemma lem = lemma::resolve( lemma_container, sublemmas,
		                  resolvedvariables, conflictassignments );
      ASSERT( lemmas. isfalse( lem ));

      return lem;
   }

  



   partial< lemma > searchstate::forward( unsigned int solutionbase,
		                          unsigned int clausenr,
			                  unsigned int varnr )
   {

#if 0
      std::cout << "entering forward/3 with clausenr = ";
      std::cout << clausenr << ", varnr = " << varnr << "\n";
      std::cout << solution << "   "; 
      std::cout << "solutionbase = " << solutionbase << "\n";
#endif

      if( solutionbase == solution. size( ))
         return guessclause( clausenr + 1, varnr );
      else
      {
         variable v = solution. nthvar( solutionbase );
	 const unsigned int* x = solution. lookup(v);
         ASSERT(x);

         return forward( solutionbase, 
			 occ [v] [ *x ]. begin( ), occ [v] [ *x ]. end( ),
			 clausenr, varnr );
      }
   }
			     
      

   partial< lemma > searchstate::guessclause( unsigned int clausenr,
		                              unsigned int varnr )
   {

#if 0
      std::cout << "Entering guessclause for clause " << clausenr << "\n";
      std::cout << solution << "\n\n";
#endif

      if( clausenr == clauses. size( ))
         return partial< lemma > ( ); 

      if( clauseistrue( clausenr ))
         return guessclause( clausenr + 1, varnr ); 

      unsigned int solution_size = solution. size( );
      unsigned int vars = clauses [ clausenr ]. v1;
      unsigned int length = clauses [ clausenr ]. v2 - 
	                    clauses [ clausenr ]. v1; 

      // In the first pass, we recursively generate lemmas for the subcases,
      // in case that is required. 

      for( unsigned int 
	      alt = clauses[ clausenr ]. i1; 
	      alt != clauses[ clausenr ]. i2; 
	      alt += length )
      {
         // Each assignment in this alternative must be consistent
	 // with the possible values of the variable, and it
	 // should be consistent with the substitution. 
	 // Otherwise we skip it. 

         for( unsigned int i = 0; i < length; ++ i )
         {
            variable v = vvvv [ vars + i ];
	    unsigned int x = iiii [ alt + i ];

	    if( vd [v]. possiblevalues. contains(x) == false )
               goto skip_alt;

	    const unsigned int* s = solution. lookup(v);
	    if( s && *s != x ) 
               goto skip_alt; 
         }

         // We could not aforehand exclude this alternative. We add all 
	 // missing assignments and recursively produce lemmas for 
	 // the subcase. 

         for( unsigned int i = 0; i < length; ++ i )
         {
            variable v = vvvv [ vars + i ];
	    unsigned int x = iiii [ alt + i ]; 
	    if( ! solution. lookup(v))
               solution. assign( v, x );
         }

         {
            const lemma* reused = lemmas. findfalselemma( solution_size );
	    if( reused )
            {
               // std::cout << "reusing " << *reused << "\n";
	       solution. backtrack( solution_size ); 
            }
	    else
            {
               // With forward reasoning is somewhat worse. 
#if 0              
               partial< lemma > lem = forward( solution_size, 
			                       clausenr, varnr );
#else
               partial< lemma > lem = guessclause( clausenr + 1, varnr );
#endif

	       if( lem. isundefined( ))
                  return lem;

	       solution. backtrack( solution_size );

	       // If the new lemma is still false, we pass it further down.
	       // It will be inserted at the moment when it becomes true. 

	       if( lemmas. isfalse( lem. contents( )))
                  return lem;

	       // If lemma is not false, we insert it into lemmas:

	       lemmas. insert( lem. contents( ) ); 
            }
         }
      skip_alt:
         ;
      }
      
      // We now have generated lemmas for the subcases. We
      // collect them and make an inference. 

      std::list< lemma > sublemmas;
      std::list< variable > resolvedvariables;
      std::list< lemma::atom > conflictassignments;

      for( unsigned int alt = clauses [ clausenr ]. i1;
                        alt != clauses [ clausenr ]. i2; 
	                alt += length )
      {
         // If one of the assignments in this alternative
	 // is outside the possible assignments for the variable,
	 // then there is nothing to do. 

         for( unsigned int i = 0; i < length; ++ i )
         {
            variable v = vvvv [ vars + i ];
	    unsigned int x = iiii [ alt + i ];

	    if( vd [v]. possiblevalues. contains(x) == false )
               goto alt_complete;
         }

	 // If one of the assignments in this alternative
	 // is inconsistent with the substitution, we add the  
	 // assignment to the conflicting assignments. There is no
	 // problem when the same assignment is inserted
	 // twice, so we don't check for that. 

	 for( unsigned int i = 0; i < length; ++ i )
         {
            variable v = vvvv [ vars + i ];
	    unsigned int x = iiii [ alt + i ];

	    const unsigned int* s = solution. lookup(v);
	    if( s && *s != x )
            {
               conflictassignments. push_back( lemma::atom( v, *s ));
	       goto alt_complete;
            }
         }

         for( unsigned int i = 0; i < length; ++ i )
         {
            variable v = vvvv [ vars + i ];
	    unsigned int x = iiii [ alt + i ];

	    if( ! solution. lookup(v))
            {
               solution. assign( v, x );
               resolvedvariables. push_back(v);
            }
         }

	 {
            const lemma* f = lemmas. findfalselemma( solution_size );
	    ASSERT(f);
            sublemmas. push_back(*f);
            solution. backtrack( solution_size );
         }
      alt_complete:
         ;
      }

      lemma lem = lemma::resolve( lemma_container, sublemmas, 
		                  resolvedvariables, conflictassignments );

#if 0
      lemmas. removebacksubsumed( lem ); 
#endif

      ASSERT( lemmas. isfalse( lem ));
      return lem;
   }



   partial< lemma > searchstate::guessvar( unsigned int clausenr,
		                           unsigned int varnr )
   {
#if 0
      std::cout << "Entering guessvar for variable " << vvvv[ varnr ] << "\n";
      std::cout << solution << "\n";
#endif

      if( varnr == vvvv. size( ))
         return partial< lemma > ( );
 
      if( solution. lookup( vvvv [ varnr ] ))
         return guessvar( clausenr, varnr + 1 );
            // Variable is already assigned. We continue with the next. 

      unsigned int solution_size = solution. size( );

      variable v = vvvv [ varnr ];
      const valueset& vs = vd [v]. possiblevalues;

      // We go through its possible values:

      for( unsigned int x = 0; x < vs. upperbound( ); ++ x )
      {
         if( vs. contains(x) )
         {
            solution. assign( v, x );

	    const lemma* reused = lemmas. findfalselemma( solution_size );
	    if( reused )
            {
               // std::cout << "reusing " << *reused << "\n";
	       solution. backtrack( solution_size );
            }
	    else
            {
               // Forward reasoning is essential here.

               partial< lemma > lem = forward( solution_size, 
			                       clausenr, varnr );

	       if( lem. isundefined( ))
                  return lem;

	       solution. backtrack( solution_size );

	       // If the new lemma is still false, we pass it further
	       // down. It will be inserted as soon as it becomes
	       // true.

	       if( lemmas. isfalse( lem. contents( )))
                  return lem;

	       // Now it is not false anymore, and we insert it into
	       // lemmas.

	       lemmas. insert( lem. contents( ));
            }
         }
      }

      // We now have generated the lemmas for the subcases. 
      // We collect them and make an inference.

      std::list< lemma > sublemmas;
      std::list< variable > resolvedvariables;
      std::list< lemma::atom > conflictassignments;

      resolvedvariables. push_back(v);

      for( unsigned int x = 0; x < vs. upperbound( ); ++ x )
      {
         if( vs. contains(x) )
         {
            solution. assign( v, x );

            const lemma* f = lemmas. findfalselemma( solution_size );
	    ASSERT(f);
	    sublemmas. push_back( *f );
	    solution. backtrack( solution_size );
         }
      }

      lemma lem = lemma::resolve( lemma_container, sublemmas,
		                  resolvedvariables, conflictassignments );

#if 0
      if( lemmas. subsumes( lem ))
      {
         std::cout << " -> lemma is subsumed " << lem << "\n\n\n";
	 std::cout << lemmas. nrlemmas( ) << "\n\n\n";
      }
#endif

      ASSERT( lemmas. isfalse( lem ));
      return lem;
   }
   

   bool searchstate::clauseistrue( unsigned int nr ) const
   {

      unsigned int vars = clauses [ nr ]. v1;
      unsigned int length = clauses [ nr ]. v2 - clauses [ nr ]. v1;

      for( unsigned int 
              alt = clauses [ nr ]. i1;
	      alt != clauses [ nr ]. i2;
	      alt += length )
      {
         for( unsigned int i = 0; i < length; ++ i )
         {
            variable v = vvvv [ vars + i ];
	    unsigned int x = iiii [ alt + i ];
	    if( ! vd [v]. possiblevalues. contains(x) )
               goto skip_alternative;

	    const unsigned int* s = solution. lookup(v);
	    if( !s || *s != x )
               goto skip_alternative;
         }
         return true;

      skip_alternative:
	 ;
      }
      return false;
   }


   bool searchstate::avoids( varatom at, const partial< variable > & v )
   {
      if( ! v. isdefined( ))
         return true;

      variable w = v. contents( );

      for( std::vector< variable > :: const_iterator 
	      a = at. a1;
              a != at. a2;
	      ++ a )
      {
         if( *a == w )
            return false;
      }
      return true;
   }


   std::ostream& operator << ( std::ostream& stream, 
		               const searchstate& state )
   {
      stream << "Search State:\n";

      stream << state. solution << "\n";

      for( unsigned int cl = 0; cl < state. clauses. size( ); ++ cl )
      {
         stream << "clause " << cl << " : ";

	 stream << "["; 
	 for( unsigned int i = state. clauses [ cl ]. v1; 
			   i != state. clauses [ cl ]. v2;
			   ++ i )
         {
            if( i != state. clauses [ cl ]. v1 )
               stream << ",";
	    stream << state. vvvv [i];
         }
	 stream << "] ==> "; 

         unsigned int step = state. clauses [ cl ]. v2 - 
		             state. clauses [ cl ]. v1;

	 for( unsigned int alt = state. clauses [ cl ]. i1; 
	         alt != state. clauses [ cl ]. i2;
	         alt += step )
         {
            if( alt != state. clauses [ cl ]. i1 )
               stream << " | "; 

            stream << "["; 
            for( unsigned int i = 0; i < step; ++ i )
            {
               if( i != 0 )
                  stream << ",";
	       stream << state. iiii [ alt + i ];
            }

	    stream << "]"; 
         }
	 stream << "\n"; 

      }

      stream << "\n"; 
      stream << state. vd << "\n"; 
      stream << state. occ << "\n"; 
      return stream;
   }

} 

