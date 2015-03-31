
// Geometric deduction rules, as described in Section 5 of the
// Journal of Logic and Computation paper. 
// Written by Hans de Nivelle, Jan-Feb 2015.

#include "deduction.h"

void geometric::horn_res::flatten( )
{
   auto p = dynamic_cast< const horn_res* > ( & clos. getinference( ));
   while(p)
   {
      std::vector< rule > parents = std::move( horn );
      for( const rule& r : p -> horn )
         parents. push_back( r );
      horn = std::move( parents ); 
      clos = ( p -> clos );  
      p = dynamic_cast< const horn_res* > ( & clos. getinference( ));
   }
}


auto geometric::horn_res::getparents( ) const -> std::vector<rule>
{
   std::vector< rule > parents = horn;
   parents. push_back( clos );
   return parents;
}

void geometric::horn_res::print( std::ostream& out ) const
{
   out << "horn_res( ";
   for( auto p = horn. begin( ); p != horn. end( ); ++ p )
   {
      if( p != horn. begin( ))
         out << ", ";
      p -> printkey( out );
   }
   out << "; ";
   clos. printkey( out );  
   out << " )";
}


void geometric::exdisj_res::flatten( )
{
   auto p = dynamic_cast< const exdisj_res* > ( & start. getinference( ));
   while(p)
   {
      std::vector< rule > parents = p -> lemmas;
      for( rule& r : lemmas )
         parents. push_back( std::move(r) );
      start = ( p -> start );
      lemmas = std::move( parents );
      p = dynamic_cast< const exdisj_res* > ( & start. getinference( ));
   }
}


auto geometric::exdisj_res::getparents( ) const -> std::vector<rule>
{
   std::vector<rule> parents = { start };
   for( const rule& r : lemmas )
      parents. push_back(r);
   return parents;
}


void geometric::exdisj_res::print( std::ostream& out ) const
{
   if( start. islemma( ))
      throw std::runtime_error( "exdisj_res cannot handle lemmas" ); 
   if( start. isdisjunctive( ))
      out << "disj_res( ";
   if( start. isexistential( ))
      out << "exists_res( ";

   start. printkey( out ); 
   for( auto p = lemmas. begin( ); p != lemmas. end( ); ++ p )
   {
      if( p != lemmas. begin( ))
         out << ", ";
      else
         out << "; ";
      p -> printkey( out );
   }

   out << " )";
}



//////////////////////////////////////////////////////////////////

geometric::varatom geometric::replace( varatom at, variable v1, variable v2 )
{
   for( auto& arg : at )
   {
      if( arg == v1 )
         arg = v2;
   }
   return at; 
}


bool geometric::groundconflict( const groundatom& at1, const groundatom& at2 )
{
   return at1.p == at2.p && 
          ( at1.t & at2.t ) == truthval::empty &&
          equal_arguments( at1, at2 );
}

bool geometric::iscreate( element e, const groundatom& at )
{
   return at.p.iscreate( ) && 
          at.t == truthval::F &&
          at[0] == e;
}

bool geometric::isequalityfrom( element e, const groundatom& at ) 
{
   return at.p.isequality( ) && 
          at.t == truthval::T &&
          at[0] == e && at[1] != e;
}

bool geometric::isequalityinto( element e, const groundatom& at )
{
   return at.p.isequality( ) && 
          at.t == truthval::T &&
          at[1] == e && at[0] != e;
}


size_t 
geometric::find( size_t val, const std::vector<size_t> & vect ) 
{
   size_t j = 0; 
   while( j < vect. size( )) 
   {
      if( val == vect[j] )
         return j;
      ++ j;
   }
   return j;
}


void geometric::transfer( const subst< element > & orig,
                          label lab,
                          const unifier& unif,
                          normalizer& norm,
                          const set< variable > & varsinresult,
                          subst< element > & result )
{
   for( std::pair<variable,element> a : orig )
   {
      a. first = unif. apply( norm, a. first, lab );
      // std::cout << a. first << " -> " << a. second << "\n";
      if( varsinresult. contains( a.first ))
         result. assign( a.first, a.second );
   }
}



