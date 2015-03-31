

// Code written by Hans de Nivelle, November 2005. 


#include "varsubst.h"


std::ostream& operator << ( std::ostream& stream,
		            const varsubst& s )
{
   stream << "Variable Substitution: \n\n";
   for( std::vector< varsubst::assignment > :: const_iterator 
	   p = s. repr. begin( );
           p !=  s. repr. end( );
	   ++ p )
   {
      stream << "(" << ( p -> from_lab ) << "," << ( p -> from_var ) << ")";
      stream << " := ";
      stream << "(" << ( p -> into_lab ) << "," << ( p -> into_var ) << ")";
      stream << "\n"; 
   }
   stream << "\n";
   return stream;
}



bool varsubst::unify( label lab1, varatom va1, label lab2, varatom va2 )
{
   if( va1. p != va2. p ) 
      return false;
   
   std::vector< variable > :: const_iterator p1 = va1. a1;
   std::vector< variable > :: const_iterator p2 = va2. a1;

   while( p1 != va1. a2 )
   {
      label l1 = lab1;
      variable v1 = *p1;
      normalize( l1, v1 );

      label l2 = lab2;
      variable v2 = *p2; 
      normalize( l2, v2 );

      if( v1 != v2 || l1 != l2 )
         assign( l1, v1, l2, v2 );

      ++ p1;
      ++ p2; 
   }
   return true;
}


varatom varsubst::apply_on( normalizer& n, label lab, varatom va ) const 
{

   static std::vector< variable > base;
   base. clear( );

   for( std::vector< variable > :: const_iterator 
	   p = va. a1;
           p != va. a2;
           ++ p )
   {
      label l = lab;
      variable v = *p;
      normalize( l, v ); 
      base. push_back( n. lookup( l, v ));
   }
   return varatom( va. p, base );
}


bool varsubst::results_equal( label lab1, variable v1,
		              label lab2, variable v2 ) const 
{
   normalize( lab1, v1 );
   normalize( lab2, v2 );
    
   if( lab1 == lab2 && v1 == v2 )
      return true;
   else
      return false;
}


varatom varsubst::apply_and_replace( normalizer& n, label lab, varatom va,
				     label lab1, variable v1,
				     label lab2, variable v2 ) const 
{
   static std::vector< variable > base;
   base. clear( );

   normalize( lab1, v1 );
   normalize( lab2, v2 );

   for( std::vector< variable > :: const_iterator p = va. a1;
           p != va. a2;
           ++ p )
   {
      label l = lab;
      variable v = *p;
      normalize( l, v );

      // If we have (lab1,v1), then make it (lab2, v2). 
      // Otherwise, change nothing. 

      if( l == lab1 && v == v1 )
         base. push_back( n. lookup( lab2, v2 ));
      else
         base. push_back( n. lookup( l, v ));
   }
   return varatom( va. p, base );
}


