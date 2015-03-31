
// Written by Hans de Nivelle, January 2015.

#include "interpretation.h"
#include "matching/basics.h"


namespace geometric
{


   interpretation::interpretation( const std::vector< groundatom > & start )
      : nrinitials{ start. size( ) },
        closure_counter{0}
   {

      set< element > s;
         // We collect all elements occuring in the atoms of start, so that
         // we can put them in elements.

      for( const auto& at : start )
      {
         atoms. insert( at );

         for( auto p = at. args_cbegin( ); p != at. args_cend( ); ++ p )
            s. insert( *p );
      }

      for( auto p = s. cbegin( ); p != s. cend( ); ++ p )
         elements. push_back( *p );
   }



   size_t interpretation::findcreate( element e ) const
   {
      if( atoms. size( ) == 0 )
         return 0; // There is certainly no create in empty interpretation.

      predicate create = atoms[0].p. getcreatefromsignature( );

      auto r = atoms. find( create, 0, e );
      while( r. size( ))
      {
         size_t i = *(r++);
         if( atoms[i].t == truthval::T )
            return i;
      }
      return atoms. size( );
   }


   size_t interpretation::find( const varatom& at,
                                const subst<element> & subst ) const
   {
      if( at.p. isequality( ))
      {
         if( at.t != truthval::T )
            throw std::runtime_error( "find: equality must have truthval T" );
 
         auto p1 = subst. clookup( at[0] );
         auto p2 = subst. clookup( at[1] );
         if( !p1 || !p2 )
            throw std::runtime_error( "find: all variables must be "\
                                      "instantiated" );
         if( *p1 == *p2 )
            return findcreate( *p1 ); 
         else 
            return atoms. size( );
      }

      std::vector< range< size_t >> ranges =
                  matching::getranges( at, subst, atoms );

      bool b = findcommon( ranges );
      if( b )
      {
         auto groundinst = subst. apply( at );
            // We instantiate only if there is a chance of finding
            // the atom.

         while( b )
         {
            size_t i = * ( ranges[0] ++ );

            // Must have equal truth values, and the arguments must match.
            // Equality of predicate symbols is guaranteed by getranges( ).

            if( groundinst.t == atoms[i].t &&
                equal_arguments( groundinst, atoms[i] ))
            {
               return i;
            }

            b = findcommon( ranges );
         }
      }
      return atoms. size( );    // Not found :-( 
   }


   size_t interpretation::find( variable v, const varatom& at,
                                const subst<element> & subst ) const
   {
      // We lookup v in at:

      size_t a = 0;
      while( a < at. getarity( ) && at[a] != v )
         ++ a;

      if( a == at. getarity( ))
      {
         return find( at, subst );
            // Since v does not occur in at, we just call the other function.
            // Actually, this is not allowed. 
      }

      if( subst.clookup(v))
         throw std::runtime_error( "find(v): variable v is assigned" );

      if( at.p. isequality( ))
      {
         throw std::runtime_error( "find(v): cannot handle equality" );
      }

      std::vector< range< size_t >> ranges =
                  matching::getranges( at, subst, atoms );

      bool b = findcommon( ranges );
      if( b )
      {
         while( b )
         {
            size_t i = * ( ranges[0] ++ );

            if( at.t == atoms[i]. t )
            {
               element e = atoms[i][a];
                  // If atom[i] fits, then atom[i] must have e at position a.

               groundatom groundinst = subst. apply( at, v, e );
 
               // Must have equal truth values, and the arguments must match.
               // Equality of predicate symbols is guaranteed by getranges( ).

               if( equal_arguments( groundinst, atoms[i] ))
               {
                  return i;
               }
            }

            b = findcommon( ranges );
         }
      }
      return atoms. size( );    // Not found :-( 
   }


   size_t interpretation::findconflict( const varatom& at,
                                        const subst<element> & subst ) const
   {
      if( at.p. isequality( ))
      {
         if( at.t != truthval::T )
            throw std::runtime_error( "findconflict: can only handle "\
                                      "equality with truthvalue T" );

         auto p1 = subst. clookup( at[0] );
         auto p2 = subst. clookup( at[1] );

         if( !p1 || !p2 )
            throw std::runtime_error( "all variables must be instantiated" );

         if( *p1 != *p2 )
         {
            // We find the create atoms and return the one with the greater
            // index. If one of the creates does not exist, the result
            // will be atoms. size( ). 
   
            size_t i1 = findcreate( *p1 );
            size_t i2 = findcreate( *p2 );  
         
            return ( i1 > i2 ) ? i1 : i2; 
         }
         return atoms. size( ); 
      }
 
      std::vector< range< size_t >> ranges =
                      matching::getranges( at, subst, atoms );

      bool b = findcommon( ranges );
      if( b )
      {
         auto groundinst = subst. apply( at ); 
            // We instantiate only if there is a chance of finding
            // the atom.

         while( b )
         {
           
            size_t i = * ( ranges[0] ++ );

            // Must have incompatible truth values, and arguments must match.
            // Equality of predicate symbols is guaranteed by getranges( ).

            if( ( groundinst.t & atoms[i].t ) == truthval::empty &&
                  equal_arguments( groundinst, atoms[i] ))
            {
               return i;
            }

            b = findcommon( ranges );
         }
      }
      return atoms. size( ); 
   }