geometric::matched< geometric::disjunctive >
geometric::ex_resolve( const matched<existential> & exists,
                       element skolem_constant,
                       const false_instance& inst_lemma ) 
{
#if 1
   std::cout << "existential resolve:\n";
   std::cout << exists << "\n";
   std::cout << "skolem constant " << skolem_constant << "\n";
   std::cout << inst_lemma << "\n\n";
#endif

   variable exvar = exists.r. getvariable( );
   const varatom& exatom = exists.r. getatom( );

   // All free variables of the existential formula must have an 
   // assignment, and this assignment must be not the skolem_constant.

   {
      set< variable > freevars = exists.r. getfreevariables( );
 
      for( variable v : freevars ) 
      {
         auto p = exists. inst. clookup(v);
 
         if( !p )
            throw std::runtime_error( "existential resolution: unassigned "\
                                      "variable in existential formula" );
         if( *p == skolem_constant )
            throw std::runtime_error( "existential resolution: variables "\
                                      "different from the existential "\
                                      "variable cannot be mapped into "\
                                      "the Skolem constant" );
      } 
   }

   {
      auto p = exists. inst. clookup( exvar );
      if(p)
         throw std::runtime_error( "existential resolution: existential "\
                                   "variable has assignment" );

      if( !exatom. contains( exvar ))
         throw std::runtime_error( "existential resolution: the existential "\
                                   "variable does not occur in the "\
                                   "existential atom" );
   }

   // The existential atom cannot be an equality atom. 
   // Create seems to be possible. 

   if( exatom.p. isequality( ))
      throw std::runtime_error( "existential resolution: existential atom "\
                                "is equality" );


   groundatom exatomground = 
                   exists. inst. apply( exatom, exvar, skolem_constant );

   // We can start looking at the lemma:

   if( !inst_lemma.r.islemma( ))
      throw std::runtime_error( "existential resolution: "\
                                "conflict lemma is not a lemma" );

   const auto& lemma = inst_lemma.r.getlemma( ); 

   // Every variable in the lemma must have an assignment: 

   set<variable> freevars_lemma = lemma. getfreevariables( );

   for( variable v : freevars_lemma )
   {
      auto p = inst_lemma.inst.clookup(v);
      if( !p )
         throw std::runtime_error( "existential resolution: unassigned "\
                                      "variable in conflict lemma" );
   }

   // The lemma must not have a conclusion:

   if( lemma. hasactive( ))
   {
      throw std::runtime_error( "existential resolution: conflict lemma "\
                                "has active atom" ); 
   }

   unifier unif;
   label lab_exists = label(1);
   label lab_lemma =  label(2);

   // We start by constructing the most general unifier:
   // Every variable in the lemma that is mapped into the Skolem constant,
   // must be unified with the existential variable.

   for( variable v : freevars_lemma )
   {
      if( *inst_lemma.inst.clookup(v) == skolem_constant )
      {
         unif. unify( exvar, lab_exists, v, lab_lemma );
      }
   }

   // The atoms in lemma, whose ground instances are in conflict 
   // with the ground instance of the existential atom, 
   // must also be unified, because they will resolve. 

   for( const varatom& at : lemma. getpassive( ))
   {
      groundatom inst = inst_lemma.inst.apply(at);
      if( groundconflict( inst, exatomground ))
      {
         unif. unify_arguments( exatom, lab_exists, at, lab_lemma );
      }
   }

   normalizer norm;

   // We can now start constructing the result: 

   disjunctive result( {} );

   // Substitute and copy atoms from the existential formula:

   for( const varatom& at : exists.r. getpassive( ))
   {
      result. insert( unif. apply( norm, at, lab_exists ));
   }

   // Apply the unifier on the existential atom. We need it when we
   // encounter equalities. 

   varatom exatomunified = unif. apply( norm, exatom, lab_exists );   


   for( const varatom& at : lemma. getpassive( ))
   {
      groundatom inst = inst_lemma.inst.apply( at ); 
      varatom unified = unif. apply( norm, at, lab_lemma );
         // We now have ground instance, and unified instance of at.

      if( !inst. contains( skolem_constant ))
      {
         // Atoms who instantiation does not contain the Skolem constant,
         // are copied:

         result. insert( unified ); 
         goto done;
      }

      if( isequalityfrom( skolem_constant, inst ))
      {
         result. insert( replace( exatomunified, unified[0], unified[1] ));
         goto done;
      }

      if( isequalityinto( skolem_constant, inst ))
      {
         result. insert( replace( exatomunified, unified[1], unified[0] )); 
         goto done;
      }

      if( iscreate( skolem_constant, inst ))
         goto done;

      if( groundconflict( inst, exatomground ))
         goto done;  

      throw std::runtime_error( "existential resolution: atom "\
                                "contains Skolem element, but is not "\
                                "resolvable" );
   done:
      ;
   }

   set<variable> freevars_result = result. getfreevariables( );

   subst< element > inst; 
   transfer( exists. inst, lab_exists, unif, norm, 
                freevars_result, inst );
   transfer( inst_lemma. inst, lab_lemma, unif, norm, 
                freevars_result, inst ); 
   
   // We put 0 choices in the false instance, because we have no way  
   // of determining the real number without knowing the interpretation.

   return { result, inst }; 
}


