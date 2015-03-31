

// Code written by Hans de Nivelle, June 2006.
// Modified for Bliksem, september 2006.


#ifndef PROLOG_EXPRESSION_INCLUDED
#define PROLOG_EXPRESSION_INCLUDED   1


#include "funcpred.h"

#include <iostream> 
#include <list>
#include <vector>



namespace prolog 
{


   class expression
   {

      struct expression_node
      {
         unsigned int reference_count;
         funcpred func; 
         std::list< expression > subexpressions;


	 expression_node( funcpred func, 
			  const std::list< expression > & subexpressions )
            : func( func ),
	      subexpressions( subexpressions )
         {
         }


	 expression_node( funcpred func, 
			  const std::vector< expression > & subexpressions )
            : func( func ) 
         {
            for( std::vector< expression > :: const_iterator 
			 p = subexpressions. begin( );
			 p != subexpressions. end( );
			 ++ p )
            {
               ( this -> subexpressions ). push_back( *p );
            }
         }

      };


   private:
      expression_node* repr;




   public: 
      expression( const expression& other );
      void operator = ( expression other );
      ~expression( );

      expression( funcpred f, 
		  const std::list< expression > & subexpressions );
      expression( funcpred f, 
		  const std::vector< expression > & subexpressions );
         // The number of subexpressions must fit to the arity of f.

      std::list< expression > :: const_iterator subexpressions_begin( ) const;
      std::list< expression > :: const_iterator subexpressions_end( ) const;

      funcpred getconstructor( ) const;
      unsigned int getnrsubexpressions( ) const;
         // Equal to getfunction( ). getarity( ) of course.

   };

   std::ostream& operator << ( std::ostream& stream, const expression& exp );

}


#endif


