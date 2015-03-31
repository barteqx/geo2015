
// Written by Hans de Nivelle, Jan 2015.
// Searchstate contains all the ugly methods that
// don't fit anywhere else.

#include "searchstate.h"
#include "matching/basics.h"


namespace geometric
{

   std::ostream& operator << ( std::ostream& out, const searchstate& ss )
   {
      out << "Searchstate( " << ss. starttime << " ):\n\n";

      out << "Permutation Generator:\n";
      out << "   " << ss. generator << "\n";
      out << "\n";

      out << ss.rs << "\n";

      out << ss. intp;

      return out;
   }


   best_false_instances 
   searchstate::findfalseinstances( unsigned int nr, bool onlylemmas ) const
   {
      best_false_instances inst( nr );

      for( const indexedrule& lem : rs. learnt )
      {
         instance_checker check( inst, *this, lem.r );

         subststack<element> ss;
         std::cout << "-------------------------------\n";
         std::cout << intp << "\n";
         std::cout << lem << "\n";
         std::cout << "starting match\n";
         matching::conflictmatch( lem.r.getpassive( ), 0,
                                  intp.atoms, ss, check );
         std::cout << "ended match\n\n";
      }

      // If we found false lemmas, or onlylemmas is true, we stop.

#if 0
      if( inst. size( ) || onlylemmas )
         return inst;
#endif

      for( const indexedrule& init : rs. initials )
      {
         instance_checker check( inst, *this, init.r );

         subststack<element> ss;
         matching::conflictmatch( init.r.getpassive( ), 0,
                                  intp.atoms, ss, check );
      }

      return inst;
   }


   void searchstate::construct_disjunctive_resolvents(
         const matched< disjunctive > & disj,
         const std::vector< size_t > & extended,
         std::vector< false_instance > & chosen,
         const std::vector< best_false_instances > & options,
         const rule& mainparent,
         std::initializer_list< rule > firstsideparents,
         best_false_instances& best ) const 
   {
      size_t level = chosen. size( );
      if( level == options. size( ))
      {
         // We have a complete set of choices, we construct a resolvent:

         matched<lemma> res = 
                    disj_resolve( disj, extended, chosen );
         std::vector< rule > sideparents = firstsideparents;
         for( const false_instance& ff : chosen )
            sideparents. push_back( ff.r );

         false_instance candidate(
            rule( res. r, exdisj_res( mainparent, sideparents )),
            intp. nrchoices( res.r, res. inst ), res. inst,
            intp. getassumptions( res. r, res. inst ));

         best. insert( std::move( candidate ));
      }
      else
      {
         // We choose from options, and continue: 

         for( const false_instance& ff : options[ level ] )
         {
            chosen. push_back( ff );
            construct_disjunctive_resolvents( disj, extended, 
               chosen, options, mainparent, firstsideparents, best );
            chosen. pop_back( );
         }
      }
   }


