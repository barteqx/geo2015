

// Written by Ravinder Indarapu and Hans de Nivelle in March 2003.
// Replaced containers::hashmap by std::map in August 2005. 


#include "syntax.h"


namespace prolog 
{


   syntax::syntax( ) :
      hh( std::map< std::string, syntacticdefsforidentifier > ( )), 
      nil( funcpred( "[]", 0 )),
      cons( funcpred( ".", 2 )), 
      printlistsyntax( true ),
      maxprintdepth( 1000000000 ),
      doublequotes( true ) 
   {
   }



   bool syntax::insert( const syntacticdef& toadd )
   {
      const std::string& ident = toadd. getstring( );
      return hh [ ident ]. insert( toadd ); 
   }
 

   bool syntax::remove( const syntacticdef& toremove )
   {
      const std::string& ident = toremove. getstring( );

      std::map< std::string, syntacticdefsforidentifier > :: iterator 
            pos = hh. find( ident );

      if( pos != hh. end( ))
      {
         bool removed = ( pos -> second ). remove( toremove ); 

	 if( ! ( pos -> second ). hasprefixdef( ) &&
             ! ( pos -> second ). hasinfixdef( ) &&
	     ! ( pos -> second ). haspostfixdef( ))
         {
            hh. erase( pos );  
         }
	 return removed; 
      }
      else
         return false; 
   }


   bool syntax::hasprefixdef( const std::string& ident ) const
   {
      std::map< std::string, syntacticdefsforidentifier > :: 
	 const_iterator pos = hh. find( ident ); 
      return pos != hh.end( ) && ( pos -> second ). hasprefixdef( );
   }


   bool syntax::hasinfixdef( const std::string& ident ) const
   {
      std::map< std::string, syntacticdefsforidentifier > :: 
         const_iterator pos = hh. find( ident ); 
      return pos != hh. end( ) && ( pos -> second ). hasinfixdef( ); 
   }

   
   bool syntax::haspostfixdef( const std::string& ident ) const
   {
      std::map< std::string, syntacticdefsforidentifier > ::
	 const_iterator pos = hh. find( ident );
      return pos != hh. end( ) && ( pos -> second ). haspostfixdef( );
   }




   const syntacticdef& syntax::getprefixdef( const std::string& ident) const
   {
      std::map< std::string, syntacticdefsforidentifier > ::
	 const_iterator pos = hh. find( ident );
      ASSERT( pos != hh. end( ) && pos -> second. hasprefixdef( ));

      return ( pos -> second ). getprefixdef( );
   }


   const syntacticdef& syntax::getinfixdef( const std::string& ident) const
   {
      std::map< std::string, syntacticdefsforidentifier > ::
	 const_iterator pos = hh. find( ident );
      ASSERT( pos != hh. end( ) && pos -> second. hasinfixdef( ));

      return ( pos -> second ). getinfixdef( );
   }


   const syntacticdef& syntax::getpostfixdef( const std::string& ident) const
   {
      std::map< std::string, syntacticdefsforidentifier > ::
	 const_iterator pos = hh. find( ident );
      ASSERT( pos != hh. end( ) && pos -> second. haspostfixdef( ));

      return ( pos -> second ). getpostfixdef( );
   }




   std::ostream& operator << ( std::ostream& os, const syntax& synt )
   {
      os << "Syntax:\n\n";

      os << "nil =                 " << synt. nil << "\n";
      os << "cons =                " << synt. cons << "\n";
      os << "printlistsyntax =     " << synt. printlistsyntax << "\n";
      os << "maxprintdepth =       " << synt. maxprintdepth << "\n";
      os << "doublequotes =        " << synt. doublequotes << "\n"; 
      os << "\n";

      os << "defined operators: \n\n";
      for( std::map< std::string, 
		     syntacticdefsforidentifier > :: const_iterator 
           it = synt. hh. begin( ); it != synt. hh. end( ); ++ it ) 
      {
         os << ( it -> first ) << "    " << ( it -> second ) << "\n"; 
      }
      return os;
   }

} 


