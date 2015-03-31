

// Code written by Hans de Nivelle, June 2006.


#include "formula.h"
#include <cstdlib>


namespace firstorder
{

   formula::formula( const formula& other )
      : repr( other. repr ) 
   {
      ASSERT( repr -> reference_count ); 
      ++ ( repr -> reference_count );
   }


   // If you replace 'formula other' by 'const formula& other',
   // the reference counting may mess up in case you assign
   // a subterm to a superterm.

   void formula::operator = ( formula other )
   {
      -- ( repr -> reference_count );
      if( ( repr -> reference_count ) == 0 )
         delete repr;

      repr = other. repr;
      ASSERT( repr -> reference_count ); 
      ++ ( repr -> reference_count );
   }


   formula::~formula( )
   {
      -- ( repr -> reference_count );
      if( ( repr -> reference_count ) == 0 )
         delete repr;
   }



   formula formula::make_false( ) 
   {
      return formula( fol_false,
		      std::list< function > ( ),
		      std::list< predicate > ( ),
		      std::list< term > ( ),
		      std::list< formula > ( ));
   }


   formula formula::make_true( ) 
   {
      return formula( fol_true,
		      std::list< function > ( ),
		      std::list< predicate > ( ),
		      std::list< term > ( ),
		      std::list< formula > ( ));
   }


   formula formula::make_not( const formula& f )
   {
      std::list< formula > sub;
      sub. push_back(f); 
      return formula( fol_not,
		      std::list< function > ( ),
		      std::list< predicate > ( ),
		      std::list< term > ( ),
		      sub );
   }


   formula formula::make_and( const std::list< formula > & subforms ) 
   {
      return formula( fol_and,
		      std::list< function > ( ),
		      std::list< predicate > ( ),
		      std::list< term > ( ),
		      subforms );
   }


   formula formula::make_or( const std::list< formula > & subforms )
   {
      return formula( fol_or,
		      std::list< function > ( ),
		      std::list< predicate > ( ),
		      std::list< term > ( ),
		      subforms );
   }


   formula formula::make_implies( const formula& f1, const formula& f2 )
   {
      std::list< formula > sub;
      sub. push_back(f1);
      sub. push_back(f2); 
      return formula( fol_implies,
		      std::list< function > ( ),
		      std::list< predicate > ( ),
		      std::list< term > ( ),
		      sub );
   }


   formula formula::make_equiv( const formula& f1, const formula& f2 )
   {
      std::list< formula > sub;
      sub. push_back(f1);
      sub. push_back(f2);
      return formula( fol_equiv,
                      std::list< function > ( ),
		      std::list< predicate > ( ),
		      std::list< term > ( ),
		      sub );
   }


   formula formula::make_forall( const std::list< function > & variables,
		                 const formula& f )
   {
      std::list< formula > sub;
      sub. push_back(f);

      ASSERT( zero_arity_only( variables )); 

      return formula( fol_forall,
		      variables,
		      std::list< predicate > ( ),
		      std::list< term > ( ),
		      sub );
   }


   formula formula::make_exists( const std::list< function > & variables,
		                 const formula& f )
   {
      std::list< formula > sub;
      sub. push_back(f);

      ASSERT( zero_arity_only( variables ));

      return formula( fol_exists,
                      variables,
		      std::list< predicate > ( ),
		      std::list< term > ( ),
		      sub );
   }


   formula formula::make_equals( const term& t1, const term& t2 )
   {
      std::list< term > sub;
      sub. push_back(t1);
      sub. push_back(t2);

      return formula( fol_equals,
		      std::list< function > ( ),
		      std::list< predicate > ( ),
		      sub, 
		      std::list< formula > ( ));
   }


   formula formula::make_atom( const predicate& p,
		               const std::list< term > & subterms )
   {
      ASSERT( p. getarity( ) == subterms. size( ));

      std::list< predicate > pred;
      pred. push_back(p);

      return formula( fol_atom, 
		      std::list< function > ( ),
		      pred, 
		      subterms,
		      std::list< formula > ( ));
   }


   
   fol_oper formula::getoperator( ) const
   {
      return ( repr -> op );
   }


   std::list< function > :: const_iterator 
   formula::variables_begin( ) const
   {
      ASSERT( ( repr -> op ) == fol_forall || ( repr -> op ) == fol_exists );
      return ( repr ->  variables ). begin( );
   }