   void searchstate::learn_from_disjunctive( 
      const matched< disjunctive > & disj, 
      unsigned int maxclosing, 
      const rule& mainparent,
      std::initializer_list< rule > firstsideparents,  
      best_false_instances& best )
   {
      std::cout << "learn_from_disjunctive: " << disj << "\n";
      std::cout << "maxclosing = " << maxclosing << "\n";
      
      std::vector< size_t > extensions; 
      std::vector< best_false_instances > lemmas;
      std::vector< size_t > nolemma; 

      // For each active atom j of disj, there are three possibilities:
      // 1. Immediate conflict. We ignore j.
      // 2. No immediate conflict and we find conflict lemmas.
      //    j goes into extensions, and we append the closing lemmas to lemmas.
      // 3. No immediate extensions, but no conflict lemmas. j goes into
      //    nolemma. At most one j can be without lemma. 

      for( size_t j = 0; j < disj.r.nractive( ); ++ j )
      {
         if( !intp.conclusionconflicts( disj.r, disj. inst, j ))
         {
            // The false instance is irrelevant:

            intp. extend( disj.r, disj. inst,
                    false_instance( disj.r, 1, { }, { } ), j );

            best_false_instances 
               closing = findfalseinstances( maxclosing, true );

            intp. pop_nondeterministic( );

            // We remove the non-conflicts from closing. This probably is not
            // necessary, because findfalseinstances returned only lemmas.

            while( closing. size( ) && closing. back( ). nrchoices != 0 )
               closing. pop_back( );

            if( closing. size( ) == 0 )
            {
               nolemma. push_back(j);
            } 
            else
            {
               extensions. push_back(j); 
               lemmas. push_back( std::move( closing ));
            }
         }
      }

      std::cout << "Closing Lemmas:\n";
      for( size_t i = 0; i < lemmas. size( ); ++ i )
      {
         std::cout << "------------------------------------------------\n";
         std::cout << "choice = " << extensions[i] << "\n";
         std::cout << lemmas[i] << "\n";
      }
      std::cout << "--------------------------------------------------\n";

      if( nolemma. size( ) > 1 )
         throw std::runtime_error( "at most one extension may be "\
                                   "without conflict" );

      if( nolemma. size( ) == 1 )
         extensions. push_back( nolemma[0] );

      std::vector< false_instance > chosen;
         // Used for backtracking in construct_disjunctive_resolvents.
 
      construct_disjunctive_resolvents( 
             disj, extensions, 
             chosen, lemmas, mainparent, firstsideparents,
             best );
   }

  
   void  
   searchstate::learn_from_existential( 
         const false_instance& ex,
         unsigned int maxclosing,
         best_false_instances& best ) 
   {
      std::cout << "learn_from_existential: " << ex << "\n";
      std::cout << "maxclosing: " << maxclosing << "\n";

      intp. extend( ex.r.getexistential( ), ex.inst,
                    false_instance( ex.r, 1, 
                                    { }, { } ), intp. nrelements( ));

         // The false instance is irrelevant.

      element skolem_const = intp. lastelement( );
      best_false_instances closing = findfalseinstances( maxclosing, true ); 
      intp. pop_nondeterministic( );

      // We remove the non-conflicts from closing. This probably is not 
      // necessary, because findfalseinstances returned only lemmas. 

      while( closing. size( ) && closing. back( ). nrchoices != 0 )
         closing. pop_back( );

      if( closing. size( ) == 0 )
         throw std::runtime_error( "there must be closing lemmas" );

      std::cout << "closing = " << closing << "\n";
      
      for( const auto& ff : closing ) 
      {
         matched< disjunctive > 
         candidate = ex_resolve( { ex.r.getexistential( ), ex.inst }, 
                                 skolem_const, ff );

         learn_from_disjunctive( candidate, maxclosing, 
                                 ex.r, { ff.r }, best );
      }

      std::cout << "best = " << best << "\n";
   }


   // It must be check that unwinding goes as deep as possible.  
   // A deeper Horn clause should be preferred over an undeep
   // closure.

   void  searchstate::unwindlemmas( best_false_instances& confl )
   {
   restart:

      // We find the highest level among confl.

      std::cout << "unwinding application\n";
      std::cout << confl << "\n";
    
      size_t level = 0;
      for( const false_instance& f : confl )
      {
         size_t l = intp. conflictlevel( f.r. getlemma( ), f. inst );
         if( l > level )
            level = l;
      }

      std::cout << intp << "\n";
      std::cout << "level = " << level << "\n";

      if( level > intp. atoms_size( ))
         throw std::runtime_error( "conflict lemmas messed up" );

      // We unwind to level:

   removed:
      if( intp. extension_size( ) == 0 )
         return; 

      if( intp. extension_size( ) && 
          intp. lastjustification( ).r. islemma( ) &&
          level < intp. atoms_size( ))
      {
         intp. pop_deterministic( );
         goto removed;
      }

      if( intp. nrguesses( ) && 
          level <= intp. lastguesser( ). at_index )
      {
         intp. pop_nondeterministic( );
         intp. pop_guesser( );
         goto removed; 
      }

      const false_instance& last = intp. lastjustification( );
      if( last.r. islemma( ))
      {
         for( false_instance& ff : confl ) 
         {
            // If f uses current atom, we resolve:

            if( intp.conflictlevel( ff.r.getlemma( ), ff.inst ) == level )
            {
               matched<lemma> res = horn_resolve( 
                  matched<lemma> ( last.r.getlemma( ), last.inst ), ff );
                  
               ff = false_instance(
                       rule( res.r, horn_res( { res.r }, ff.r )),
                       intp. nrchoices( res.r, res. inst ),
                       res. inst, 
                       intp. getassumptions( res.r, res.inst ));

               std::cout << "resolved " << ff << "\n";
               rs. addlearnt( { ff } ); 
            }
         }

         intp. pop_deterministic( ); 
         goto restart; 
      }
      else
      {
         // If we find conflicts that do not use the
         // current guess, we move them to confl2.

         if( intp. nrguesses( ) == 0 )
            throw std::runtime_error( "there should be guesses" );

         best_false_instances confl2( confl. size( ));

         for( false_instance& f : confl )
         {
            if( intp.conflictlevel( f.r. getlemma( ), f.inst ) <=
                intp. lastguesser( ). at_index )
            {
               confl2. insert( std::move( f ));
            }
         }

         // If we have false instances that didn't use current
         // assumption, we just forget about the others,
         // and continue.

         if( confl2. size( ))
         {
            std::cout << "Geo2007f would have said: `Sharpening!'\n";
            confl = std::move( confl2 );

            intp. pop_nondeterministic( );
            intp. pop_guesser( );
            goto restart;
         }
      }
   }


