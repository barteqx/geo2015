

// Code written by Hans de Nivelle, June 2006.


#ifndef FIRSTORDER_TERM_INCLUDED
#define FIRSTORDER_TERM_INCLUDED   1


#include "function.h"

#include <list>
#include <vector>



namespace firstorder
{


   class term 
   {

      struct term_node
      {
         unsigned int reference_count;
         function func; 
         std::list< term > subterms;


	 term_node( function func, const std::list< term > & subterms )
            : func( func ),
	      subterms( subterms )
         {
         }


	 term_node( function func, const std::vector< term > & subterms )
            : func( func ) 
         {
            for( std::vector< term > :: const_iterator 
			 p = subterms. begin( );
			 p != subterms. end( );
			 ++ p )
            {
               ( this -> subterms ). push_back( *p );
            }
         }

      };


   private:
      term_node* repr;




   public: 
      term( const term& other );
      void operator = ( term other );
      ~term( );

      term( function f, const std::list< term > & subterms );
      term( function f, const std::vector< term > & subterms );
         // The number of subterms must fit to the arity of f.

      std::list< term > :: const_iterator subterms_begin( ) const;
      std::list< term > :: const_iterator subterms_end( ) const;

      function getfunction( ) const;
      unsigned int getnrsubterms( ) const;
         // Equal to getfunction( ). getarity( ) of course.


      static int compare( term t1, term t2 );
         // -1 means: t1 is smaller.
	 // 0 means: t1, t2 equal.
	 // 1 means: t1 is bigger.
         // The order is not a well-order.

      bool operator < ( term other ) const;
      bool operator == ( term other ) const;
      bool operator != ( term other ) const; 

   };

   std::ostream& operator << ( std::ostream& stream, const term& t );

}


#endif


