

// Code written by Ravinder Indarapu and Hans de Nivelle, May 2003.


#ifndef PROLOG_PRINTINGREADING_INCLUDED
#define PROLOG_PRINTINGREADING_INCLUDED   1


#include <string>
#include <iostream>


#include "syntax.h"
#include "expression.h"
#include "../parser/errors.h"
#include "../parser/inputstream.h"
#include "../partial.h"


namespace prolog  
{

   // Print expr, using the operator definitions in synt.


   void printexpression( std::ostream& os, 
                         const syntax& syntax,
                         const expression& expr );




   // Read an expression from inputreader. The read expression
   // is returned as a partial object.
   // In case we encounter EOF at a legal position, we return undefined. 
   // In case the inputreader creates a readerror, this error is
   // simply passed downwards.

   partial< expression > readexpression( parser::inputstream& inputreader, 
                                         const syntax& synt ) 
   throw ( parser::syntaxerror, parser::readerror );

} 


#endif


