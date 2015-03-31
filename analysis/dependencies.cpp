
// Written by Hans de Nivelle, June 2007.


#include "dependencies.h"


namespace analysis
{


   void dependencies::addequation( unsigned int lhs, unsigned int rhs )
   {
      equations. push_back( equation( lhs, rhs ));
   }


   void dependencies::addskolem( unsigned int lhs, 
                                 predicate f,
                                 const std::list< unsigned int > & rhs )
   {
      skolems. push_back( skolem( lhs, f, rhs ));
   }




   void dependencies::reduce( )
   {
      
      bool change = true;
      while( change )
      {
         change = false;

         // Delete equations of form t = t:

         {
            std::list< equation > :: iterator p = equations. begin( );
            while( p != equations. end( ))
            {
               std::list< equation > :: iterator p1 = p;
               ++ p1;

               if( ( p -> lhs ) == ( p -> rhs ))
               {
                  equations. erase(p);
                  change = true; 
               }
               p = p1; 
            }
         }

         for( std::list< equation > :: iterator
                 p1 = equations. begin( );
                 p1 != equations. end( );
                 ++ p1 )
         {
            // If lhs < rhs, then swap equation: 

            if( ( p1 -> lhs ) < ( p1 -> rhs ))
            {
               unsigned int c = ( p1 -> lhs );
               p1 -> lhs = p1 -> rhs;
               p1 -> rhs = c;
               change = true;  
            }

            // Now that *p1 is directed, one can try to simplify other things 
            // with it.

            if( ( p1 -> lhs ) > ( p1 -> rhs ))
            {

               // Equations:
 
               for( std::list< equation > :: iterator
                       p2 = equations. begin( );
                       p2 != equations. end( );
                       ++ p2 )
               {
                  if( p1 != p2 )
                  {
                     if( ( p1 -> lhs ) == ( p2 -> lhs ) &&
                         ( p1 -> rhs ) <= ( p2 -> rhs ))
                     {
                        ( p2 -> lhs ) = ( p1 -> rhs );
                        change = true;
                     }

                     if( ( p1 -> lhs ) == ( p2 -> rhs ) &&
                         ( p1 -> rhs ) <= ( p2 -> lhs ))
                     {
                        ( p2 -> rhs ) = ( p1 -> rhs );
                        change = true;
                     }
                  }
               }

               // Skolem expressions:

               for( std::list< skolem > :: iterator
                       p2 = skolems. begin( );
                       p2 != skolems. end( );
                       ++ p2 )
               {
                  if( ( p2 -> lhs ) == ( p1 -> lhs ))
                  {
                     ( p2 -> lhs ) = ( p1 -> rhs );
                     change = true; 
                  }

                  for( std::list< unsigned int > :: iterator
                          q = p2 -> rhs. begin( );
                          q != p2 -> rhs. end( );
                          ++ q )
                  {
                     if( *q == ( p1 -> lhs ))
                     {
                        *q = ( p1 -> rhs );
                        change = true;
                     }
                  }
               }
            }
         }


         // The final check is for Skolem dependencies of form
         // f(t1...tn) => a,  f(t1...tn) => b, 
         // One of them will be removed, and replaced by a = b. 

         for( std::list< skolem > :: iterator
                 p1 = skolems. begin( );
                 p1 != skolems. end( );
                 ++ p1 )
         {
            std::list< skolem > :: iterator p2 = p1;
            ++ p2;
            while( p2 != skolems. end( ))
            {
               std::list< skolem > :: iterator p2n = p2;
               ++ p2n;

               if( ( p1 -> f ) == ( p2 -> f ) &&
                   ( p1 -> rhs. size( )) == ( p2 -> rhs. size( )))
               {
                  std::list< unsigned int > ::const_iterator 
                                 q1 = p1 -> rhs. begin( ); 
                  std::list< unsigned int > ::const_iterator
                                 q2 = p2 -> rhs. begin( ); 

                  while( q1 != p1 -> rhs. end( ))
                  {
                     if( *q1 != *q2 )
                        goto diff;
                     ++ q1; ++ q2;
                  }

                  equations. push_back( equation( p1 -> lhs, p2 -> lhs ));
                  skolems. erase( p2 );
                  change = true;
               diff:
                  ;
               }
              
               p2 = p2n;  
            }
         }
      }
   }


   // This is enough, because rhs-s are always normalized.

