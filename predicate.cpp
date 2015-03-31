

// code written by Hans de Nivelle, April 2006.


#include "predicate.h"



bool predicate::isdefined( const std::string& stringrepr,
		           unsigned int arity )
{
   std::map< std::string, std::list< unsigned int > > :: iterator 
         i = lookuptable. find( stringrepr );

   if( i == lookuptable. end( ))
      return false;

   for( std::list< unsigned > :: const_iterator 
           j = ( i -> second ). begin( );
	   j != ( i -> second ). end( );
	   ++ j )
   {
      if( properties [ *j ]. arity == arity )
         return true;
   }
   return false;
}


predicate::predicate( const std::string& stringrepr,
	              unsigned int arity ) 
{
   std::map < std::string, std::list< unsigned int > > :: iterator 
         i = lookuptable. find( stringrepr ); 

   if( i == lookuptable. end( ))
   {
      lookuptable. insert( 
         std::pair< std::string, std::list< unsigned int > > (
               stringrepr, std::list< unsigned int > ( )));

      i = lookuptable. find( stringrepr ); 
   }

   for( std::list< unsigned int > :: const_iterator
           j = ( i -> second ). begin( );
	   j != ( i -> second ). end( );
	   ++ j )
   {
      if( properties [ *j ]. arity == arity )
      {
         repr = *j;
	 return;
      }
   }

   // We need to create a new entry in the tables.

   repr = properties. size( );

   properties. push_back( prop( stringrepr, arity ));
   i -> second. push_back( repr );
}


void predicate::printtable( std::ostream& stream ) 
{
   stream << "Table of Predicate Names:\n";
   stream << "=========================\n\n";
   for( unsigned int i = 0; i < properties. size( ); ++ i )
   {
      stream << properties [i]. stringrepr;
      stream << " / ";
      stream << properties [i]. arity;
      stream << "\n";
   }
   stream << "\n\n";

#if 1
   for( std::map< std::string, std::list< unsigned int > > :: const_iterator 
	   i = lookuptable. begin( );
           i != lookuptable. end( );
	   ++ i )
   {
      stream << "( " << i -> first << ", " ;

      stream << "[ ";
      for( std::list< unsigned int > :: const_iterator 
              j = ( i -> second ). begin( );
	      j != ( i -> second ). end( );
	      ++ j )
      {
         if( j != ( i -> second ). begin( ))
            stream << ", ";
	 stream << *j;
      }
      stream << " ] ) ";
   }
   stream << "\n\n"; 
#endif

}



