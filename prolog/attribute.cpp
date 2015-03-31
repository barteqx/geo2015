

// Code written by Ravinder Indarapu and Hans de Nivelle, March 2003. 
// Modified for Bliksem in September 2006. 



#include "attribute.h"


namespace prolog 
{


   attribute::~attribute( )
   { 
   }




   stringattribute::stringattribute( const std::string& str ) : str( str )
   {
   }


   void stringattribute::print( std::ostream& os ) const
   {
      os << str;
   }


   const attribute* stringattribute::clone( ) const
   {
      return new stringattribute( str );
   }


   stringattribute::~stringattribute( )
   {
   }




   emptyattribute::emptyattribute( ) 
   {
   }


   void emptyattribute::print( std::ostream& os ) const
   {
   }


   const attribute* emptyattribute::clone( ) const
   {
      return new emptyattribute( );
   }


   emptyattribute::~emptyattribute( )
   {
   }





   syntacticdefattribute::syntacticdefattribute( const syntacticdef& sdf ): 
      syntdef( sdf )
   {
   }


   void syntacticdefattribute::print( std::ostream& os ) const
   {
      os << syntdef;
   }

    
   const attribute* syntacticdefattribute::clone( ) const
   {
      return new syntacticdefattribute( syntdef );
   }
      

   syntacticdefattribute::~syntacticdefattribute( )
   {
   }





   expressionattribute::expressionattribute( const expression& exp ): 
      exp( exp )
   {
   }


   void expressionattribute::print( std::ostream& os ) const
   {
      os << exp;
   }


   const attribute* expressionattribute::clone( ) const
   {
      return new expressionattribute( exp );
   }
      

   expressionattribute::~expressionattribute( )
   {
   }





   expressionlistattribute::expressionlistattribute( 
         const std::list< expression > & explist ):
               explist( explist ) 
   {
   }


   void expressionlistattribute::print( std::ostream& os ) const  
   {
      os << "[ ";

      if( explist. size( ) == 0 ) 
         os << "]";
      else
      {
         std::list< expression > :: const_iterator p = explist. begin();
         os << *p;
         p ++ ;

         while( p != explist. end( ))
         {
            os << ", ";
            os << *p;
            p ++ ;
         }
         os << " ]";
      }
   }
 

   const attribute* expressionlistattribute::clone( ) const
   {
      return new expressionlistattribute( explist );
   }


   expressionlistattribute::~expressionlistattribute( )
   {
   }



} 