geometric::matched< geometric::lemma > 
geometric::disj_resolve( 
      const matched< disjunctive > & disj, 
      const std::vector<size_t> & extensions, 
      const std::vector< false_instance > & inst_lemmas ) 
{
#if 1
   std::cout << "disjunction resolve:\n";
   std::cout << "disjunction:\n";
   std::cout << disj << "\n";

   std::cout << "extensions: {";
   for( size_t i = 0; i != extensions. size( ); ++ i )
   {
      if( i != 0 )
         std::cout << ", ";
      else
         std::cout << " ";
      std::cout << extensions[i];
   }
   std::cout << " }\n\n";
 
   std::cout << "lemmas:\n";
   for( const auto& lem : inst_lemmas )
   {
      std::cout << lem << "\n";
   }
   std::cout << "\n";
#endif

   // Every variable in the disjunctive formula must have an assignment:

   {
      auto s = disj. getunassigned( );
      if( s. size( )) 
      {
         std::cerr << s << "\n";
         throw std::runtime_error( "disjunctive resolution: there are "\
                           "unassigned variables in the main disjunction" );
      } 
   }

   for( const auto& p : inst_lemmas )
   {
      if( !p.r. islemma( ))
         throw std::runtime_error( "disjunctive resolution: "\
                                   "one of the lemmas is not a lemma" );

      const lemma& lem = p.r.getlemma( );

      set< variable > freevars = lem. getfreevariables( );
      for( variable v : freevars )
      {
         if( !p.inst.clookup(v) )
            throw std::runtime_error( "disjunctive resolution: unassigned "\
                                      "variable in one of the lemmas" );
      }

      if( lem. hasactive( ))
         throw std::runtime_error( "disjunctive resolution: One of the "\
                                   "lemmas has an active atom" );  
   }

   for( size_t i : extensions )
   {
      if( i >= disj.r. nractive( ))
         throw std::runtime_error( "disjunctive:resolution: index "\
                      "of active atom is too large" );
   }

   if( extensions. size( ) < inst_lemmas. size( ))
      throw std::runtime_error( "disjunctive resolution: There are "\
                                "too many lemmas" );

   if( extensions. size( ) > inst_lemmas. size( ) + 1 )
      throw std::runtime_error( "disjunctive resolution: There are "\
                                "too few lemmas" );

   unifier unif;
   label lab_disj = label(-1);

   // In the first pass of the active atoms, we construct the unifier:

   for( size_t j = 0; j < disj.r. nractive( ); ++ j )
   {
      size_t position = find( j, extensions );

      if( position < inst_lemmas. size( )) 
      {
         groundatom inst = disj. inst. apply( disj.r [j] );
         label current_label = label(j);

         const lemma& lem = inst_lemmas[ position ].r. getlemma( );
         for( const varatom& at : lem. getpassive( ))
         {
            groundatom atinst = inst_lemmas[position]. inst. apply( at );
            if( groundconflict( inst, atinst ))
            {
               unif. unify_arguments( disj.r[j], lab_disj, at, current_label );
            }
         }
      }
   }

   normalizer norm;

   // If we have less lemmas than extensions, the last extension goes
   // into the conclusion. 

   lemma result = ( inst_lemmas. size( ) < extensions. size( )) ? 
      lemma( {}, unif. apply( norm, disj.r [ extensions.back( ) ], lab_disj ))  
        :
      lemma( {} );

   // Substitute and copy the passive atoms from the disjunctive formula:

   for( const varatom& at : disj.r. getpassive( ))
   {
      result. insert( unif. apply( norm, at, lab_disj ));
   }
  
   for( size_t j = 0; j < disj.r. nractive( ); ++ j )
   {
      size_t position = find( j, extensions );
      if( position < inst_lemmas. size( ))
      {
         groundatom inst = disj. inst. apply( disj.r[j] );
         label current_label = label(j);

         const lemma& lem = inst_lemmas[ position ]. r. getlemma( );
         for( const varatom& at : lem. getpassive( ))
         {
            groundatom atinst = inst_lemmas[position].inst.apply( at );
            if( !groundconflict( inst, atinst ))
            {
               result. insert( unif. apply( norm, at, current_label )); 
            }
         }
      }
      else
      {
         // This means that disj[j] should become a premiss:

         if( position >= extensions. size())
         {
            result. insert( unif. apply( norm, disj.r[j], lab_disj ));
         }
      }
   }

   set<variable> freevars_result = result. getfreevariables( );

   subst< element > resinst; 
   transfer( disj. inst, lab_disj, unif, norm, freevars_result, resinst );

   for( size_t j = 0; j < disj.r. nractive( ); ++ j )
   {
      label current_label = label(j);

      auto position = find( j, extensions );
      if( position < inst_lemmas. size( ))
      {
         transfer( inst_lemmas[position]. inst, current_label, 
                   unif, norm,
                   freevars_result, resinst ); 
      }
   }

   return { result, resinst };
}


