
// Code written by Ravinder Indarapu and Hans de Nivelle, May 2003.


#include "printingreading.h"
#include "context.h"
#include "DFAclassification.h"


#include <sstream>




namespace prolog 
{

   namespace
   {

      void printexpression( std::ostream& os,
                            const syntax& syntax,
                            unsigned int depth,
                            const context& ctxt,
                            const syntacticdef* leftsd,
                            const syntacticdef* rightsd,
                            const expression& expr );


      void printlist( std::ostream& os,
                      const syntax& syntax,
                      unsigned int depth,
                      const syntacticdef* leftsd,
                      const syntacticdef* rightsd,
                      expression expr );


      void printprefix( std::ostream& os,
                        const syntax& syntax,
                        unsigned int depth,
                        const context& ctxt,
                        const syntacticdef* leftsd,
                        const syntacticdef* rightsd,
                        const expression& expr );


      void printpostfix( std::ostream& os,
                         const syntax& syntax,
                         unsigned int depth,
                         const context& ctxt,
                         const syntacticdef* leftsd,
                         const syntacticdef* rightsd,
                         const expression& expr );

      void printinfix( std::ostream& os,
                       const syntax& syntax,
                       unsigned int depth,
                       const context& ctxt,
                       const syntacticdef* leftsd,
                       const syntacticdef* rightsd,
                       const expression& expr );


      void printatom( std::ostream& os,
                      const syntax& syntax,
                      unsigned int depth,
                      const context& ctxt,
                      const syntacticdef* leftsd,
                      const syntacticdef* rightsd,
                      const expression& expr );


      void printfuncterm( std::ostream& os,
                          const syntax& syntax,
                          unsigned int depth,
                          const context& ctxt,
                          const syntacticdef* leftsd,
                          const syntacticdef* rightsd,
                          const expression& expr );

      void printidentifier( std::ostream& os,
		            const syntax& syntax,
			    const context& ctxt,
			    funcpred op,
			    bool withlpar );
         // Print op, deciding whether quotes are necessary,
	 // Also print the following '(' if withlpar == true.




      void printexpression( std::ostream& os,
                            const syntax& syntax,
                            unsigned int depth,
                            const context& ctxt,
                            const syntacticdef* leftsd,
                            const syntacticdef* rightsd,
                            const expression& expr )
      {
         if( depth > syntax. maxprintdepth )
            os << " <...> ";
         else
         {
            funcpred op = expr. getconstructor( );
            unsigned int arity = expr. getnrsubexpressions( );

            if( syntax. printlistsyntax && op == syntax.cons )
            {
               printlist( os, syntax, depth, leftsd, rightsd, expr );
               return;
            }

            if( arity == 1 && syntax. hasprefixdef( op. getstring( ) ))
            {
               printprefix( os, syntax, depth, ctxt, 
			    leftsd, rightsd, expr );
               return;
            }

            if( arity == 1 && syntax. haspostfixdef( op. getstring( ) ))
            {
               printpostfix( os, syntax, depth, ctxt, 
			     leftsd, rightsd, expr );
               return;
            }

            if( arity == 2 && syntax. hasinfixdef( op. getstring( ) ))
            {
	       printinfix( os, syntax, depth, ctxt, leftsd, rightsd, expr );
               return;
            }

            if( arity == 0 )
	    {
	       printatom( os, syntax, depth, ctxt, leftsd, rightsd, expr );
               return;
            }
            else
            {
	       printfuncterm( os, syntax, depth, ctxt, leftsd, rightsd, expr );
               return;
            }
         }
      }