   unsigned int dependencies::normalization( unsigned int x ) const
   {
      for( std::list< equation > :: const_iterator
              p = equations. begin( );
              p != equations. end( );
              ++ p )
      {
         if( x == ( p -> lhs ))
            return ( p -> rhs );
      }
      return x;
   }



   int dependencies::commonrepresentative( 
                             const dependencies& dep1, unsigned int x1,
                             const dependencies& dep2, unsigned int x2 )
   {
      x1 = dep1. normalization(x1);
      x2 = dep2. normalization(x2);

      if( x1 == x2 )
         return x1;

      if( dep2. normalization(x1) == x2 )
         return x1; 
  
      if( dep1. normalization(x2) == x1 )
         return x2;
 
      for( std::list< dependencies::equation > :: const_iterator 
              p = dep1. equations. begin( );
              p != dep1. equations. end( );
              ++ p )
      {
         if( x1 == ( p -> rhs ) && dep2. normalization( p -> lhs ) == x2 )
            return ( p -> lhs );  
      }

      return -1;
   }


   dependencies dependencies::intersection( const dependencies& dep1,
                                            const dependencies& dep2 ) 
   {
      dependencies result;

      for( std::list< equation > :: const_iterator 
              p1 = dep1. equations. begin( );
              p1 != dep1. equations. end( );
              ++ p1 )
      {
         if( dep2. normalization( p1 -> lhs ) == 
             dep2. normalization( p1 -> rhs ))
         {
            result. addequation( p1 -> lhs, p1 -> rhs );
         }

         std::list< equation > :: const_iterator p2 = p1;
         ++ p2;
         while( p2 != dep1. equations. end( ))
         {
            if( ( p1 -> rhs ) == ( p2 -> rhs ) &&
                dep2. normalization( p1 -> lhs ) ==
                dep2. normalization( p2 -> lhs ))
            {
               result. addequation( p1 -> lhs, p2 -> lhs );
            }
            ++ p2; 
         }
      }

      for( std::list< skolem > :: const_iterator
              p1 = dep1. skolems. begin( );
              p1 != dep1. skolems. end( );
              ++ p1 )
      {
         for( std::list< skolem > :: const_iterator
                 p2 = dep2. skolems. begin( );
                 p2 != dep2. skolems. end( );
                 ++ p2 )
         {
            if( ( p1 -> f ) == ( p2 -> f ) &&
                ( p1 -> rhs. size( )) == ( p2 -> rhs. size( )))
            {
               std::list< unsigned int > args;

               std::list< unsigned int > :: const_iterator
                       q1 = p1 -> rhs. begin( );
               std::list< unsigned int > :: const_iterator
                       q2 = p2 -> rhs. begin( ); 

               while( q1 != p1 -> rhs. end( ))
               {
                  int r;
                  if( ( r = commonrepresentative( dep1, *q1, 
                                                  dep2, *q2 )) >= 0 ) 
                  {
                     args. push_back(r);
                  }
                  else
                     goto notsharable; 

                  ++ q1; ++ q2; 
               }

               int l; 
               if( ( l = commonrepresentative( dep1, p1 -> lhs,
                                               dep2, p2 -> lhs )) >= 0 )
               {
                  result. addskolem( l, p1 -> f, args );
               }
            notsharable: 
               ;
            }
         }
      } 
 
      result. reduce( ); 
      return result;
   }

  
   dependencies dependencies::merging( const dependencies& dep1,
                                       const dependencies& dep2 )
   {
      dependencies result = dep1; 

      for( std::list< equation > :: const_iterator
              p = dep2. equations. begin( );
              p != dep2. equations. end( );
              ++ p )
      {
         result. addequation( p -> lhs, p -> rhs );
      }

      for( std::list< skolem > :: const_iterator
              p = dep2. skolems. begin( );
              p != dep2. skolems. end( );
              ++ p )
      {
         result. addskolem( p -> lhs, p -> f, p -> rhs );
      }

      return result; 
   }


   void dependencies::mergewith( const dependencies& dep )
   {
      for( std::list< equation > :: const_iterator
              p = dep. equations. begin( );
              p != dep. equations. end( );
              ++ p )
      {
         addequation( p -> lhs, p -> rhs );
      }

      for( std::list< skolem > :: const_iterator
              p = dep. skolems. begin( );
              p != dep. skolems. end( );
              ++ p )
      {
         addskolem( p -> lhs, p -> f, p -> rhs );
      }
   }
 