   size_t 
   interpretation::findconflict( const varatom& at,
                                 const subst<element> & subst,
                                 variable v, element e ) const
   {

      if( at.p.isequality( ))
      {
         if( at.t != truthval::T )
            throw std::runtime_error( "findconflict( v := e ): can "\
                                      "only handle equality with "\
                                      "truthvalue T" );

         element e1 = e;
         element e2 = e;

         if( at[0] != v )
         {
            auto p = subst. clookup( at[0] );
            if( !p )
               throw std::runtime_error( "findconflict( v := e ): all "\
                             "variables must be instantiated" );
            e1 = *p;
         }

         if( at[1] != v )
         {
            auto p = subst. clookup( at[1] );
            if( !p )
               throw std::runtime_error( "findconflict( v := e ): all "\
                             "variables must be instantiated" );
            e2 = *p; 
         }

         if( e1 != e2 )
         {
            // We find the create atoms and return the one with the greater
            // index. If one of the creates does not exist, the result
            // will be atoms. size( ).

            size_t i1 = findcreate( e1 );
            size_t i2 = findcreate( e2 );

            return ( i1 > i2 ) ? i1 : i2;
         }
         return atoms. size( );
      }

      std::vector< range< size_t >> ranges =
                      matching::getranges( at, subst, v, e, atoms );

      bool b = findcommon( ranges );
      if( b )
      {
         auto groundinst = subst. apply( at, v, e );
            // We instantiate only if there is a chance of finding
            // the atom.

         while( b )
         {

            size_t i = * ( ranges[0] ++ );

            // Must have incompatible truth values, and arguments must match.
            // Equality of predicate symbols is guaranteed by getranges( ).

            if( ( groundinst.t & atoms[i].t ) == truthval::empty &&
                  equal_arguments( groundinst, atoms[i] ))
            {
               return i;
            }

            b = findcommon( ranges );
         }
      }
      return atoms. size( );
   }


   size_t interpretation::conflictlevel( const lemma& lem, 
                                         const subst<element> & subst ) const
   {
      size_t s = 0;
      for( const varatom& at : lem. getpassive( ) )
      {
         size_t l = findconflict( at, subst );
         if( l >= s )
            s = l + 1;
      }

      if( lem. hasactive( ))
      {
         size_t l = findconflict( lem. getactive( ), subst );
         if( l >= s )
            s = l + 1;
      }

      return s;
   }


   void interpretation::extend( const lemma& lem, 
                                const subst<element> & subst,
                                false_instance&& just )
   {
#if 1
      std::cout << "Extending interpretation by: ";
      std::cout << lem << "\n";
      std::cout << subst << "\n";
#endif

      if( !lem.hasactive( ))
         throw std::runtime_error( "extend( lemma ): lemma "\
                                   "has no active atom" );

      groundatom at = subst. apply( lem. getactive( ));

      atoms. insert( at );
      justifications. push_back( std::move(just) );
   }

   
   void interpretation::extend( const disjunctive& disj,
                                const subst<element> & subst,
                                false_instance&& just, size_t j )
   {
#if 1
      std::cout << "Extending interpretation by: ";
      std::cout << disj << "\n";
      std::cout << subst << "\n";
      std::cout << "choice: " << j << "\n";
#endif

      if( j >= disj. nractive( ))
         throw std::runtime_error( "extend( disj ): choice is too high" );

      groundatom at = subst. apply( disj[j] );

      atoms. insert( at );
      justifications. push_back( std::move(just) );
   }


