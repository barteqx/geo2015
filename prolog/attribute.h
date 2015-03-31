

// Code written by Ravinder Indarapu and Hans de Nivelle, March 2003. 
// Adopted for Bliksem in September 2006 by Hans de Nivelle.


#ifndef PROLOG_ATTRIBUTES_INCLUDED
#define PROLOG_ATTRIBUTES_INCLUDED   1


#include <iostream>
#include <list>
#include <string>


#include "syntacticdef.h"
#include "expression.h"


namespace prolog 
{


// attribute is the abstract class from which the possible attribute
// classes must inherit.


   struct attribute
   {
      virtual ~attribute( );  
      virtual void print( std::ostream& ) const = 0; 
      virtual const attribute* clone( ) const = 0;
   };


   struct stringattribute : public attribute
   {
      const std::string str;

      stringattribute( const std::string& ); 
      void print( std::ostream& ) const; 
      const attribute* clone( ) const; 
      ~stringattribute( ); 
   };


   struct emptyattribute : public attribute
   {
      emptyattribute( );  
      void print( std::ostream& ) const; 
      const attribute* clone( ) const; 
      ~emptyattribute( ); 
   };


   struct syntacticdefattribute : public attribute
   {
      const syntacticdef syntdef;  

      syntacticdefattribute( const syntacticdef& ); 
      void print( std::ostream& ) const; 
      const attribute* clone( ) const; 
      ~syntacticdefattribute( ); 
   };


   struct expressionattribute : public attribute
   {
      const expression exp;

      expressionattribute( const expression& ); 
      void print( std::ostream& ) const; 
      const attribute* clone( ) const; 
      ~expressionattribute( ); 
   };


   struct expressionlistattribute : public attribute
   {
      const std::list< expression > explist;

      expressionlistattribute( const std::list< expression > & explist ); 
      void print( std::ostream& ) const; 
      const attribute* clone( ) const; 
      ~expressionlistattribute( ); 
   };


}


#endif