geometric::matched< geometric::lemma > 
geometric::horn_resolve( const matched< lemma > & horn,
                         const false_instance& inst_confl )
{
   std::cout << "lemma resolve:\n";
   std::cout << "horn lemma:\n";
   std::cout << horn << "\n";

   std::cout << "conflict lemma:\n";
   std::cout << inst_confl << "\n";

   // Every variable in the Horn lemma must have an assignment:

   {
      auto s = horn. getunassigned( ); 
      if( s. size( ))  
      {
         std::cout << s << "\n";
         throw std::runtime_error( "Horn resolution: there are unassigned "\
                                   "variables in the Horn lemma" );
      } 
   }

   if( !horn.r. hasactive( ))
   {
      throw std::runtime_error( "Horn resolution: the Horn lemma "\
                                "has no active atom" );
   }

   const varatom& head = horn.r. getactive( );
   groundatom headinst = horn. inst. apply( head );
   std::cout << "head = " << head << "\n";
   std::cout << "headinst = " << headinst << "\n";

   if( !inst_confl.r.islemma( ))
   {
      throw std::runtime_error( "Horn resolution: The conflict lemma "\
                                "is not a lemma" );
   }
 
   const lemma& lem = inst_confl.r.getlemma( );

   {
      set< variable > freevars = lem. getfreevariables( );
      for( variable v : freevars )
      {
         if( !inst_confl.inst.clookup(v) )
            throw std::runtime_error( "Horn resolution: unassigned "\
                                      "variable in the lemma" );
      }
   }

   if( lem.hasactive( ))
   {
      throw std::runtime_error( "Horn resolution: The conflict lemma "\
                                "is not allowed to have active atoms" );
   }

   unifier unif;
   label lab_horn = label(0);
   label lab_lem = label(1); 

   // In the first sweep, we construct the most general unifier.
   // In the second sweep, we do the actual resolution:

   for( const varatom& at : lem. getpassive( ))
   {
      groundatom atinst = inst_confl.inst.apply( at );
      if( groundconflict( headinst, atinst ))
      {
         unif. unify_arguments( head, lab_horn, at, lab_lem );
      } 
   }

   std::cout << unif << "\n";
   normalizer norm;

   lemma result = lemma( {} );

   // Substitute and copy the passive atoms from the Horn lemma:

   for( const varatom& at : horn.r. getpassive( ))
   {
      result. insert( unif. apply( norm, at, lab_horn ));
   }
   
   // Substitute and copy the passive atoms, whose
   // ground instance does not clash with headinst, from the conflict lemma: 

   for( const varatom& at : lem. getpassive( ))
   {
      groundatom atinst = inst_confl.inst.apply( at );
      if( !groundconflict( headinst, atinst ))
      {
         result. insert( unif. apply( norm, at, lab_lem )); 
      }
   }

   set<variable> freevars_result = result. getfreevariables( );

   subst< element > resinst; 
   transfer( horn. inst, lab_horn, unif, norm, freevars_result, resinst );
   transfer( inst_confl.inst, lab_lem, unif, norm, freevars_result, resinst ); 

   return { result, resinst };   
}