   void dependencies::mergewith( const dependencies& dep,
                                 rule::prem_iterator p1,
                                 rule::prem_iterator p2 )
   {

      rule::prem_iterator p3 = p2;
      ++ p3;
         // This is possible because p2 has a contents.
         // firstoccurrence would look only below p2, but not at p2.

      for( std::list< equation > :: const_iterator
              p = dep. equations. begin( );
              p != dep. equations. end( );
              ++ p )
      {
         addequation( firstoccurrence( contents( p -> lhs, *p2 ), p1, p3 ),
                      firstoccurrence( contents( p -> rhs, *p2 ), p1, p3 ));
      }

      
      for( std::list< skolem > :: const_iterator
              p = dep. skolems. begin( );
              p != dep. skolems. end( );
              ++ p )
      {
         std::list< unsigned int > args;
         for( std::list< unsigned int > :: const_iterator
                 q = p -> rhs. begin( );
                 q != p -> rhs. end( );
                 ++ q )
         {
            args. push_back( 
                     firstoccurrence( contents( *q, *p2 ), p1, p3 ));
         }
         addskolem( firstoccurrence( contents( p -> lhs, *p2 ), p1, p3 ),
                    p -> f, args );
      }
   }

 

   bool dependencies::implies( const dependencies& dep ) const
   {
      for( std::list< equation > :: const_iterator
              p = dep. equations. begin( );
              p != dep. equations. end( );
              ++ p )
      {
         if( normalization( p -> lhs ) != normalization( p -> rhs ))
            return false;
      }

      for( std::list< skolem > :: const_iterator
              p1 = dep. skolems. begin( );
              p1 != dep. skolems. end( );
              ++ p1 )
      {
         unsigned int lhs = normalization( p1 -> lhs );
         std::list< unsigned int > args; 

         for( std::list< unsigned int > :: const_iterator
                 q = p1 -> rhs. begin( );
                 q != p1 -> rhs. end( );
                 ++ q )
         {
            args. push_back( normalization( *q ));
         }

         for( std::list< skolem > :: const_iterator
                 p2 = skolems. begin( );
                 p2 != skolems. end( );
                 ++ p2 )
         {
            if( lhs == ( p2 -> lhs ) &&
                ( p1 -> f ) == ( p2 -> f ) &&
                args. size( ) == ( p2 -> rhs. size( )))
            {
               std::list< unsigned int > :: const_iterator 
                              q1 = args. begin( );
               std::list< unsigned int > :: const_iterator 
                              q2 = p2 -> rhs. begin( ); 
               while( q1 != args. end( ))
               {
                  if( *q1 != *q2 )
                     goto distinct;
                  ++ q1; ++ q2;
               }
               goto equal;
            }
         distinct:
            ;
         }
         return false; 
      equal:
         ; 
      }

      return true;
   } 


   std::list< rule > 
   dependencies::axioms( std::vector< predicate > & predicates_base,
                         std::vector< variable > & arguments_base,
                         predicate pred ) const
   {
      std::list< rule > result;

      for( std::list< equation > :: const_iterator
              p = equations. begin( );
              p != equations. end( );
              ++ p )
      {
         variable v; 
         std::vector< variable > args;
         
         for( unsigned int i = 0; i < pred. getarity( ); ++ i )
         {
            args. push_back(v); ++ v; 
         } 
         
         rule r( predicates_base, arguments_base );

         r. addpremisse( varatom( pred, args ));

         ASSERT( ( p -> lhs ) < pred. getarity( ));
         ASSERT( ( p -> rhs ) < pred. getarity( ));

         std::vector< variable > args_diseq;
         args_diseq. push_back( args [ p -> lhs ] );
         args_diseq. push_back( args [ p -> rhs ] );

         r. addpremisse( varatom( predicate::diseq, args_diseq ));
 
         result. push_back(r);
      }

      for( std::list< skolem > :: const_iterator
              p = skolems. begin( );
              p != skolems. end( );
              ++ p )
      {
         std::vector< variable > args1;
         std::vector< variable > args2;
         std::vector< variable > args_diseq;

         bool istautology = false;
            // If one has a Skolem expression of form
            // i = f( ...,i,... ), the result will be a tautology,
            // because the generated axiom will denote: 
            // i depends (among other things) from i.

         variable v;
         for( unsigned int i = 0; i < pred. getarity( ); ++ i )
         {
            args1. push_back(v); ++ v;
         }
 
         for( unsigned int i = 0; i < pred. getarity( ); ++ i )
         {
            args2. push_back(v); ++ v;
         }

         for( std::list< unsigned int > :: const_iterator
                 q = p -> rhs. begin( );
                 q != p -> rhs. end( );
                 ++ q )
         {
            ASSERT( *q < pred. getarity( ));
            if( *q == ( p -> lhs ))
               istautology = true;
 
            args2[ *q ] = args1[ *q ];
         }
 
         ASSERT( ( p -> lhs ) < pred. getarity( ));
         
         args_diseq. push_back( args1[ p -> lhs ] );
         args_diseq. push_back( args2[ p -> lhs ] );

         rule r( predicates_base, arguments_base ); 
         r. addpremisse( varatom( pred, args1 ));
         r. addpremisse( varatom( pred, args2 ));
         r. addpremisse( varatom( predicate::diseq, args_diseq ));

         if( ! istautology )
            result. push_back(r);
      } 
  
      return result;
   }