   void interpretation::extend( const existential& exists,
                                const subst<element> & subst,
                                false_instance&& just, size_t j )
   {
#if 1
      std::cout << "Extending interpretation by: ";
      std::cout << exists << "\n";
      std::cout << subst << "\n";
      std::cout << "choice: " << j << "\n";
#endif

      if( j > elements. size( ))
         throw std::runtime_error( "extend( exists ): choice is too high" );

      variable var = exists. getvariable( );

      // 1. j < elements. size( )   =>  pick elements[j].
      // 2. j == elements. size( ) and elements is empty =>
      //             pick default constructor of element. 
      // 3. j == elements. size( ) and element is non-empty =>
      //             pick successor of elements.back( ). 

      element e = 
         ( j < elements. size( )) ? elements[j] :
            ( elements. size( ) ? 
                 elements. back( ). next( ) : 
                 element( )
            );

      groundatom at = subst. apply( exists. getatom( ), var, e );

      // If e is a new element, we add a create atom and append e
      // to elements: 

      if( j == elements. size( ))
      {
         atoms. insert( groundatom( at.p.getcreatefromsignature( ),
                        truthval::T, { e } ));
         elements. push_back(e); 
         justifications. push_back( just ); 
      } 

      atoms. insert( at );
      justifications. push_back( std::move(just) );
   }


   void interpretation::pop_deterministic( )
   {
      if( justifications. size( ) == 0 ||
          !justifications. back( ). r. islemma( ))
      {
         throw std::runtime_error( "pop_deterministic: "\
                 "there is no deterministic extension to pop" );
      } 

      pop_atom( ); 
   }

   void interpretation::pop_nondeterministic( )
   {
      if( guesses. size( ) == 0 )
         throw std::runtime_error( "pop_nondeterministic: "\
                                   "there are no guessers" );

      size_t at = guesses. back( ). at_index;
   
      while( atoms. size( ) > at )
      {
         if( justifications.back().r.islemma( ))
            throw std::runtime_error( "pop_nondeterministic( ): "\
                                      "cannot pop lemma" );
         pop_atom( );
      }
   }

   interpretation::guesser& 
   interpretation::addguesser( std::vector< size_t > && choices )
   {
      guesses. push_back( { atoms. size( ), std::move(choices), 
                            closure_counter } );

      return guesses. back( );
   }

   void interpretation::pop_guesser( ) 
   {
      if( guesses. size( ) == 0 )
         throw std::runtime_error( "pop_guesser: there is no guesser "\
                                   "to pop" );

      if( atoms. size( ) > guesses. back( ). at_index )
         throw std::runtime_error( "pop_guesser: there are still extensions" );

      guesses. pop_back( );
   }
 
   std::ostream& operator << ( std::ostream& out,
                               const interpretation::guesser& g )
   {
      out << "guesser( " << g.at_index << ", ";
      out << "(";
      for( size_t i = 0; i < g. choices. size( ); ++ i )
      {
         if( i == g.j )
            out << " | ";
         else
         {
            if( i == 0 )
               out << " ";
         else
            out << ", "; 
         }
         out << g.choices[i];
      }

      // This shouldn't happen, but I don't want to be too picky.

      if( g.j == g. choices. size( ))
         out << " |), "; 
      else
         out << " ), "; 
      out << g. closure_counter << ", " << g. time << " )";

      return out;
   }


   set< size_t > 
   interpretation::getassumptions( const rule& r,
                                   const subst<element> & inst ) const
   {
      if( justifications. size( ) + nrinitials != atoms. size( ))
         throw std::runtime_error( "something went wrong" );

      set< size_t > res;
      for( const varatom& at : r. getpassive( ))
      { 
         size_t i = findconflict( at, inst );

         if( i >= atoms. size( ))
            throw std::runtime_error( "getassumptions: unmatched atom "\
                                      "in rule that should be false" );

         // Otherwise, it is initial, and it has no assumptions.

         res |= getassumptions(i); 
      }
      return res; 
   }


   std::ostream& operator << ( std::ostream& out, const interpretation& interp )
   {
      out << "Interpretation:\n\n";
      out << "Elements:\n";
      out << "   {";
      for( unsigned int i = 0; i < interp. elements. size( ); ++ i )
      {
         if(i)
            out << ", ";
         else
            out << " ";
         out << interp.elements[i];
      }
      out << " }\n\n";

      if( interp. justifications. size( ) + interp. nrinitials !=
          interp. atoms. size( ))
      {
         throw std::runtime_error( 
                         "size of initials and justifications do not fit" );
      }

      out << "Guesses:\n";
      for( size_t i = 0; i < interp. guesses. size( ); ++ i )
      {
         out << "   " << i << " : " << interp. guesses[i] << "\n";
      }
      out << "\n";
 
      out << "Atoms:\n";
      for( size_t i = 0; i != interp. atoms. size( ); ++ i )
      {
         out << "   " << i << " : " << interp. atoms[i] << "\n";
      }
      out << "\n";

      out << "Justifications:\n\n";
      for( size_t i = 0; i != interp. justifications. size( ); ++ i )
      {
         out << "atom " << ( i + interp. nrinitials ) << " : "; 
         const false_instance& just = interp. justifications[i];
         out << just;
         out << "\n"; 
      }

      out << "Closure Counter:\n";
      out << "   " << interp. closure_counter << "\n";
      return out;
   }

}

