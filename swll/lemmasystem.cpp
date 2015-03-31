

// Code written by Hans de Nivelle, April 2006.


#include "lemmasystem.h"


namespace swll
{


   std::vector< lemma > *
   lemmasystem::getwatchinglemmas( lemma::atom at ) const
   {
      unsigned int k = at. v. getindex( );
      if( k >= watchmatrix. size( ))
         return 0; 

      std::vector< std::vector< lemma > > & row = watchmatrix [k]; 

      if( at. val >= row. size( ))
         return 0;

      return & row [ at. val ];
   }


   void lemmasystem::clear( )
   {
      watchmatrix. clear( );
   }



   void lemmasystem::insert( const lemma& lem )
   {
      // First we ensure that every atom in lemm could be 
      // watched atom, as far as the allocations in  
      // watchmatrix are concerned. Otherwise we run the risk of
      // reallocations during execution of procedure findfalselemma,
      // which we definitely do not want. 

      for( lemma::atom_iterator
              p = lem. atoms_begin( );
	      p != lem. atoms_end( );
	      ++ p )
      {
         variable v = p -> v;
	 unsigned int k = v. getindex( ); 
	 unsigned int x = p -> val;

         if( k >= watchmatrix. size( ))
	 {
	    watchmatrix. insert( 
			    watchmatrix. end( ),
                            1 + k - watchmatrix. size( ),
                            std::vector< std::vector< lemma > > ( ));
         }

         std::vector< std::vector< lemma > > & row = watchmatrix [k];

         if( x >= row. size( ))
         {
	    row. insert( row. end( ), 1 + x - row. size( ),
	                 std::vector< lemma > ( ));
         }
      }

      // The allocation of watchmatrix is now complete. 
      // Next we find a false atom at in lem, (it is precondition that 
      // there exists one) and insert lem with at as watched atom.

      const lemma::atom* at = findfalseatom( lem );
      ASSERT( at );

      getwatchinglemmas( *at ) -> push_back( lem );
   }


   void lemmasystem::insert( const lemma& lem, lemma::atom at ) const 
   {
      std::vector< lemma > * p = getwatchinglemmas( at ); 
      ASSERT(p); 
      p -> push_back( lem ); 
   }


   const lemma* lemmasystem::findfalselemma( unsigned int base ) const
   {
      const lemma* best = 0;

      while( base < subst. size( ) )
      {
         variable v = subst. nthvar( base );
         lemma::atom at = lemma::atom( v, * ( subst. lookup( v ) ) );
	 
	 // at := subst [ base ].

	 std::vector< lemma > * watching = getwatchinglemmas( at );
	 if( watching )
         {
            std::vector< lemma > & watching2 = *watching;

	    // std::cout << "size = " << watching2.size( ) << "\n";

	    unsigned int i = 0;
	    while( i < watching2. size( ) )
            {
               const lemma::atom* otherat = findfalseatom( watching2 [i] );
               if( otherat )
               {
                  // watching2 [i] has another false atom, and 
	          // we make that atom the watched atom.

                  insert( watching2 [i], *otherat );
	          if( i < watching2. size( ))
	             watching2 [i] = watching2. back( );
	          watching2. pop_back( );
               }
               else
               {
                  // The lemma is really really false.
                  // If it is better than best we make it best.

                  if( best == 0 || ispreferredover( watching2 [i], *best ))
                     best = & watching2 [i]; 

	          ++ i; 
               }
            }
         }

	 ++ base; 
      }

      return best; 
   }


   unsigned int lemmasystem::nrlemmas( ) const
   {
      unsigned int nr = 0;
      for( unsigned int i = 0; i < watchmatrix. size( ); ++ i )
         for( unsigned int j = 0; j < watchmatrix [i]. size( ); ++ j )
         {
            nr += watchmatrix [i] [j]. size( );
         }
      return nr;
   }


   bool lemmasystem::subsumes( const lemma& lem ) const
   {
      for( unsigned int i = 0; i < watchmatrix. size( ); ++ i )
         for( unsigned j = 0; j < watchmatrix [i]. size( ); ++ j )
            for( unsigned int k = 0; k < watchmatrix [i] [j]. size( ); ++ k )
            {
               if( watchmatrix [i] [j] [k]. subsumes( lem ))
                  return true;
            }
      return false;
   }


   void lemmasystem::removebacksubsumed( const lemma& lem ) const
   {
      for( unsigned int i = 0; i < watchmatrix. size( ); ++ i )
         for( unsigned int j = 0; j < watchmatrix [i]. size( ); ++ j )
            for( unsigned int k = 0; k < watchmatrix [i] [j]. size( ); ++ k )
            {
                if( lem. subsumes( watchmatrix [i][j][k] ))
                {
                   std::cout << "lemma " << watchmatrix [i] [j] [k] << " ";
		   std::cout << "is subsumed by " << lem << "\n";
                }
            }
   }


   bool lemmasystem::ispreferredover( const lemma& lem1,
		                      const lemma& lem2 ) const 
   {

#if 1
      if( lem1. size( ) < lem2. size( )) return true;
      if( lem1. size( ) > lem2. size( )) return false;
#endif

#if 0
      std::cout << "ispreferredover \n";
      std::cout << "   " << lem1 << "\n";
      std::cout << "   " << lem2 << "\n";
      std::cout << "   " << subst << "\n\n";
#endif

      for( unsigned int i = 0; i < subst. size( ); ++ i ) 
      {

	 variable v = subst. nthvar(i);

	 if( lem1. contains(v) && ! lem2. contains(v) )
            return false;
	 if( ! lem1. contains(v) && lem2. contains(v) )
            return true;

      }
      return false;
   }