   dependencies dependencies::readequations( varatom at ) 
   {
      dependencies res;

      unsigned int nr1 = 0;
      for( std::vector< variable > :: const_iterator
              a1 = at. a1;
              a1 != at. a2;
              ++ a1, ++ nr1 )
      {
         bool found = false;
         unsigned int nr2 = nr1; 
         for( std::vector< variable > :: const_iterator
                 a2 = a1;
                 a2 != at. a2 && !found;
                 ++ a2, ++ nr2 )
         {
            if( nr1 < nr2 && *a1 == *a2 ) 
            {
               res. addequation( nr1, nr2 );
               found = true;
            }
         }
      }
      return res;
   }


   int dependencies::firstoccurrence( variable v, varatom at )
   {
      int nr = 0;
      for( std::vector< variable > :: const_iterator
              a = at. a1;
              a != at. a2;
              ++ a, ++ nr )
      {
         if( v == *a )
            return nr;
      }

      return -1;
   }
 
 
   int dependencies::firstoccurrence( variable v,
                                      rule::prem_iterator p1,
                                      rule::prem_iterator p2 )
   {
      int nr = 0;
      for( rule::prem_iterator p = p1; p != p2; ++ p )
      {
         for( std::vector< variable > :: const_iterator
                 a = (*p). a1;
                 a != (*p). a2;
                 ++ a, ++ nr )
         {
            if( v == *a )
               return nr;
         }
      }
      return -1;
   }


   dependencies dependencies::readskolem( variable ex, varatom at )
   {
      dependencies res;

      std::list< unsigned int > args;
      
      unsigned int nr = 0;
      for( std::vector< variable > :: const_iterator
              a = at. a1;
              a != at. a2;
              ++ a, ++ nr )
      {
         if( *a != ex )
            args. push_back(nr);
      }

         // It is essential that we collect them all, even in case there
         // are repeated variables, since otherwise we would have problems
         // when there are two rules of form 
         // exists y. p(x,x,y) and
         // exists y. p(x,y,y). In both cases, we would have
         //    2 = f(0), which would be wrong. 

      int occ = firstoccurrence( ex, at );
      if( occ >= 0 )
         res. addskolem( occ, at. p, args );
      return res;
   }


   variable dependencies::contents( unsigned int x, varatom at )
   {
      ASSERT( x < at. p. getarity( ));

      std::vector< variable > :: const_iterator a = at. a1;
      while( x > 0 )
      {
         ++ a;
         -- x;
      }

      return *a; 
   }

   
   variable dependencies::contents( unsigned int x, 
                                    rule::prem_iterator p1,
                                    rule::prem_iterator p2 )
   {
      rule::prem_iterator p = p1;
      while( p != p2 && x >= (*p). getarity( ))
      {
         x -= (*p). getarity( );
         ++ p;
      }

      ASSERT( p != p2 );

      return contents( x, *p );
   }
      

   void dependencies::replace( unsigned int x1, unsigned int x2 )
   {
      for( std::list< equation > :: iterator
              p = equations. begin( );
              p != equations. end( );
              ++ p )
      {
         if( ( p -> lhs ) == x1 )
            ( p -> lhs ) = x2;
         if( ( p -> rhs ) == x1 )
            ( p -> rhs ) = x2;
      }

      for( std::list< skolem > :: iterator
              p = skolems. begin( );
              p != skolems. end( );
              ++ p )
      {
         if( ( p -> lhs ) == x1 )
            ( p -> lhs ) = x2;
 
         for( std::list< unsigned int > :: iterator
                 q = p -> rhs. begin( ); 
                 q != p -> rhs. end( );
                 ++ q )
         {
            if( *q == x1 )
               *q = x2; 
         }
      }
   }


