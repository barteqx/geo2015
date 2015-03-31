

// Code written by Ravinder Indarapu and Hans de Nivelle, March 2003.
// Adapted for Bliksem in September 2006 by Hans de Nivelle.



#include "token.h"
#include "../assert.h"


namespace prolog 
{

   bool token::has_string( unsigned int x ) 
   {
      switch(x)
      {
      case identifier:
      case quotedidentifier:
      case gluedidentifier:
      case gluedquotedidentifier:
      case error:
         return true;
      }
      return false;
   }


   bool token::has_nothing( unsigned int x ) 
   {
      switch(x)
      {
      case lpar:
      case rpar:
      case lsqpar:
      case rsqpar:
      case endoffile:
      case dot:
      case comma:
      case bar:
      case start:
         return true;
      }
      return false;
   }


   bool token::has_syntacticdef( unsigned int x )
   {
      switch(x)
      {
      case prefix:
      case infix:
      case postfix:
         return true;
      }
      return false;
   }


   bool token::has_expression( unsigned int x )
   {
      switch(x)
      {
      case input:
      case expr:
         return true;
      }
      return false;
   }


   bool token::has_expressionlist( unsigned int x ) 
   {
      switch(x)
      {
      case args:
      case list:
         return true;
      }
      return false;
   }
 


// The real constructors. 


   token::token( unsigned int x, const std::string& str )
      : type( x ), attr( new stringattribute( str ))
   {
      ASSERT( has_string(x) );
   }
    


   token::token( unsigned int x ) 
      : type( x ), attr( new emptyattribute( ))
   {
      ASSERT( has_nothing(x) );
   }


   token::token( unsigned int x, const syntacticdef& sd ) 
      : type( x ), attr( new syntacticdefattribute( sd ))
   {
      ASSERT( has_syntacticdef(x) );
   }


   token::token( unsigned int x, const expression& exp )
      : type( x ), attr( new expressionattribute( exp ))
   {
      ASSERT( has_expression(x) );
   }


   token::token( unsigned int x, const std::list< expression > & explist )
      : type( x ), attr( new expressionlistattribute( explist ))
   {
      ASSERT( has_expressionlist(x) );
   }

   
// Copy constructor:


   token::token( const token& other ) 
      : type( other. type ), attr( other. attr -> clone( )) 
   {
   }


// Assignment operator:


   void token::operator = ( const token& other ) 
   {
      if( attr != other. attr ) 
      {
         delete attr;

         type = other. type; 
         attr = other. attr -> clone( );
      }
   }


// Destructor:

   token::~token( )
   {
      delete attr;
   }


   
// Attribute extraction functions: 




   const std::string& token::getstring( ) const
   {
      ASSERT( has_string( type ));
      const stringattribute* a = 
            dynamic_cast< const stringattribute* > ( attr );
      return a -> str;
   }
   


   const syntacticdef& token::getsyntacticdef( ) const
   {
      ASSERT( has_syntacticdef( type ));
      const syntacticdefattribute* a = 
            dynamic_cast< const syntacticdefattribute* > ( attr );
      return a -> syntdef;
   }


   const expression& token::getexpression( ) const 
   {
      ASSERT( has_expression( type ));
      const expressionattribute* a = 
            dynamic_cast< const expressionattribute* > ( attr );
      return a -> exp;
   }


   const std::list< expression > & token::getexpressionlist( ) const
   {
      ASSERT( has_expressionlist( type ));
      const expressionlistattribute* a = 
            dynamic_cast< const expressionlistattribute* > ( attr );
      return a -> explist;
   }



   std::ostream& operator << ( std::ostream& os, const token& tok )
   {
      switch( tok. type )
      {
      case token::identifier:
         os << "identifier( "; 
         break;
      case token::quotedidentifier:
         os << "quotedidentifier( ";
         break;
      case token::gluedidentifier:
         os << "gluedidentifier( ";
         break;
      case token::gluedquotedidentifier:
         os << "gluedquotedidentifier( ";
         break;
      case token::error:
         os << "error( ";
         break;

      case token::lpar:
         os << "lpar( ";
         break;
      case token::rpar:
         os << "rpar( ";
         break;
      case token::lsqpar:
         os << "lsqpar( ";
         break;
      case token::rsqpar:
         os << "rsqpar( ";
         break;
      case token::endoffile:
         os << "endoffile( ";
         break;
      case token::dot:
         os << "dot( ";
         break;
      case token::comma:
         os << "comma( ";
         break;
      case token::bar:
         os << "bar( ";
         break;
      case token::start:
         os << "start( ";
         break;

      case token::prefix:
         os << "prefix( ";
         break;
      case token::infix:
         os << "infix( ";
         break;
      case token::postfix:
         os << "postfix( ";
         break;

      case token::input:
         os << "input( ";
         break;
      case token::expr:
         os << "expr( ";
         break;

      case token::args:
         os << "args( ";
         break;
      case token::list:
         os << "list( ";
         break;

      }

      tok. attr -> print( os ); 

      os << " )"; 
      
      return os; 
   }



   void testtoken( ) 
   { 
      for( unsigned int i = 0; i < 80000; i ++ ) 
      {

         std::list< expression > args;
	 args. push_back( expression( funcpred( "a", 0 ), 
				      std::list< expression > ( )));
	 args. push_back( expression( funcpred( "b", 0 ),
				      std::list< expression > ( )));
         token t( 500 + i % 2, args );
         std::cout << t << "\n";
         token t2(t);
         std::cout << t2 << "\n"; 
         t = t2;
         std::cout << t << "\n";
      }
   }

}


