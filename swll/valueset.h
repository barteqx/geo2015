

// Written by Hans de Nivelle, April 2006.
// 
// This data structure intended for storing the set of
// possible values that can be assigned to a variable.


#ifndef SWLL_VALUESET_DEFINED 
#define SWLL_VALUESET_DEFINED  1


#include <vector>
#include <iostream>


namespace swll
{

   class valueset
   {
      
      struct member
      {
         bool contained;
	 bool marked;

	 member( ) 
            : contained(0), marked(0)
         {
         }

      };

      std::vector< member > repr;

   public:
      valueset( );
      explicit valueset( unsigned int x );

      void clear( );
      void unmarkall( );
         // This operation should be unnecesary, because
	 // elements are inserted unmarked, and restrictedtomarked( )
	 // removes the marks on the remaining members.

      void insert( unsigned int x );
         // x will be inserted with marked == false.

      void mark( unsigned int x );
         // Mark x if it occurs in the valueset.
	 // Otherwise do nothing.

      bool contains( unsigned int x ) const;
         // True if the set contains x.

      unsigned int nrelements( ) const;
         // This operation is linear in the index of the
	 // greatest variable in the set. It is not very efficient. 
 
      unsigned int nrelements( unsigned int x ) const; 
         // Returns the number of elements in case there are less than 
	 // or exactly x elements. Otherwise it returns x. This function
	 // may be useful in case one is only interested in knowing 
	 // whether the set contains 0 or 1 element. 

      unsigned int someelement( ) const;
         // Returns an element from the set. 
         // In case the set is empty, a nonsense element is returned. 

      bool restricttomarked( );
         // Restrict the valueset to the marked integers.
	 // In the result, the remaining members will be unmarked.
	 // True is returned in case an unmarked member was deleted. 

      unsigned int upperbound( ) const { return repr. size( ); } 
         // Returns an unsigned int which is an upperbound of the 
	 // elements in the valueset. 

      friend std::ostream& operator << ( std::ostream& , const valueset& );
   };

   std::ostream & operator << ( std::ostream& , const valueset& );
      // Marked integers are printed with an asterix attached. 



   inline void valueset::insert( unsigned int x ) 
   {
      if( x >= repr. size( ))
         repr. insert( repr. end( ), 1 + x - repr. size( ), member( ));

      repr [x]. contained = true;
      repr [x]. marked = false; 
   }

   inline void valueset::mark( unsigned int x )
   {
      if( x >= repr. size( ))
         return;
      if( repr [x]. contained == false )
         return;
      repr [x]. marked = true;
   }

   inline bool valueset::contains( unsigned int x ) const
   {
      if( x >= repr. size( ))
         return false;

      return repr [x]. contained;
   }

   inline unsigned int valueset::nrelements( ) const
   {
      unsigned int s = 0;
      for( std::vector< member > :: const_iterator 
	      p = repr. begin( );
              p != repr. end( );
	      ++ p )
      {
         if( p -> contained )
            ++ s;
      }
      return s;
   }

   inline unsigned int valueset::nrelements( unsigned int x ) const
   {
      unsigned int s = 0;

      if( s == x )
         return s;

      for( std::vector< member > :: const_iterator 
	      p = repr. begin( );
              p != repr. end( );
	      ++ p )
      {
         if( p -> contained )
         {
            ++ s;  
	    if( s == x )
               return s;
         }
      }
      return s; 
   }

   inline unsigned int valueset::someelement( ) const
   {
      for( std::vector< member > :: const_iterator 
	      p = repr. begin( );
              p != repr. end( );
	      ++ p )
      {
         if( p -> contained ) 
            return p - repr. begin( );
      }
      return 0; 
   }


} 


#endif