   void dependencies::project( rule::prem_iterator p1,
                               rule::prem_iterator p2,
                               varatom at )
   {
      for( std::list< equation > :: iterator
              p = equations. begin( );
              p != equations. end( );
         )
      {
         std::list< equation > :: iterator p_next = p;
         ++ p_next;

         int lhs = firstoccurrence( contents( p -> lhs, p1, p2 ), at ); 
         int rhs = firstoccurrence( contents( p -> rhs, p1, p2 ), at ); 

         if( lhs < 0 )
         {
            replace( p -> lhs, p -> rhs ); 
            equations. erase(p); 
         }
         else
         {
            if( rhs < 0 )
            {
               replace( p -> rhs, p -> lhs ); 
               equations. erase(p);
            }
         }

         p = p_next; 
      }

      for( std::list< skolem > :: iterator
              p = skolems. begin( );
              p != skolems. end( );
         )
      {
         std::list< skolem > :: iterator p_next = p;
         ++ p_next;

         int lhs = firstoccurrence( contents( p -> lhs, p1, p2 ), at );
         if( lhs < 0 )
         {
            skolems. erase(p);
            goto erased;
         }

         for( std::list< unsigned int > :: const_iterator
                 q = p -> rhs. begin( );
                 q != p -> rhs. end( );
                 ++ q )
         {
            int rhs = firstoccurrence( contents( *q, p1, p2 ), at );
            if( rhs < 0 )
            {
               skolems. erase(p);
               goto erased;
            }
         }
         
      erased:
         p = p_next; 
      }
   }
   
  
   void dependencies::transfertoconclusion( rule::prem_iterator p1,
                                            rule::prem_iterator p2,
                                            varatom conc )
 
   {
      for( std::list< equation > :: iterator
              p = equations. begin( );
              p != equations. end( );
              ++ p )
      {
         int lhs = firstoccurrence( contents( p -> lhs, p1, p2 ), conc ); 
         int rhs = firstoccurrence( contents( p -> rhs, p1, p2 ), conc ); 
         ASSERT( lhs >= 0 && rhs >= 0 );
 
         ( p -> lhs ) = lhs;
         ( p -> rhs ) = rhs; 
      }

      for( std::list< skolem > :: iterator
              p = skolems. begin( );
              p != skolems. end( );
              ++ p )
      {
         int lhs = firstoccurrence( contents( p -> lhs, p1, p2 ), conc ); 
         ASSERT( lhs >= 0 );
         ( p -> lhs ) = lhs; 
 
         for( std::list< unsigned int > :: iterator
                 q = p -> rhs. begin( );
                 q != p -> rhs. end( );
                 ++ q )
         {
            int x = firstoccurrence( contents( *q, p1, p2 ), conc ); 
            ASSERT( x >= 0 ); 
            *q = x; 
         } 
      }
   }

 
   std::ostream& operator << ( std::ostream& stream, 
                               const dependencies::equation eq )
   {
      stream << '$' << eq. lhs << " = " << '$' << eq. rhs;
      return stream;
   }


   std::ostream& operator << ( std::ostream& stream,
                               const dependencies::skolem& skol )
   {
      stream << '$' << skol. lhs << " = ";
      stream << skol. f << "( ";

      for( std::list< unsigned int > :: const_iterator
              p = skol. rhs. begin( );
              p != skol. rhs. end( );
              ++ p )
      {
         if( p != skol. rhs. begin( ))
            stream << ", "; 
         stream << '$' << *p;
      }
      stream << " )";
      return stream;
   }


   std::ostream& operator << ( std::ostream& stream, const dependencies& dep ) 
   {
      stream << "Dependencies:\n";

      for( std::list< dependencies::equation > :: const_iterator
              p = dep. equations. begin( );
              p != dep. equations. end( );
              ++ p )
      {
         if( p != dep. equations. begin( ))
            stream << ", ";
         
         stream << *p;
      }

      for( std::list< dependencies::skolem > :: const_iterator
              p = dep. skolems. begin( );
              p != dep. skolems. end( );
              ++ p )
      {
         if( p != dep. skolems. begin( ) || dep. equations. size( )) 
            stream << ", ";
         stream << *p; 
      }
 
      stream << "\n";
      return stream;
   }

}



