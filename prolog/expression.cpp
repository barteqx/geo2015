

// Code written by Hans de Nivelle, June 2006.
// Modified for Bliksem, September 2006. 


#include "expression.h"


namespace prolog 
{

   expression::expression( const expression& other )
      : repr( other. repr ) 
   {
      ASSERT( repr -> reference_count ); 
      ++ ( repr -> reference_count );
   }


   void expression::operator = ( expression other )
   {
      -- ( repr -> reference_count );
      if( ( repr -> reference_count ) == 0 )
         delete repr;

      repr = other. repr;
      ASSERT( repr -> reference_count );
      ++ ( repr -> reference_count );
   }


   expression::~expression( ) 
   {
      -- ( repr -> reference_count );
      if( ( repr -> reference_count ) == 0 )
         delete repr;
   }




   expression::expression( funcpred f, 
		           const std::list< expression > & subexpressions )
      : repr( new expression_node( f, subexpressions ))
   {
      ASSERT( f. getarity( ) == subexpressions. size( )); 
      ( repr -> reference_count ) = 1;
   }

   expression::expression( funcpred f, 
		           const std::vector< expression > & subexpressions )
      : repr( new expression_node( f, subexpressions ))
   {
      ASSERT( f. getarity( ) == subexpressions. size( ));
      ( repr -> reference_count ) = 1;
   }


   std::list< expression > :: const_iterator 
   expression::subexpressions_begin( ) const
   {
      return ( repr -> subexpressions. begin( ));
   }

   std::list< expression > :: const_iterator 
   expression::subexpressions_end( ) const
   {
      return ( repr -> subexpressions. end( ));
   }


   funcpred expression::getconstructor( ) const
   {
      return ( repr -> func );
   }

   unsigned int expression::getnrsubexpressions( ) const
   {
      return ( repr -> subexpressions. size( ));
   }



   std::ostream& operator << ( std::ostream& stream, 
		               const expression& exp )
   {
      stream << exp. getconstructor( );
      std::list< expression > :: const_iterator 
	     p1 = exp. subexpressions_begin( );
      std::list< expression > :: const_iterator 
	     p2 = exp. subexpressions_end( );

      if( p1 != p2 )
      {
         stream << "( "; 
         for( std::list< expression > :: const_iterator p = p1; p != p2; ++ p )
         {
            if( p != p1 )
               stream << ", ";
	    stream << *p;
         }
	 stream << " )"; 
      }
      return stream;
   }

} 