geometric::lemma
geometric::makeconflictlemma( const lemma& lem )
{
   lemma res = lemma( {} );
   for( const varatom& at : lem. getpassive( ))
      res. insert( at ); 

   if( lem. hasactive( ))
      res. insert( lem. getactive( ));

   return res;
}


geometric::lemma
geometric::makeconflictlemma( const disjunctive& disj )
{
   lemma res = lemma( {} ); 

   for( const varatom& at : disj. getpassive( ))
      res. insert( at );

   for( const varatom& at : disj. getactive( ))
      res. insert( at );

   return res;
}


geometric::lemma
geometric::makehornlemma( const disjunctive& disj, size_t j )
{
   lemma res = lemma( {}, disj[j] );

   for( const varatom& at : disj. getpassive( ))
      res. insert( at );

   for( size_t k = 0; k != disj. nractive( ); ++ k )
   {
      if( k != j )
         res. insert( disj[k] );
   }

   return res;
}


std::ostream& geometric::operator << ( std::ostream& out, merging m )
{
   out << m. v1 << " == " << m. v2;
   return out;
}


void geometric::mergings::print( std::ostream& out ) const
{
   out << "mergings( ";
   for( auto p = m. begin( ); p != m. end( ); ++ p )
   {
      if( p != m. begin( ))
         out << ", ";
      out << *p; 
   }
   out << "; ";
   parent. printkey( out );
   out << " )";
}


geometric::matched<geometric::lemma>
geometric::merge( const matched<lemma> & lem,
                  const std::vector<merging> & eq )
{
#if 0
   std::cout << "merging " << lem << "\n";
   std::cout << "mergings: ";
   for( auto p = eq. begin( ); p != eq. end( ); ++ p )
      std::cout << *p << "  ";
   std::cout << "\n";
#endif 

   label lab(0);

   unifier unif;
   for( merging m : eq )
      unif. unify( m.v1, lab, m.v2, lab );

   normalizer norm;
  
   matched<lemma> result = 
      matched<lemma> ( 
          lem. r. hasactive( ) ?
             lemma( {}, unif. apply( norm, lem. r. getactive( ), lab )) :
             lemma( {} ),
          { } );

   for( const varatom& at : lem. r. getpassive( ))
   {
      result. r. insert( unif. apply( norm, at, lab ));
   }

   set<variable> freevars_result = result. r. getfreevariables( );
   transfer( lem. inst, lab, unif, norm, freevars_result, result. inst );

   return result;  
}


geometric::matched<geometric::disjunctive>
geometric::merge( const matched<disjunctive> & disj,
                  const std::vector<merging> & eq )
{
#if 1
   std::cout << "merging " << disj << "\n";
   std::cout << "mergings: ";
   for( auto p = eq. begin( ); p != eq. end( ); ++ p )
      std::cout << *p << "  ";
   std::cout << "\n";
#endif

   label lab(0);

   unifier unif;
   for( merging m : eq )
      unif. unify( m.v1, lab, m.v2, lab );

   std::cout << unif << "\n";

   normalizer norm;

   matched<disjunctive> result = { { }, { } };

   for( const varatom& at : disj. r. getpassive( ))
      result. r. insert( unif. apply( norm, at, lab ));
 
   for( const varatom& at : disj. r. getactive( ))
      result. r. insert( unif. apply( norm, at, lab ));

   set<variable> freevars_result = result. r. getfreevariables( );
   transfer( disj. inst, lab, unif, norm, freevars_result, result. inst );

   return result;
}


