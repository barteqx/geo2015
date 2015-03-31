

// Code written by Hans de Nivelle, July 2006.

 
#include "antiskolemization.h"
#include "bindingstack.h"
#include <cstdlib>


namespace firstorder
{
   

   bool antiskolemization::functionreplacement::contains( function f ) const 
   {
      std::map< function, predicate > :: const_iterator p = repr. find(f);
      if( p != repr. end( ))
         return true; 
      else
	 return false;
   }


   void antiskolemization::functionreplacement::ensure( 
		                               function f, sequent& seq )
   {
      std::map< function, predicate > :: const_iterator p = repr. find(f);
      if( p == repr. end( ))
      {
         predicate pred = seq. freshfunctionpredicate(f);
	 repr. insert( std::pair< function,predicate > ( f, pred ));
      }
   }


   predicate antiskolemization::functionreplacement::lookup( function f ) const
   {
      std::map< function, predicate > :: const_iterator p = repr. find(f);
      ASSERT( p != repr. end( ));
      return ( p -> second );
   }


   namespace
   {

      std::string converttostring( unsigned int x )
      {
         if( x >= 10 )
	 {
	    char c = ( x % 10 ) + '0';
	    x = x / 10;
	    std::string res = converttostring(x);
            res += c;
            return res;
         }
         else
         {
	    std::string res;
	    char c = x + '0';
	    res += c;
	    return res;
         }
      }
   } 



   formula 
   antiskolemization::functionreplacement::seriality_axiom( predicate p )
   {
      ASSERT( p. getarity( ) > 0 );

      std::list< term > args; 
      std::list< function > xxxx;
      std::list< function > yyyy;

      // Since quantified variables and predicates live in 
      // different namespaces, we need not be careful about
      // freshness.

      for( unsigned int i = 0; i < p. getarity( ) - 1; ++ i )
      {
         function f = 
            function( std::string( "xxxx" ) + converttostring(i), 0 );

	 xxxx. push_back( f ); 
         args. push_back( term( f, std::list< term > ( )));
      }
	 
      yyyy. push_back( function( std::string( "yyyy" ), 0 ));
      args. push_back( term( yyyy. front( ), std::list< term > ( )));

      formula f = formula::make_atom( p, args );

      f = formula::make_exists( yyyy, f );
      if( xxxx. size( )) 
         f = formula::make_forall( xxxx, f );
      return f;
   }


   void 
   antiskolemization::functionreplacement::addseriality_axioms( sequent& seq )
   const 
   {
      for( std::map< function, predicate > :: const_iterator
              p = repr. begin( );
	      p != repr. end( );
	      ++ p )
      {
         seq. formulas. push_back( seriality_axiom( p -> second ));
      }
   }


   std::ostream& 
   antiskolemization::operator << ( 
		          std::ostream& stream,
		          const antiskolemization::functionreplacement& f )
   {
      for( std::map< function, predicate > :: const_iterator 
              p = f. repr. begin( );
	      p != f. repr. end( );
	      ++ p )
      {
         if( p != f. repr. begin( )) 
            stream << ", ";

	 stream << ( p -> first ) << " -> " << ( p -> second );
      }
      stream << "\n";
      return stream; 
   }




   namespace
   {


      void addfunctions( antiskolemization::functionreplacement& repl, 
		         bindingstack & bound,
		         sequent& seq, term t )
      {
	 function f = t. getfunction( );

	 if( f. getarity( ) != 0 || ! bound. isbound(f) ) 
            repl. ensure( f, seq );

	 for( std::list< term > :: const_iterator
                 p = t. subterms_begin( );
		 p != t. subterms_end( );
		 ++ p )
         {
            addfunctions( repl, bound, seq, *p );
         }
      }



