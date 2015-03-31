

#include "testing.h"



namespace firstorder
{


   void test_term( )
   {
      std::cout << "testing term\n";

      for( unsigned int k = 0; k < 100000; ++ k )
      {
         function f( "f", 2 );
	 function g( "g", 3 );

         std::cout << f. getarity( ) << "\n";
         std::cout << f. getstring( ) << "\n";

         function cc( "c", 0 );
         function dd( "d", 0 ); 

         std::vector< term > sub;

         term t1 = term( cc, sub );
         term t2 = term( dd, sub );

         sub. push_back(t1); sub. push_back(t2);
         term t = term( f, sub );
         std::cout << t << "\n";

	 sub. clear( );
	 sub. push_back(t);
	 sub. push_back(t);
	 t = term( f, sub );
	 std::cout << t << "\n"; 

	 sub. clear( );
	 sub. push_back(t);
	 sub. push_back(t);
	 sub. push_back(t); 
	 term u = term( g, sub );

	 std::vector< term > v1;
	 std::vector< term > v2;

	 for( unsigned int i = 0; i < 100; ++ i )
         {
            v1. push_back(t);
	    v2. push_back(u);
         }

	 for( unsigned int i = 0; i < 50; ++ i )
         {
            std::cout << v2 [i] << " "; 
            v1 [i] = v2 [i];
         }
      }

   }


   void test_form( )
   {

      for( unsigned int kk = 0; kk != 100000; ++ kk )
      {

         std::list< term > args;
	 function aa = function( "a", 0 );
	 function bb = function( "b", 0 );

	 std::list< term > args1; 
	 args1. push_back( term( aa, args ));
         args1. push_back( term( bb, args ));
         predicate pp = predicate( "p", 2 );
	
	 formula at = formula::make_equals( term( aa, args ),
			                    term( bb, args ));

	 std::cout << at << "\n";

         formula f = formula::make_not( at );

	 std::list< formula > sub;
	 sub. push_back(f); sub. push_back(formula::make_true( )); 
	 sub. push_back(f); 

	 std::list< function > vars; 
	 vars. push_back( function( "a", 0 ));
         vars. push_back( function( "b", 0 ));

         std::cout << formula::make_exists( vars, 
	              formula::make_forall( vars, f )) << "\n";
      }


   }



} 


