
#ifndef GEOMETRIC_BEST_FALSE_INSTANCES_INCLUDED
#define GEOMETRIC_BEST_FALSE_INSTANCES_INCLUDED  1

// best_false_instances collects a few of the best false instances,
// as decided by false_instance::isworsethan( ). 
// In Geo 2007, only a single one was kept, but here we try to keep
// a few more. Objects of best_false_instances are intended to be
// small. 
// Written by Hans de Nivelle, Jan 2015. 

#include "false_instance.h"
#include <list>

namespace geometric
{

   struct best_false_instances 
   {
      unsigned int max;  
      std::list< false_instance > lst;
         // List is sorted, best first.

      explicit best_false_instances( unsigned int max )
         : max{ max }
      { }


      using iterator = std::list< false_instance > :: iterator;
      using const_iterator = std::list< false_instance > :: const_iterator;
 
      iterator begin( ) { return lst. begin( ); }
      iterator end( ) { return lst. end( ); }

      const_iterator cbegin( ) const { return lst. cbegin( ); }
      const_iterator cend( ) const { return lst. cend( ); }

      const_iterator begin( ) const { return lst. cbegin( ); }
      const_iterator end( ) const { return lst. cend( ); }

      void insert( false_instance&& f );
         // Insert at the first position where the current false instance
         // is strictly worse. Also transfer ownership. 

      unsigned int size( ) const { return lst. size( ); } 

      false_instance& front( ) { return lst. front( ); }
      const false_instance& front( ) const { return lst. front( ); } 
      false_instance& back( ) { return lst. back( ); }
      const false_instance& back( ) const { return lst. back( ); }
 
      void pop_front( ) { lst. pop_front( ); }
      void pop_back( ) { lst. pop_back( ); } 

   };

   std::ostream& operator << ( std::ostream& stream,
                               const best_false_instances& best );

}

#endif