   best_false_instances searchstate::nextguess( )
   {
      std::cout << "nextguess\n";
      std::cout << intp << "\n";

      best_false_instances learnt(2);

      if( intp. nrguesses( ) == 0 )
         throw std::runtime_error( "nextguess: there must be guesses" );

      interpretation::guesser& g = intp. lastguesser( );

      if( intp. atoms_size( ) <= g. at_index )
         throw std::runtime_error( "nextguess: interpretation is messed up" );

      false_instance just = std::move( intp. lastjustification( ));
      intp. pop_nondeterministic( );

      if( just.r.isdisjunctive( ))
      {
         if( g.j + 1 < g. choices. size( ))
         {
            intp. extend( just.r.getdisjunctive( ), just.inst, 
                          std::move( just ), g. choices [ g.j ++ ] );

            return learnt;   // Nothing was learnt today. 
         }

         // We have either 0 or 1 choice left. We are going 
         // to create new lemmas. 

         matched< disjunctive > disj{ just.r.getdisjunctive( ), 
                                      std::move(just.inst) };

         learn_from_disjunctive( disj, 2, disj.r, { }, learnt );
         intp. pop_guesser( ); 
         return learnt;
      }

      if( just.r.isexistential( ))
      {
         // If there are two or more choices remaining, we extend
         // and continue. 
 
         if( g.j + 1 < g. choices. size( ))
         {
            intp. extend( just.r.getexistential( ), just.inst,
                          std::move( just ), g. choices[ g.j ++ ] );

            return learnt;
         }

         // We have either 0 or 1 choice left.
         // We are going to construct new closing lemmas. 

         learn_from_existential( just, 2, learnt );
         intp. pop_guesser( );  
         return learnt; 
      }

      throw std::runtime_error( "nextguess: reached the unreachable" );
   }


   best_false_instances
   searchstate::recheck( best_false_instances inst ) const
   {
      best_false_instances res( inst. size( ));

      for( false_instance& ff : inst )
      {
         if( ff.r.islemma( ))
         {
            const lemma& lem = ff.r.getlemma( );
            if( !intp. conclusionistrue( lem, ff. inst ))
            {
               ff. nrchoices = intp. nrchoices( lem, ff. inst ); 
               res. insert( std::move(ff));
            }
         }

         if( ff.r.isdisjunctive( ))
         {
            const disjunctive& disj = ff.r.getdisjunctive( );
            if( !intp. conclusionistrue( disj, ff. inst ))
            {
               ff. nrchoices = intp. nrchoices( disj, ff. inst );
               res. insert( std::move(ff));
            }
         }

         if( ff.r.isexistential( ))
         {
            const existential& exists = ff.r.getexistential( );
            if( !intp. conclusionistrue( exists, ff. inst ))
            { 
               ff. nrchoices = intp. nrchoices( exists, ff. inst ); 
               res. insert( std::move(ff));
            } 
         }
      }
      return res; 
   }



   bool searchstate::makelemma( false_instance& ff ) const
   {
      std::cout << "makelemma " << ff << "\n";
      if( ff.r.islemma( ) && ff.nrchoices == 0 )
      {
         ff.r = makeconflictlemma( ff.r.getlemma( ));
         intp. getassumptions( ff.r, ff.inst );
         return true;
      }

      if( ff.r.isdisjunctive( ) && ff.nrchoices == 0 )
      {
         ff.r = makeconflictlemma( ff.r.getdisjunctive( )); 
         std::cout << ff << "\n";
         throw std::runtime_error( "test2" );
         return true;
      }

      if( ff.r.isdisjunctive( ) && ff.nrchoices == 1 )
      {
         const disjunctive& disj = ff.r.getdisjunctive( );
         for( size_t j = 0; j != disj. nractive( ); ++ j )
         {
            if( !intp.conclusionconflicts( disj, ff.inst, j ))
            {
               ff.r = makehornlemma( disj, j );
               intp. getassumptions( ff.r, ff.inst );
               return true;
            }
         }
         throw std::runtime_error( "makelemma: unreachable" );
      } 

      return false; 
   }


