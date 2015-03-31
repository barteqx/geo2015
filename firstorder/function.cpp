

// code written by Hans de Nivelle, June 2006.


#include "function.h" 


namespace firstorder
{



   function::function( const std::string& stringrepr, unsigned int arity ) 
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


   bool function::isdeclared( ) const
   {
      return properties [ repr ]. declared;
   }


   void function::declare( )
   {
      if( ! properties [ repr ]. declared )
      {
         properties [ repr ]. declared = true;
	 declarationstack. push_back( *this );
      }
   }


   unsigned int function::nrdeclarations( ) 
   {
      return declarationstack. size( );
   }

   void function::backtrackdeclarations( unsigned int x )
   {
      ASSERT( x <= declarationstack. size( ));
      while( declarationstack. size( ) > x )
      {
          function f = declarationstack. back( ); 
	  properties [ f. repr ]. declared = false; 
          declarationstack. pop_back( );
      }
   }


   void function::printtable( std::ostream& stream ) 
   {
      stream << "Table of Function Names:\n";
      stream << "=========================\n\n";
      for( unsigned int i = 0; i < properties. size( ); ++ i )
      {
         stream << properties [i]. stringrepr;
         stream << " / ";
         stream << properties [i]. arity;
         stream << "\n";
      }
      stream << "\n\n";

      stream << "Declared Functions:\n";
      stream << "======================\n";

      for( unsigned int i = 0; i < declarationstack. size( ); ++ i )
      {
         if( i != 0 )
            stream << ", ";
	 stream << declarationstack [i] << "/" << 
		   declarationstack [i]. getarity( ); 
      }
      stream << ".\n";
      stream << "\n\n";

#if 1
      for( std::map< std::string, std::list< unsigned int > > :: iterator 
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

} 


