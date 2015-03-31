

// A syntacticdef is a Prolog-style operator definition, see
// for example in 
//    Sterling and Shapiro, the art of Prolog. 


// Code written by Ravinder Indarapu and Hans de Nivelle, March 2003
// Modified in September 2006.


#ifndef PROLOG_SYNTACTICDEF_INCLUDED
#define PROLOG_SYNTACTICDEF_INCLUDED   1


#include <iostream>
#include <string>


namespace prolog
{

   class syntacticdef 
   {

   private:

      std::string ident;
      unsigned int type;
      unsigned int attraction;

   public:

// Attractions should lie between 0 and supattraction:
// A smaller number means stronger attraction.

      static const unsigned int supattraction = 1000;


// Prefix, not allowing same binding strength in argument:

     static const unsigned int FX = 100;


// Prefix, allowing same binding strength in argument:
      static const unsigned int FY = 200;

// Infix, not allowing same binding strength in any of the
// arguments. The operator is non-associative:

      static const unsigned int XFX = 300;


// Infix, allowing same binding strength on the right, but
// not on the left. The operator is right-associative:

      static const unsigned int XFY = 400;


// Infix, allowing same binding strength on the left, but
// not on the right. The operator is left-associative:

      static const unsigned int YFX = 500;


// Postfix, not allowing same binding strength in argument: 

      static const unsigned int XF = 600;


// Postfix, allowing same binding strength in argument:
      
      static const unsigned int YF = 700;


// Create a syntacticdef. Once it is created, it cannot be
// changed anymore. The constructor terminates the program if  
// it does not like any of its arguments. The type should be one of
// XFX, XFY, YFX, FX, FY, XF, YF. attraction should be an unsigned integer
// between 0 and SUPATTRACTION. A smaller number means a stronger
// attraction.

   syntacticdef( const std::string& ident, 
		 unsigned int type, unsigned int attraction );

// Return the funcpred from which this syntacticdef is built.
      const std::string& getstring( ) const;
 

// Return true if the syntacticdef is of prefix-type. 
      bool isprefix( ) const;

// Return true if the syntacticdef is of infix-type. 
      bool isinfix( ) const;

// Return true if the syntacticdef is of postfix-type.  
      bool ispostfix( ) const;

// Decide how the conflict between this syntacticdef
// and the other syntacticdef should be resolved. 
// The question is how to parse
// T1 this T2 other T3.
// A return value of -1 denotes ( T1 this T2 ) other T3.
// A return value of 1 denotes T1 this ( T2 other T3 ).
// A return value of 0 denotes that the conflict is a syntax error.

      int decideconflict( const syntacticdef& other ) const;

//    True if this and the other syntacticdef are equal. 

      bool operator == ( const syntacticdef& other ) const;
      bool operator != ( const syntacticdef& other ) const;


      friend std::ostream& operator << ( std::ostream&, const syntacticdef& );
   };

}


#endif