   bool searchstate::search( unsigned int maxtime )
   {
      starttime. restart( ); 
   
   restart:
      std::cout << "Continuing Search:\n";
      std::cout << *this << "\n";
      
      if( starttime. seconds_passed( ) >= maxtime )
      {
         throw std::runtime_error( "time out" );
      }

      best_false_instances current_false = findfalseinstances( 10, false );

      if( current_false. size( ) == 0 )
      {
         // This means that all rules are happy with all of their instances.

         return true;
      }

      while( current_false. size( ))
      {
         false_instance& ff = current_false. front( );
         
         std::cout << "Processing " << ff << "\n";

         if( ff. nrchoices <= 1 )
            makelemma(ff);

         if( ff. nrchoices == 0 )
         {
            ++ intp. closure_counter;

            while( current_false. back( ). nrchoices != 0 )
               current_false. pop_back( );
 
            unwindlemmas( current_false );
    
            if( intp. nrguesses( ) == 0 )
            {
               return false;
            }
            else
            {
               // We have a guess, by which we can extend: 

               current_false = nextguess( ); 
               if( current_false. size( ))
               {
                  for( const false_instance& ff : current_false )
                  {
                     rs. addlearnt( { ff } ); 
                  }
               }
               goto restart; 
            }
         }

         if( ff. nrchoices == 1 )
         {
            // There are only two possibilities, namely
            // lemma and existential.

            if( ff.r.islemma( ))
            {
               const lemma& lem = ff.r.getlemma( );
               intp. extend( lem, ff. inst, std::move( ff )); 
            }
            else
            {
               // Deterministic extension by existential rule: 

               const existential& exists = ff.r.getexistential( );
           
               auto& g = intp. addguesser( {0} );
               intp. extend( exists, ff. inst, std::move(ff), g.j ++ );
            }
         }
         else
         {
            if( ff.r. isdisjunctive( ))
            {
               std::cout << "Extending non-dterm by disjunctive rule:\n";
  
               // Non-deterministic extension by disjunctive rule.

               const disjunctive& disj = ff.r.getdisjunctive( );

               std::vector< size_t > extensions;
               for( size_t j = 0; j < disj. nractive( ); ++ j )
               {
                  if( !intp. conclusionconflicts( disj, ff.inst, j )) 
                     extensions. push_back(j);
               }

               permutation perm =
               generator. makepermutation( extensions. size( ), &intp. atoms );
               
               extensions = perm( extensions ); 
          
               ff. assumptions. insert( intp. nrguesses( ));
 
               auto& g = intp. addguesser( std::move( extensions ));
               intp. extend( disj, ff.inst, std::move(ff), 
                             g. choices [ g.j ++ ] ); 
            }
            else
            {
               std::cout << "non-deterministic extension by existential rule\n";

               const existential& exists = ff.r.getexistential( );

               std::vector< size_t > extensions;
               for( size_t j = 0; j < intp. nrelements( ); ++ j )
               {
                  if( !intp. conclusionconflicts( exists, ff.inst, j ))
                     extensions. push_back(j);
               }
 
               permutation perm =
               generator. makepermutation( extensions. size( ), &intp. atoms );

               extensions = perm( extensions );

               // The second last choice will be to extend the interpretation.
               // We add this second last choice.

               extensions. push_back( extensions. back( ));
               extensions [ extensions. size( ) - 2 ] = intp. nrelements( ); 
              
               ff. assumptions. insert( intp. nrguesses( ));

               auto& g = intp. addguesser( std::move( extensions ));
               intp. extend( exists, ff.inst, std::move(ff),
                             g. choices [ g.j ++ ] );

            }
         }

         current_false. pop_front( );
         current_false = recheck( std::move( current_false ) ); 
            // Rechecking is necessary, because, once the first
            // false atom has been extended, the second doesn't 
            // need to be false anymore. 
      }

      goto restart; 
   }


   void 
   searchstate::instance_checker::operator( )( const subst< element > & ss )
   {

      if( r. islemma( )) 
      {
         const lemma& lem = r. getlemma( ); 
         if( !state.intp.conclusionistrue( lem, ss ))
         {
            false_instance inst( r, state. intp. nrchoices( lem, ss ),
                                 ss, state. intp. getassumptions( r, ss ));

            best. insert( std::move( inst )); 
         }
         return;
      }

      if( r.isdisjunctive( ))
      {
         const disjunctive& disj = r. getdisjunctive( );
         if( !state.intp.conclusionistrue( disj, ss ))
         {
            false_instance inst( r, state. intp. nrchoices( disj, ss ),
                                 ss, state. intp. getassumptions( r, ss ));

            best. insert( std::move( inst )); 
         }
         return; 
      }

      if( r.isexistential( ))
      {
         const existential& ext = r. getexistential( );
         if( !state.intp.conclusionistrue( ext, ss )) 
         {
            false_instance inst( r, state. intp. nrchoices( ext, ss ),
                                 ss, state. intp. getassumptions( r, ss ));

            best. insert( std::move( inst ));
         }
         return;
      }
 
      // Should be unreachable: 
 
      throw std::runtime_error( "forward reasoning: unknown rule type" ); 
   }

}