   std::list< function > :: const_iterator
   formula::variables_end( ) const
   {
      ASSERT( ( repr -> op ) == fol_forall || ( repr -> op ) == fol_exists );
      return ( repr -> variables ). end( );
   }


   std::list< term > :: const_iterator 
   formula::subterms_begin( ) const
   {
      ASSERT( ( repr -> op ) == fol_equals || ( repr -> op ) == fol_atom );
      return ( repr -> subterms ). begin( );
   }

   std::list< term > :: const_iterator
   formula::subterms_end( ) const
   {
      ASSERT( ( repr -> op ) == fol_equals || ( repr -> op ) == fol_atom );
      return ( repr -> subterms ). end( );
   }


   std::list< formula > :: const_iterator
   formula::subformulas_begin( ) const
   {
      fol_oper op = ( repr -> op );
      ASSERT( op == fol_false || op == fol_true ||
              op == fol_not ||
	      op == fol_and || op == fol_or ||
	      op == fol_implies || op == fol_equiv ||
	      op == fol_forall || op == fol_exists );
      return ( repr -> subformulas ). begin( );
   }

   std::list< formula > :: const_iterator
   formula::subformulas_end( ) const
   {
      fol_oper op = ( repr -> op );
      ASSERT( op == fol_false || op == fol_true ||
              op == fol_not ||
              op == fol_and || op == fol_or ||
              op == fol_implies || op == fol_equiv ||
              op == fol_forall || op == fol_exists );
      return ( repr -> subformulas ). end( );
   }


   predicate formula::getpredicate( ) const
   {
      ASSERT( ( repr -> op ) == fol_atom );
      return ( repr -> pred ). front( ); 
   }


   // We print in such a way that the result is immune against 
   // every context. Because of this, we don't need to
   // bother about subformulas. 

   std::ostream& operator << ( std::ostream& stream, const formula& f )
   {
      switch( f. repr -> op )
      {
      case fol_false:
         stream << "FALSE";
	 return stream;

      case fol_true:
         stream << "TRUE";
	 return stream;
      
      case fol_not:
         stream << "! ";
	 stream << * f. subformulas_begin( );
	 return stream; 

      case fol_and:
      case fol_or:
      case fol_implies:
      case fol_equiv: 
         stream << "( ";

	 for( std::list< formula > :: const_iterator 
		 s = f. subformulas_begin( ); 
		 s != f. subformulas_end( ); 
		 ++ s )
         {
            if( s != f. subformulas_begin( ))
            {
               switch( f. repr -> op )
               {
               case fol_and:      stream << " /\\ "; break;
               case fol_or:       stream << " \\/ "; break;
               case fol_implies:  stream << " -> ";  break;
               case fol_equiv:    stream << " <-> "; break;
               }
            }

	    stream << *s;
         }
	 stream << " )"; 
	 return stream;

      case fol_exists:
      case fol_forall: 
         if( ( f. repr -> op ) == fol_exists )
            stream << "( <";
	 if( ( f. repr -> op ) == fol_forall )
            stream << "( [";

	 for( std::list< function > :: const_iterator 
		    v = f. variables_begin( );
		    v != f. variables_end( );
		    ++ v )
         {
            if( v != f. variables_begin( ))
               stream << ",";
	    stream << " " << *v;
         }

	 if( ( f. repr -> op ) == fol_exists )
            stream << " > ";
	 if( ( f. repr -> op ) == fol_forall )
            stream << " ] ";

	 stream << *f. subformulas_begin( );
	 stream << " )";
	 return stream; 

      case fol_equals:
	 {
            std::list< term > :: const_iterator t = f. subterms_begin( );
	    stream << *t;
	    stream << " = ";
	    ++ t;
	    stream << *t;
	 }
	 return stream;

      case fol_atom: 
	 stream << f. getpredicate( );
	 if( f. subterms_begin( ) != f. subterms_end( ))
         {
            stream << "(";
	    for( std::list< term > :: const_iterator 
                    t = f. subterms_begin( );
		    t != f. subterms_end( );
		    ++ t )
            {
               if( t != f. subterms_begin( ))
                  stream << ",";
	       stream << " " << *t;
            }
	    stream << " )";
         }
	 return stream;
      }

      ASSERT(false); exit(0); 
   }



   bool formula::zero_arity_only( const std::list< function > & test )
   {
      for( std::list< function > :: const_iterator 
              p = test. begin( );
	      p != test. end( );
	      ++ p )
      {
         if( ( p -> getarity( )) != 0 )
            return false;
      }
      return true;
   }


}