      void printlist( std::ostream& os,
                      const syntax& syntax,
                      unsigned int depth,
                      const syntacticdef* leftsd,
                      const syntacticdef* rightsd,
                      expression expr )
      {

         ASSERT( expr. getconstructor( ) == syntax. cons );

         os << "[ ";

         std::list< expression > :: const_iterator 
	                sub = expr. subexpressions_begin( );

         printexpression( os, syntax, depth + 1,
                          context( context::listarg ), 0, 0, *( sub ++ ) );
         expr = *sub; 

         while( expr. getconstructor( ) == syntax. cons )
         {
            os << ", ";

	    sub = expr. subexpressions_begin( );

            printexpression( os, syntax, depth + 1, 
                             context( context::listarg ), 0, 0, * ( sub ++ ));
	    expr = *sub; 
         }
	    
         if( expr. getconstructor( ) != syntax. nil )
         {
            os << " | ";
            printexpression( os, syntax, depth + 1, 
                             context( context::listarg ), 0, 0, expr );
         }
         os << " ]";
      }



      void printprefix( std::ostream& os,
                        const syntax& syntax,
                        unsigned int depth,
                        const context& ctxt,
                        const syntacticdef* leftsd,
                        const syntacticdef* rightsd,
                        const expression& expr )
      {
         funcpred op = expr. getconstructor( );
         const syntacticdef& prefixdef = 
		                syntax. getprefixdef( op. getstring( ));

         bool parentheses = false;

         if( rightsd && prefixdef. decideconflict( *rightsd ) != -1 )
            parentheses = true;

         if( parentheses )
         {
            os << "( ";
            printidentifier( os, syntax, ctxt, op, false );
	    os << " ";
            printexpression( os, syntax, depth + 1, 
                             context( context::parentheses ), 
			     &prefixdef, 0, *expr. subexpressions_begin( ));
            os << " )";
         }
         else
         {
            printidentifier( os, syntax, ctxt, op, false );
            os << " ";
            printexpression( os, syntax, depth + 1, ctxt, 
			     &prefixdef, rightsd, 
			     *expr. subexpressions_begin( ));
         }
      }




      void printpostfix( std::ostream& os,
                         const syntax& syntax,
                         unsigned int depth,
                         const context& ctxt,
                         const syntacticdef* leftsd,
                         const syntacticdef* rightsd,
                         const expression& expr )
      {

         funcpred op = expr. getconstructor( ); 
         const syntacticdef& 
	             postfixdef = syntax. getpostfixdef( op. getstring( ));

         bool parentheses = false;

         if( leftsd && ( leftsd -> decideconflict( postfixdef )) != -1 )
            parentheses = true;

         if( syntax. hasinfixdef( op. getstring( ) ) && rightsd )
            parentheses = true;

         if( parentheses )
         {
            os << "( ";
            printexpression( os, syntax, depth + 1, 
                             context( context::parentheses ), 
                             0, &postfixdef, *expr. subexpressions_begin( ));
            os << " ";
            printidentifier( os, syntax, ctxt, op, false );
            os << " )";
         }
         else
         {
            printexpression( os, syntax, depth + 1, 
                             ctxt, leftsd, &postfixdef, 
			     *expr. subexpressions_begin( )); 
            os << " ";
            printidentifier( os, syntax, ctxt, op, false );
         }
      }



      void printinfix( std::ostream& os,
                       const syntax& syntax,
                       unsigned int depth,
                       const context& ctxt,
                       const syntacticdef* leftsd,
                       const syntacticdef* rightsd,
                       const expression& expr )
      {

         funcpred op = expr. getconstructor( ); 
         const syntacticdef& infixdef = syntax. getinfixdef( op. getstring( ));

         bool parentheses = false;

         if( leftsd && ( leftsd -> decideconflict( infixdef )) != 1 )
            parentheses = true;

         if( rightsd && infixdef. decideconflict( *rightsd ) != -1 )
            parentheses = true;

         std::list< expression > :: const_iterator
                         sub = expr. subexpressions_begin( );

         if( parentheses )
         {
            os << "( ";
            printexpression( os, syntax, depth + 1, 
                             context( context::parentheses ), 
                             0, &infixdef, * ( sub ++ ) );
            os << " ";
            printidentifier( os, syntax, ctxt, op, false );
	    os << " ";

            printexpression( os, syntax, depth + 1, 
                             context( context::parentheses ), 
                             &infixdef, 0, *sub );

            os << " )";
         }
         else
         {
            printexpression( os, syntax, depth + 1, 
                             ctxt, leftsd, &infixdef, * ( sub ++ ) );
            os << " ";
            printidentifier( os, syntax, ctxt, op, false );
            os << " ";

            printexpression( os, syntax, depth + 1, ctxt,
                             &infixdef, rightsd, *sub );

         }
      }



