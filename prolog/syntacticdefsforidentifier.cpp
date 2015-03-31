

// Written by Ravinder Indarapu, modified by Hans de Nivelle,
// in March 2003. Modified one more in September 2006. 


#include "syntacticdefsforidentifier.h"


#include <iostream>



namespace prolog
{
   
   syntacticdefsforidentifier::syntacticdefsforidentifier( ) :
      prefixdef(0), infixdef(0), postfixdef(0) 
   {
   }


   syntacticdefsforidentifier::syntacticdefsforidentifier(
      const syntacticdefsforidentifier& other )
         : prefixdef( other. prefixdef ? 
                         new syntacticdef( *other.prefixdef ) : 0 ),
           infixdef( other. infixdef ? 
                         new syntacticdef( *other. infixdef ) : 0 ),
           postfixdef( other. postfixdef ? 
                         new syntacticdef( *other. postfixdef ) : 0 )
   {
   }


   syntacticdefsforidentifier::~syntacticdefsforidentifier( )
   {
      delete prefixdef;
      delete infixdef;
      delete postfixdef;
   }



   bool syntacticdefsforidentifier::insert( const syntacticdef& toadd ) 
   {
      bool overwritten = false;

      if( toadd. isprefix( ))
      {
         if( prefixdef )
         {
            overwritten = true;
	    delete prefixdef;
         }
         prefixdef = new syntacticdef( toadd );
	 return overwritten; 
      }

      if( toadd. isinfix( ))
      {
         if( infixdef )
         {
            overwritten = true;
            delete infixdef;
         }
         infixdef = new syntacticdef( toadd );
         return overwritten;
      }

      if( toadd. ispostfix( ))
      {
         if( postfixdef )
         {
            overwritten = true;
	    delete postfixdef;
         }
         postfixdef = new syntacticdef( toadd );
         return overwritten;
      }
      ASSERT( false ); 
   }
 
   
   bool syntacticdefsforidentifier::remove( const syntacticdef& toremove ) 
   {
      if( prefixdef && *prefixdef == toremove ) 
      {
         delete prefixdef;
         prefixdef = 0;
	 return true; 
      }

      if( infixdef && *infixdef == toremove ) 
      {
         delete infixdef;
         infixdef = 0;
	 return true;
      }

      if( postfixdef && *postfixdef == toremove ) 
      {
         delete postfixdef;
         postfixdef = 0;
         return true;  
      }
      return false; 
   }


   void syntacticdefsforidentifier::operator = ( 
		   const syntacticdefsforidentifier& other ) 
   {
      if( prefixdef != other. prefixdef )
      {
         ASSERT( infixdef != other. infixdef && 
                 postfixdef != other. postfixdef );

         delete( prefixdef );
         delete( infixdef );
         delete( postfixdef );

         prefixdef = 0;
         infixdef = 0;
         postfixdef = 0;

         if( other. prefixdef )
            prefixdef = new syntacticdef( *other. prefixdef );
         if( other. infixdef ) 
            infixdef = new syntacticdef( *other. infixdef );
         if( other. postfixdef ) 
            postfixdef = new syntacticdef( *other. postfixdef );

      }
      else
      {
         ASSERT( infixdef == other. infixdef &&
                 postfixdef == other. postfixdef );
      }
   }


   bool syntacticdefsforidentifier::hasprefixdef( ) const 
   {
      return prefixdef;
   }

   bool syntacticdefsforidentifier::hasinfixdef( ) const
   {
      return infixdef;
   }

   bool syntacticdefsforidentifier::haspostfixdef( ) const
   {
      return postfixdef;
   }

   
   const syntacticdef& syntacticdefsforidentifier::getprefixdef( ) const
   {
      ASSERT( prefixdef );
      return *prefixdef;
   }
  
   
   const syntacticdef& syntacticdefsforidentifier::getinfixdef( ) const
   {
      ASSERT( infixdef );
      return *infixdef;
   }


   const syntacticdef& syntacticdefsforidentifier::getpostfixdef( ) const
   {
      ASSERT( postfixdef ); 
      return *postfixdef;
   }
 



   std::ostream& operator << ( std::ostream& os, 
                               const syntacticdefsforidentifier& sdi )
   {
      os << "[ ";  

      if( sdi. hasprefixdef( )) 
         os << sdi. getprefixdef( );
      else
         os << "(no prefixdef)";

      os << ", "; 

      if( sdi. hasinfixdef( ))
         os << sdi. getinfixdef( ); 
      else
         os << "(no infixdef)";
 
      os << ", ";

      if( sdi. haspostfixdef( ))                                   
         os << sdi. getpostfixdef( );
      else
         os << "(no postfixdef)";

      os << " ]";

      return os;
   }


}


      