      void addfunctions( antiskolemization::functionreplacement& repl,
			 bindingstack & bound,
			 sequent& seq, formula f )
      {
         switch( f. getoperator( ))
         {
	 case fol_false: 
	 case fol_true:
         case fol_not: 
         case fol_and:
         case fol_or:
         case fol_implies:
         case fol_equiv: 
            for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               addfunctions( repl, bound, seq, *p );
            }
	 return;

         case fol_exists:
         case fol_forall:
            {
               formula sub = * f. subformulas_begin( );

	       for( std::list< function > :: const_iterator
                       v = f. variables_begin( );
		       v != f. variables_end( );
		       ++ v )
               {
                  bound. bind( *v );
               }
	       addfunctions( repl, bound, seq, sub );

	       for( std::list< function > :: const_iterator
                       v = f. variables_begin( );
		       v != f. variables_end( );
		       ++ v )
               {
                  bound. release( *v );
               }
            }
            return;

         case fol_equals:
         case fol_atom:
	    for( std::list< term > :: const_iterator
                    p = f. subterms_begin( );
		    p != f. subterms_end( );
		    ++ p )
            {
               addfunctions( repl, bound, seq, *p );
            }
	    return;

         default:
	    ASSERT( false ); exit(0);
         }
      }

   } 



   antiskolemization::functionreplacement 
   antiskolemization::functionlist( sequent& seq )
   {
      functionreplacement res;
      bindingstack bound;

      for( std::list< formula > :: const_iterator
              p = seq. formulas. begin( );
	      p != seq. formulas. end( );
	      ++ p )
      {
         addfunctions( res, bound, seq, *p );
      }
      return res;
   }




   /////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////


   // Ensure that termreplacement specifies a defining variable
   // for t at the required level. 

   term antiskolemization::termreplacement::lookup( term t, sequent& seq )
   {

      // First we recurse, so that the direct subterms are defined  
      // at their required levels.

      for( std::list< term > :: const_iterator
              p = t. subterms_begin( );
	      p != t. subterms_end( );
	      ++ p )
      {
         (void) lookup( *p, seq );
      }

      // We compute the level that t must have. 

      int requiredlevel = -1;
      if( t. getfunction( ). getarity( ) != 0 )
      {
         // For a compound term, it is the maximal level among
	 // the direct subterms:

         for( std::list< term > :: const_iterator
                 p = t. subterms_begin( );
		 p != t. subterms_end( );
		 ++ p )
         {
            const std::list< levelvar > & val = vars [ *p ];
	    ASSERT( val. size( ) != 0 );
	    if( val. front( ). level > requiredlevel )
               requiredlevel = val. front( ). level;
         }
      }
      else
      {
         // For a term without subterms, we look up the highest 
	 // definition, if there is one. Otherwise, we are
	 // satisfied with -1.

         std::list< levelvar > & val = vars [t];
	 if( val. size( ) != 0 )
            requiredlevel = val. front( ). level;
      }

      std::list< levelvar > & val = vars [t];

      if( val. size( ) == 0 || val. front( ). level < requiredlevel )
      {
         // It has no definition at all, or not at the required
	 // level. We create a definition. 

         function fresh = seq. freshtermvariable( ); 

         val. push_front( levelvar( requiredlevel, fresh ));

	 if( requiredlevel == -1 )
            toplevel. push_back( t );
	 else
         {
            ASSERT( stack. size( ) > requiredlevel );
	    stack [ requiredlevel ]. push_back( t );
	 }
	 return term( fresh, std::list< term > ( ));
      }
      else
      {
         // It has a definition at the required level.

         ASSERT( val. front( ). level == requiredlevel ); 
         return term( val. front( ). var, std::list< term > ( ));
      }
   }


   // Assign variable v to itself at the highest
   // level. This function is used for quantified
   // variables. 

   void antiskolemization::termreplacement::addquantifiedvar( function v )
   {
      ASSERT( v. getarity( ) == 0 ); 
      ASSERT( stack. size( ) > 0 );
         
      std::list< term > args; 
      term t = term( v, args );

      stack. back( ). push_back(t);

      vars [t]. push_front( levelvar( stack. size( ) - 1, v ));
   }


   // Add a level:

   void antiskolemization::termreplacement::addlevel( )
   {
      stack. push_back( std::list< term > ( ));
   }


   // Remove the last level:

   void antiskolemization::termreplacement::removelastlevel( ) 
   {
      ASSERT( stack. size( ) > 0 );

      std::list< term > & last = stack. back( );

      for( std::list< term > :: const_iterator
              p = last. begin( );
	      p != last. end( );
	      ++ p )
      {
         std::list< levelvar > & occ = vars [ *p ];
	 ASSERT( occ. front( ). level == stack. size( ) - 1 );
         occ. pop_front( );

	 if( occ. size( ) == 0 )
            vars. erase( *p );
               // If the list became empty, we remove the entry 
	       // completely.
      }

      stack. pop_back( ); 
   }


      
   formula antiskolemization::termreplacement::defining_atom( 
      const functionreplacement& repl, 
      sequent& seq, 
      term t )
   {

      // We look up the arguments:

      std::list< term > args;
      for( std::list< term > :: const_iterator
              p = t. subterms_begin( );
	      p != t. subterms_end( );
	      ++ p )
      {
         args. push_back( lookup( *p, seq )); 
      }

      // We look up the term itself:

      args. push_back( lookup( t, seq )); 

      return formula::make_atom( repl. lookup( t. getfunction( ) ), args ); 
   }
         

   formula antiskolemization::termreplacement::add_defining_atoms( 
      const functionreplacement& repl,
      sequent& seq,
      formula f )
   {
      std::list< function > definingvars;
      std::list< formula > definingatoms;

      // We collect the the defining atoms and the defining variables
      // for terms that have a definition on the last level:

      if( stack. size( ) != 0 )
      {
         for( std::list< term > :: const_iterator
                 p = lastlevel_begin( );
		 p != lastlevel_end( );
		 ++ p )
         {
            term var = lookup( *p, seq );
	    if( var != *p )
            {
               definingatoms. push_back( defining_atom( repl, seq, *p ));
               definingvars. push_back( var. getfunction( ));
            }
         }
      }
      else
      {
         for( std::list< term > :: const_iterator
                 p = toplevel_begin( );
		 p != toplevel_end( );
		 ++ p )
         {
            term var = lookup( *p, seq );
	    if( var != *p )
            {
               definingatoms. push_back( defining_atom( repl, seq, *p ));
	       definingvars. push_back( var. getfunction( ));
            }
         }
      }

      // We negate the defining atoms:

      for( std::list< formula > :: iterator 
              p = definingatoms. begin( );
	      p != definingatoms. end( );
	      ++ p )
      {
         *p = formula::make_not( *p );
      }

      // We build a big disjunction, with f as its last member.

      definingatoms. push_back(f);
      f = formula::make_or( definingatoms );

      // Finally, we add a universal quantification over the
      // defining variables:

      return formula::make_forall( definingvars, f );
   }


   std::list< term > :: const_iterator 
   antiskolemization::termreplacement::lastlevel_begin( ) const
   {
      return stack. back( ). begin( );
   }

   std::list< term > :: const_iterator 
   antiskolemization::termreplacement::lastlevel_end( ) const
   {
      return stack. back( ). end( );
   }

   std::list< term > :: const_iterator 
   antiskolemization::termreplacement::toplevel_begin( ) const
   {
      return toplevel. begin( );
   }

   std::list< term > :: const_iterator 
   antiskolemization::termreplacement::toplevel_end( ) const
   {
      return toplevel. end( );
   }



   std::ostream& antiskolemization::operator << ( std::ostream& stream,
		                                  const termreplacement& repl )
   {

      stream << "Termreplacement:\n";

      for( std::map< term, std::list< levelvar > > :: const_iterator
              p = repl. vars. begin( );
	      p != repl. vars. end( );
	      ++ p )
      {
         stream << ( p -> first ) << "        ";
	 for( std::list< levelvar > :: const_iterator
                 q = p -> second. begin( );
		 q != p -> second. end( );
		 ++ q )
         {
            if( q != ( p -> second. begin( )))
               stream << ", "; 
            stream << ( q -> level ) << ":" << ( q -> var );
         }
	 stream << "\n";
      }

      stream << "\n\n";

      stream << "level -1:  ";
      for( std::list< term > :: const_iterator 
              p = repl. toplevel. begin( );
	      p != repl. toplevel. end( );
	      ++ p )
      {
         if( p != repl. toplevel. begin( ))
            stream << ",  ";
	 stream << *p;
      }
      stream << "\n";

      unsigned int lev = 0; 
      for( std::vector< std::list< term > > :: const_iterator
              p = repl. stack. begin( );
	      p != repl. stack. end( );
	      ++ p, ++ lev )
      {
         stream << "level  " << lev << ":  ";
	 for( std::list< term > :: const_iterator 
                 q = p -> begin( );
		 q != p -> end( );
		 ++ q )
         {
            if( q != p -> begin( ))
               stream << ",  ";
	    stream << *q;
         }
	 stream << "\n";
      }

      stream << "\n";

      return stream; 
   }



   namespace
   {

      // f must be in negation normal form.

      formula antiskol( antiskolemization::functionreplacement& fr,
			antiskolemization::termreplacement& tr,  
	                sequent& seq,
	                formula f )
      {

         switch( f. getoperator( ))
         {
         case fol_false:
            return formula::make_false( );
         case fol_true:
            return formula::make_true( );

         case fol_and:
         case fol_or:
            {
               std::list< formula > args;
	       for( std::list< formula > :: const_iterator
                       p = f. subformulas_begin( );
		       p != f. subformulas_end( );
		       ++ p )
               {
                  args. push_back( antiskol( fr, tr, seq, *p ));
               }

	       if( f. getoperator( ) == fol_and )
                  return formula::make_and( args );
	       else	
                  return formula::make_or( args );
            }

         case fol_forall:
         case fol_exists:
            {
               tr. addlevel( );

	       // Insert the quantified variables into tr. 

               for( std::list< function > :: const_iterator
                       v = f. variables_begin( );
		       v != f. variables_end( );
		       ++ v )
               {
                  tr. addquantifiedvar( *v ); 
		     // In case a variable is repeated, it is inserted
		     // twice. That is no problem. 
               }

	       formula body = antiskol( fr, tr, seq, *f. subformulas_begin( ));

	       body = tr. add_defining_atoms( fr, seq, body );
	
	       tr. removelastlevel( );

	       std::list< function > varlist; 
	       for( std::list< function > :: const_iterator
                       v = f. variables_begin( );
		       v != f. variables_end( );
		       ++ v )
               {
                  varlist. push_back( *v );
               }

	       if( f. getoperator( ) == fol_forall )
                  return formula::make_forall( varlist, body );
               else
                  return formula::make_exists( varlist, body ); 
            }
         case fol_atom:
	    {
               std::list< term > args;
	       for( std::list< term > :: const_iterator
                       p = f. subterms_begin( );
		       p != f. subterms_end( );
		       ++ p )
               {
                  args. push_back( tr. lookup( *p, seq ));
               }

               return formula::make_atom( f. getpredicate( ), args );
	    }
         case fol_equals:
            {
               std::list< term > :: const_iterator p = f. subterms_begin( ); 
               term t1 = tr. lookup( * ( p ++ ), seq );
	       term t2 = tr. lookup( * p, seq );
	       return formula::make_equals( t1, t2 );
            }

         case fol_not:
            {
               formula sub = * f. subformulas_begin( );
	       if( sub. getoperator( ) != fol_equals &&
                  sub. getoperator( ) != fol_atom )
               {
		  std::cout << "Formula is not in NNF\n";
		  ASSERT( false ); exit(0);
               }
	       sub = antiskol( fr, tr, seq, sub );
	       return formula::make_not( sub );
            }

         default:
	    std::cout << "Formula not in NNF\n"; 
	    ASSERT( false ); exit(0);
         }
      }

   } 



   void antiskolemization::antiskolemize( sequent& seq )
   {
      functionreplacement funcrepl = functionlist( seq );

#if 0
      std::cout << "Replacing Functions: \n"; 
      std::cout << "  " << funcrepl << "\n";
#endif

      termreplacement termrepl;

      for( std::list< formula > :: iterator
              p = seq. formulas. begin( );
	      p != seq. formulas. end( );
	      ++ p )
      {
         formula f = *p;

	 f = antiskol( funcrepl, termrepl, seq, f );
	 f = termrepl. add_defining_atoms( funcrepl, seq, f ); 

	 *p = f;
      }

      funcrepl. addseriality_axioms( seq );

#if 0
      std::cout << "result of antiskolemization:\n";
      std::cout << seq << "\n";

      std::cout << termrepl << "\n";
#endif

   }



} 