   std::ostream& operator << ( std::ostream& stream, 
		               const lemmasystem& syst )
   {
      stream << "Lemma System: \n\n";
      stream << syst. subst << "\n\n"; 

      for( unsigned int i = 0; i < syst. watchmatrix. size( ); ++ i )
      {
         for( unsigned int j = 0; j < syst. watchmatrix [i]. size( ); ++ j )
         {
            if( syst. watchmatrix [i] [j]. size( ) )
            {
               stream << "Watching: ";
	       stream << "( " << variable::constructfromunsigned(i);
	       stream << " := ";
	       stream << j << " ): \n";

	       for( unsigned int k = 0; 
		    k < syst. watchmatrix [i] [j]. size( );
		    ++ k )
               {
                  stream << "   " << syst. watchmatrix [i] [j] [k] << "\n";
               }
            }
         }
      }
      stream << "\n"; 
      return stream;
   }











   // This uses 7mBytes and takes 86 seconds on mpiat2301. 


   void testlemmasystem( )
   {
      std::cout << "testing lemma system!\n\n";

      variable v0;
      variable v1 = v0; v1 ++ ;
      variable v2 = v1; v2 ++ ;
      variable v3 = v2; v3 ++ ;
      variable v4 = v3; v4 ++ ;
      variable v5 = v4; v5 ++ ;
      variable v6 = v5; v6 ++ ;
      variable v7 = v6; v7 ++ ;
      variable v8 = v7; v8 ++ ;
      variable v9 = v8; v9 ++ ;
      variable v10 = v9; v10 ++ ; 

      swll::unsignedsubst subst;
      swll::lemmasystem syst( subst );
      std::vector< swll::lemma::atom > base;

      for( unsigned int i0 = 0; i0 < 4; ++ i0 )
      for( unsigned int i1 = 0; i1 < 4; ++ i1 )
      for( unsigned int i2 = 0; i2 < 4; ++ i2 )
      for( unsigned int i3 = 0; i3 < 4; ++ i3 )
      for( unsigned int i4 = 0; i4 < 4; ++ i4 )
      for( unsigned int i5 = 0; i5 < 4; ++ i5 )
      for( unsigned int i6 = 0; i6 < 4; ++ i6 )
      for( unsigned int i7 = 0; i7 < 4; ++ i7 )
      {
         std::list< swll::lemma::atom > at;

         at. push_back( swll::lemma::atom(v0,i0));
         at. push_back( swll::lemma::atom(v1,i1));
	 at. push_back( swll::lemma::atom(v2,i2)); 
	 at. push_back( swll::lemma::atom(v3,i3));
	 at. push_back( swll::lemma::atom(v4,i4));
	 at. push_back( swll::lemma::atom(v5,i5));
	 at. push_back( swll::lemma::atom(v6,i6));
	 at. push_back( swll::lemma::atom(v7,i7));

         swll::lemma lem = swll::lemma( base, at ); 

         syst. insert(lem);
      }

      std::cout << syst << "\n";

      for( unsigned int kk = 0; kk < 1000; ++ kk ) 
      for( unsigned int i0 = 0; i0 < 4; ++ i0 )
      {
         subst. assign( v0, i0 );
         ASSERT( syst. findfalselemma(0) == 0 );

	 for( unsigned int i1 = 0; i1 < 4; ++ i1 )
         {
            subst. assign( v1, i1 );
	    ASSERT( syst. findfalselemma(1) == 0 );

            for( unsigned int i2 = 0; i2 < 4; ++ i2 )
            {
               subst. assign( v2, i2 );
	       ASSERT( syst. findfalselemma(2) == 0 );
	       for( unsigned int i3 = 0; i3 < 4; ++ i3 )
               {
                  subst. assign( v3, i3 );
		  ASSERT( syst. findfalselemma(3) == 0 );
		  for( unsigned int i4 = 0; i4 < 4; ++ i4 )
                  {
                     subst. assign( v4, i4 );
		     ASSERT( syst. findfalselemma(4) == 0 );
		     for( unsigned int i5 = 0; i5 < 4; ++ i5 )
                     {
                        subst. assign( v5, i5 );
			ASSERT( syst. findfalselemma(5) == 0 );
			for( unsigned int i6 = 0; i6 < 4; ++ i6 )
                        {
                           subst. assign( v6, i6 );
			   ASSERT( syst. findfalselemma(6) == 0 );
			   for( unsigned int i7 = 0; i7 < 4; ++ i7 )
                           {
                              subst. assign( v7, i7 );
			      const lemma* res = syst. findfalselemma(7);
			      ASSERT( res );
			      ASSERT( syst. isfalse( *res )); 
                              
			      subst. backtrack(7); 
                           }
			   subst. backtrack(6); 
                        }
			subst. backtrack(5); 
                     }
		     subst. backtrack(4); 
                  }
		  subst. backtrack(3); 
               }
	       subst. backtrack(2); 
            }
            subst. backtrack(1);
         }
	 subst. backtrack(0);
      }

      std::cout << syst << "\n";
   }
}



