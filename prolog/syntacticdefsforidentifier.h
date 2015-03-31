

#ifndef PROLOG_SYNTACTICDEFSFORIDENTIFIER_INCLUDED
#define PROLOG_SYNTACTICDEFSFORIDENTIFIER_INCLUDED  1


// Written by Ravinder Indarapu, modified by Hans de Nivelle,
// in March 2003, and one more time modified in september 2006. 


#include "../assert.h" 
#include "syntacticdef.h"


namespace prolog 
{

   class syntacticdefsforidentifier
   {

   private: 

      const syntacticdef* prefixdef;
      const syntacticdef* infixdef;
      const syntacticdef* postfixdef;

   public: 


      syntacticdefsforidentifier( ); 
      syntacticdefsforidentifier( const syntacticdefsforidentifier& other );
      ~syntacticdefsforidentifier( );

 
// Insert toadd into this syntacticdefsforidentifier. If toadd 
// is a prefixoperator definition, and there already is a 
// prefixoporator defined, then the existing definition of overwritten,
// insert returns true. If there is no existing value, then insert
// returns false. 
// The same applies to inserting of an infixoperator or postfixoperator. 

      bool insert( const syntacticdef& toadd );


// Remove toremove if it occurs in this syntacticdefsforidentifier.
// Otherwise do nothing. In case something was removed,
// we return true. 

      bool remove( const syntacticdef& toremove );


      void operator = ( const syntacticdefsforidentifier& other ); 


      bool hasprefixdef( ) const;
      bool hasinfixdef( ) const;
      bool haspostfixdef( ) const;


      const syntacticdef& getprefixdef( ) const;
      const syntacticdef& getinfixdef( ) const;
      const syntacticdef& getpostfixdef( ) const;


   private:
      const syntacticdef* makecopy( const syntacticdef* ); 
         /* Makes a copy if the points is different from 0. */ 

   };

   std::ostream& operator << ( std::ostream&, 
                               const syntacticdefsforidentifier& ); 
}


#endif



