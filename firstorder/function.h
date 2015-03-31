

// Code written by Hans de Nivelle, June 2006. 


#ifndef FIRSTORDER_FUNCTION_INCLUDED
#define FIRSTORDER_FUNCTION_INCLUDED    1


#include "../predicate.h"
#include <map>
#include <string>


namespace firstorder 
{


   class function
   {

      static std::map< std::string, std::list< unsigned int > > lookuptable;
         // Assigns to each string the list of its representations.
	 // The distinct representations correspond to distinct
	 // arities. 


      struct prop
      {
         std::string stringrepr;
	 unsigned int arity;
         bool declared;
            // True if the name has been explictly declared under this 
	    // arity. 

	 prop( const std::string& stringrepr,
               unsigned int arity )
            : stringrepr( stringrepr ),
              arity( arity ),
	      declared( false ) 
         {
         }

      };


      static std::vector< prop > properties;
      static std::vector< function > declarationstack;


      unsigned int repr;

      function( );
         // Has no definition, because there is no default function.


   public:
      function( const std::string& stringrepr, unsigned int arity );
         // Construct function from name and arity. 

      bool isdeclared( ) const;
         // True if the function symbol has been declared.

      void declare( );
         // Declare this function symbol. 
        
      static unsigned int nrdeclarations( );
         // Returns the number of existing declarations.
         // This number can be used for backtracking.

      static void backtrackdeclarations( unsigned int x );
         // Backtrack declarations.

      const std::string& getstring( ) const;
      unsigned int getarity( ) const;


      static void printtable( std::ostream& stream );
         // Prints the table of all functions.

      friend bool operator == ( function, function );
      friend bool operator != ( function, function );
      friend bool operator < ( function, function );
      friend bool operator > ( function, function );

      friend std::ostream& operator << ( std::ostream& , const function& );
   };


   inline const std::string& function::getstring( ) const
   {
      return properties [ repr ]. stringrepr; 
   }


   inline unsigned int function::getarity( ) const
   {
      return properties [ repr ]. arity; 
   }


   inline std::ostream& operator << ( std::ostream& stream, 
		                      const function& f )
   {
      stream << f. getstring( ); 
      return stream;
   }


   inline bool operator == ( function f1, function f2 )
   {
      return f1. repr == f2. repr;
   }


   inline bool operator != ( function f1, function f2 )
   {
      return f1. repr != f2. repr;
   }


   inline bool operator < ( function f1, function f2 )
   {
      return f1. repr < f2. repr;
   }


   inline bool operator > ( function f1, function f2 )
   {
      return f1. repr > f2. repr;
   }


} 


#endif