      void printatom( std::ostream& os,
                      const syntax& syntax,
                      unsigned int depth,
                      const context& ctxt,
                      const syntacticdef* leftsd,
                      const syntacticdef* rightsd,
                      const expression& expr )
      {

         funcpred op = expr. getconstructor( ); 
         ASSERT( op. getarity( ) == 0 );

         bool parentheses = false;

         if( syntax. hasprefixdef( op. getstring( )) ||
             syntax. hasinfixdef( op. getstring( )) ||
	     syntax. haspostfixdef( op. getstring( )) )
         {
            if( leftsd || rightsd )
               parentheses = true;
         }

         if( parentheses )
         {
            os << "( ";
            printidentifier( os, syntax, ctxt, op, false );
            os << " )";
         }
         else
            printidentifier( os, syntax, ctxt, op, false );

      }



      void printfuncterm( std::ostream& os,
                          const syntax& syntax,
                          unsigned int depth,
                          const context& ctxt,
                          const syntacticdef* leftsd,
                          const syntacticdef* rightsd,
                          const expression& expr )
      {
         funcpred op = expr. getconstructor( );

         unsigned int arity = expr. getnrsubexpressions( );

         printidentifier( os, syntax, ctxt, op, true );
            // This also prints the '('.

         os << " "; 

         for( std::list< expression > :: const_iterator
                 sub = expr. subexpressions_begin( );
	         sub != expr. subexpressions_end( );
	         ++ sub ) 
         {
            if( sub != expr. subexpressions_begin( ))
               os << ", "; 

            printexpression( os, syntax, depth + 1, 
                             context( context::funcarg ), 
                             0, 0, *sub );
         }
         os << " )";
      }



      // printidentifier uses quotes to protect special strings. 
      // Dependent on synt. doublequotes, printidentifier inserts
      // single or double quotes when required. 
      // In order to decide whether the strings , | . need
      // quotes, printidentifier uses the context. 
      //
      // The parameter withlpar is a relict from older versions, 
      // where we used an outputstream that could decide to insert a 
      // newline in case the line becomes too long. Since inserting a 
      // newline between a name and a parenthesis can be fatal in
      // Prolog syntax, the parenthesis has to be
      // printed simultaneously with the identifier.


      void printidentifier( std::ostream& os, 
                            const syntax& synt,
                            const context& ctxt,
                            funcpred op,
                            bool withlpar )
      {
         std::ostringstream ss;

         bool needsquotes = false;
         const std::string& s = op. getstring( );

         if( DFAclassification::nameneedsquotes(s))
            needsquotes = true;

         if( s == "," && ctxt. allows( "," ))
            needsquotes = true;

         if( s == "|" && ctxt. allows( "|" ))
            needsquotes = true;

         if( s == "."  && ctxt. allows( "." ))
            needsquotes = true;

         if( needsquotes )
         {
            if( synt. doublequotes )
               ss << "\"" << s << "\"";
	    else
               ss << "\'" << s << "\'";
         }
         else
            ss << s;

         if( withlpar )
            ss << '(';

         os << ss. str( );  
      }

   }


   void printexpression( std::ostream& os,
                         const syntax& syntax,
                         const expression& expr )
   {
      printexpression( os, syntax, 0, 
                       context( context::toplevel ), 0, 0, expr ); 
   }


}


