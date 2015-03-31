

// Written by Hans de Nivelle, July 2006.


// In this file, the final conversion from factored (CUDEN)
// formulas to geometric formulas is implemented. 


#include "geometric.h"
#include "replacement.h"
#include <cstdlib>


namespace firstorder
{


   unsigned int geometric::nr_positive_atoms( formula f )
   {
      switch( f. getoperator( ))
      {
      case fol_false:
      case fol_true:
      case fol_equals:
         return 0;
   
      case fol_not:
         {
            formula sub = * f. subformulas_begin( );
	    if( sub. getoperator( ) != fol_atom &&
                sub. getoperator( ) != fol_equals )
            {
               std::cout << "Formula is not in NNF\n";
	       ASSERT( false ); exit(0);
            }
	    return 0; 
         }

      case fol_atom:
         return 1;

      case fol_and:
      case fol_or:
      case fol_forall:
      case fol_exists:
         {
            unsigned int nr = 0;
	    for( std::list< formula > :: const_iterator
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               nr += nr_positive_atoms( *p );
            }
	    return nr;
         }

      default:
         std::cout << "Formula is not in NNF\n";
         ASSERT( false ); exit(0);
      }
   }


   formula geometric::replace_existential( bindingstack& bs, formula f,
		                           sequent& seq, 
		                           std::list< formula > & definitions )
   {
   
      switch( f. getoperator( ))
      {
      case fol_false:
      case fol_true:
      case fol_atom:
      case fol_equals:
         return f;

      case fol_not:
         {
	    formula sub = * f. subformulas_begin( );
	    if( sub. getoperator( ) != fol_atom &&
		sub. getoperator( ) != fol_equals )
            {
	       std::cout << "Formula is not in NNF\n";
	       ASSERT( false ); exit(0);
            }
            return f;
         }

      case fol_and:
         // Actually, and should not occur anymore in the formula at this
	 // moment.

      case fol_or:
	 {
            std::list< formula > sub;

            for( std::list< formula > :: const_iterator 
                    p = f. subformulas_begin( );
		    p != f. subformulas_end( );
		    ++ p )
            {
               sub. push_back( 
			   replace_existential( bs, *p, seq, definitions ));
            }
            if( f. getoperator( ) == fol_and )
               return formula::make_and( sub );
            else
               return formula::make_or( sub );
         }

      case fol_forall:
         {
            std::list< function > varlist;
            for( std::list< function > :: const_iterator 
                    v = f. variables_begin( );
		    v != f. variables_end( );
		    ++ v )
            {
               varlist. push_back( *v );
	       bs. bind( *v );
            }

	    formula body = replace_existential( bs, * f. subformulas_begin( ),
			                        seq, definitions );

	    for( std::list< function > :: const_iterator
                    v = f. variables_begin( );
		    v != f. variables_end( );
		    ++ v )
            {
               bs. release( *v );
            }

            return formula::make_forall( varlist, body );
         }

      case fol_exists:
         {

            std::map< function, bool > varmap =
                   replacement::freevariables( bs, f );
            std::list< function > varlist =
                   replacement::converttolist( varmap );

	    formula at = replacement::make_atom( varlist, seq );

	    definitions. push_back( replacement::positive_definition( at, f ));

            return at;
	 }

      default:
         std::cout << "formula is not in NNF\n";
	 ASSERT( false ); exit(0);
      }
   }


   ///////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////



   namespace
   {

      class varmapping 
      {
      
         std::map< function, std::list< variable > > repr; 
            // Maps the quantified variables away to internal variables. 
            // The last member in repr is the actual binding.

         variable next;

      public: 
         void bind( function v );
         void release( function v );
            // Add or release a replacement for v. This is done in
	    // a stack-like fashion. 

         bool isbound( function v ) const; 
	 variable lookup( function v ) const; 
         bool isempty( ) const;

         friend std::ostream& operator << ( std::ostream& stream,
		                            const varmapping& vm ); 
      };

      std::ostream& operator << ( std::ostream& stream,
		                  const varmapping& vm );



      void varmapping::bind( function v )
      {
         ASSERT( v. getarity( ) == 0 );
         repr [ v ]. push_back( next );
	 next ++ ; 
      }


      void varmapping::release( function v )
      {
         std::map< function, std::list< variable > > :: iterator 
		p = repr. find(v);
      
         ASSERT( p != repr. end( ));
         ASSERT( p -> second. size( ));
         p -> second. pop_back( ); 
         if( p -> second. size( ) == 0 )
            repr. erase(p);
      }


      bool varmapping::isbound( function v ) const
      {
         std::map< function, std::list< variable > > :: const_iterator 
		       p = repr. find(v);
         return p != repr. end( ) && ( p -> second. size( ) != 0 );
      }


      variable varmapping::lookup( function v ) const
      {
         std::map< function, std::list< variable > > :: const_iterator
                       p = repr. find(v);
	 ASSERT( p != repr. end( ) && p -> second. size( ) != 0 );
         return p -> second. back( );
      }


      bool varmapping::isempty( ) const
      {
         return repr. size( ) == 0; 
      }


      std::ostream& operator << ( std::ostream& stream,
		                  const varmapping& vm )
      {
         stream << "Varmapping:\n"; 
         stream << "   next = " << vm. next << "\n";

         for( std::map< function, std::list< variable > > :: const_iterator
                 p = vm. repr. begin( );
	         p != vm. repr. end( );
	         ++ p )
         {
            stream << "   " << ( p -> first ) << " -> { ";
	    for( std::list< variable > :: const_iterator
                    q = p -> second. begin( );
		    q != p -> second. end( );
		    ++ q )
            {
               if( q != p -> second. begin( ))
                  stream << ", ";
	       stream << *q;
            }
	    stream << " }\n";
         }
         return stream;
      }


   } 



   ///////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////



   geometric::pseudo_atom::pseudo_atom( predicate p )
      : p(p) 
   {
   }
 

   unsigned int geometric::pseudo_atom::getarity( ) const
   {
      return p. getarity( ); 
   }


   bool geometric::pseudo_atom::check( ) const
   {
      return p. getarity( ) == args. size( );
   }


   void geometric::pseudo_atom::subst( variable v1, variable v2 )
   {
      ASSERT( check( ));

      for( std::list< variable > :: iterator
              v = args. begin( );
	      v != args. end( );
	      ++ v ) 
      {
         if( *v == v1 )
            *v = v2;
      }
   }


   bool geometric::pseudo_atom::subsumes( const pseudo_atom& at ) const
   {
      ASSERT( check( ) && at. check( ));

      if( at. p == predicate::create )
      {
         if( p == predicate::diseq )
            return false;

         for( std::list< variable > :: const_iterator 
                 q = args. begin( );
		 q != args. end( );
		 ++ q )
         {
            if( *q == at. args. front( ))
               return true;
         }
	 return false;
      }

      if( at. p == predicate::diseq )
      {
         if( p != predicate::diseq )
            return false;

	 if( args. front( ) == at. args. front( ) &&
             args. back( ) == at. args. back( ))
            return true;
	 if( args. front( ) == at. args. back( ) &&
             args. back( ) == at. args. front( ))
            return true;
         return false;
      }

      return *this == at;
   }


   bool geometric::operator == ( const geometric::pseudo_atom& at1,
		                 const geometric::pseudo_atom& at2 )
   {
      ASSERT( at1. check( ) && at2. check( ));

      if( at1. p != at2. p )
         return false;

      std::list< variable > :: const_iterator p1 = at1. args. begin( );
      std::list< variable > :: const_iterator p2 = at2. args. begin( );

      while( p1 != at1. args. end( ))
      {
         if( *p1 != *p2 )
            return false;

	 ++ p1;
	 ++ p2; 
      }
      return true;
   }


   std::ostream& geometric::operator << ( std::ostream& stream, 
		                          const geometric::pseudo_atom& at )
   {
      ASSERT( at. check( )); 
     
      if( at. p == predicate::create )
      {
         stream << at. p << " " << at. args. front( );
	 return stream;
      }

      if( at. p == predicate::diseq )
      {
         stream << at. args. front( ) << " " << at. p << " ";
	 stream << at. args. back( );
	 return stream;
      }

      stream << at. p;
      if( at. args. size( ))
      {
         stream << "(";
	 for( std::list< variable > :: const_iterator
                 p = at. args. begin( );
		 p != at. args. end( );
		 ++ p )
         {
            if( p != at. args. begin( ))
               stream << ",";
	    stream << *p; 
         }
	 stream << ")";
      }

      return stream;
   }



   ///////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////


   bool geometric::pseudo_rule::check( ) const
   {
      for( std::list< pseudo_atom > :: const_iterator
              p = prem. begin( );
	      p != prem. end( );
	      ++ p )
      {
         if( ! p -> check( ))
            return false; 
      }

      for( std::list< pseudo_atom > :: const_iterator
              p = conc. begin( );
	      p != conc. end( );
	      ++ p )
      {
         if( ! p -> check( ))
            return false;
      }

      if( conc. size( ) != 0 && conc. front( ). p == predicate::create )
      {
         if( conc. size( ) != 2 )
            return false; 
         if( conc. back( ). p == predicate::create ||
             conc. back( ). p == predicate::diseq )
            return false;
	 return true; 
      }
      else
      {
         for( std::list< pseudo_atom > :: const_iterator
                 p = conc. begin( );
		 p != conc. end( );
		 ++ p )
         {
            if( p -> p == predicate::create ||
                p -> p == predicate::diseq )
               return false; 
         }
	 return true; 
      }
   }


   bool geometric::pseudo_rule::isexistential( ) const
   {
      return conc. size( ) == 2 && 
             conc. front( ). p == predicate::create;
   }


   // A rule is a tautology if the intersection of prem and conc
   // is non-empty, or the premisse contains a trivial
   // disequality V != V. In that case, there is no point
   // in generating the rule. 

   bool geometric::pseudo_rule::istautology( ) const
   {
      for( std::list< pseudo_atom > :: const_iterator
              p = prem. begin( );
	      p != prem. end( );
	      ++ p )
      {
         if( p -> p == predicate::diseq )
         {
            std::list< variable > :: const_iterator v = p -> args. begin( );
	    variable v1 = * ( v ++ );
	    variable v2 = * ( v ++ );

	    if( v1 == v2 )
               return true;
         }
      }

      if( ! isexistential( ))
      {
         for( std::list< pseudo_atom > :: const_iterator
                 p = prem. begin( );
	         p != prem. end( );
	         ++ p )
         {
            for( std::list< pseudo_atom > :: const_iterator
                    q = conc. begin( );
		    q != conc. end( );
		    ++ q )
            {
               if( p -> subsumes( *q ))
                  return true;
            }
         }
      }

      return false;
   }



   void geometric::pseudo_rule::simplify( ) 
   {
      for( std::list< geometric::pseudo_atom > :: iterator
              p = prem. begin( );
	      p != prem. end( );
	      ++ p )
      {
         std::list< geometric::pseudo_atom > :: iterator q = prem. begin( );
	 while( q != prem. end( )) 
         {
            if( p != q && p -> subsumes(*q)) 
            {
               std::list< geometric::pseudo_atom > :: iterator r = q;
	       ++ q; 
               prem. erase(r);
            }
	    else
               ++ q;
         }
      }

      if( ! isexistential( ))
      {
         for( std::list< geometric::pseudo_atom > :: iterator
                 p = conc. begin( );
	         p != conc. end( );
	         ++ p )
         {
            std::list< geometric::pseudo_atom > :: iterator q = conc. begin( );
	    while( q != conc. end( ))
            {
               if( p != q && p -> subsumes(*q))
               {
                  std::list< geometric::pseudo_atom > :: iterator r = q; 
		  ++ q;
                  conc. erase(r);
               }
	       else
                  ++ q;
            }
         }
      }  
   }



   void geometric::pseudo_rule::insert_to( rulesystem& rs ) const 
   {
      ASSERT( check( ));

      std::vector< predicate > predicates_base;
      std::vector< variable > arguments_base;

      rule r = rule( predicates_base, arguments_base );

      // First we add non-disequality premisses: 

      for( std::list< pseudo_atom > :: const_iterator
              p = prem. begin( );
	      p != prem. end( );
	      ++ p )
      {
         if( p -> p != predicate::diseq )
         {
            std::vector< variable > args;
	    for( std::list< variable > :: const_iterator 
                    q = p -> args. begin( );
	            q != p -> args. end( );
	            ++ q )
            {
               args. push_back(*q);  
            }
	    r. addpremisse( varatom( p -> p, args ));
         }
      }

      // Then the disequality premisses: 

      for( std::list< pseudo_atom > :: const_iterator
              p = prem. begin( );
	      p != prem. end( );
	      ++ p )
      {
         if( p -> p == predicate::diseq )
         {
            std::vector< variable > args;
	    for( std::list< variable > :: const_iterator
                    q = p -> args. begin( );
		    q != p -> args. end( );
		    ++ q )
            {
               args. push_back(*q);
            }
	    r. addpremisse( varatom( p -> p, args ));
         }
      }

      if( isexistential( ))
      {
         std::vector< variable > args;
	 for( std::list< variable > :: const_iterator
                 q = conc. back( ). args. begin( );
		 q != conc. back( ). args. end( );
		 ++ q )
         {
            args. push_back(*q);
         }
	 r. setexistential( conc. front( ). args. front( ),
		            varatom( conc. back( ). p, args ));
      }
      else
      {
         for( std::list< pseudo_atom > :: const_iterator
                 p = conc. begin( );
		 p != conc. end( );
		 ++ p )
         {
            std::vector< variable > args;
	    for( std::list< variable > :: const_iterator
                    q = p -> args. begin( );
		    q != p -> args. end( );
		    ++ q )
            {
               args. push_back(*q);
            }
	    r. addconclusion( varatom( p -> p, args ));
         }
      }

      std::vector< rulesystem::binary_step > derivation; 
      rs. addrule( r, derivation, false ); 
   }


   std::ostream& geometric::operator << ( std::ostream& stream,
		                          const geometric::pseudo_rule& r )
   {
      stream << "{ ";
      for( std::list< pseudo_atom > :: const_iterator 
              p = r. prem. begin( );
	      p != r. prem. end( );
	      ++ p )
      {
         if( p != r. prem. begin( ))
            stream << ", ";
	 stream << *p; 
      }
      stream << " } -> { ";

      for( std::list< pseudo_atom > :: const_iterator
              p = r. conc. begin( );
	      p != r. conc. end( );
	      ++ p )
      {
         if( p != r. conc. begin( ))
            stream << ", ";
	 stream << *p;
      }
      stream << " }";
      return stream;
   }



   namespace
   {

      geometric::pseudo_atom convert_atom( formula f,
		                           const varmapping& vm )
      {
         ASSERT( f. getoperator( ) == fol_atom )
        
         geometric::pseudo_atom result = 
	       geometric::pseudo_atom( f. getpredicate( ));

	 for( std::list< term > :: const_iterator 
                 p = f. subterms_begin( );
		 p != f. subterms_end( );
	         ++ p )
         {
            function f = p -> getfunction( );
	    ASSERT( f. getarity( ) == 0 );
	    ASSERT( vm. isbound( f ));

            result. args. push_back( vm. lookup(f) );
         }
	 return result;
      }


      // The equality is secretly converted into a disequality.


      geometric::pseudo_atom convert_equality( formula f,
		                               const varmapping& vm )
      {
         ASSERT( f. getoperator( ) == fol_equals );

	 geometric::pseudo_atom result =
                geometric::pseudo_atom( predicate::diseq );

	 for( std::list< term > :: const_iterator
                 p = f. subterms_begin( );
		 p != f. subterms_end( );
		 ++ p )
         {
            function f = p -> getfunction( );
	    ASSERT( f. getarity( ) == 0 );
	    ASSERT( vm. isbound( f ));

	    result. args. push_back( vm. lookup(f) );
         }
         return result;
      }


      geometric::pseudo_atom make_create( function f, const varmapping& vm )
      {
         geometric::pseudo_atom result =
                geometric::pseudo_atom( predicate::create );

	 ASSERT( f. getarity( ) == 0 );
	 ASSERT( vm. isbound(f) );

	 result. args. push_back( vm. lookup(f) );
	 return result;
      }




      // This function makes a geometric rule from the formula f.
      // f is essentially a disjunction, the geometric rule r looks
      // like a sequent. 
      // - positive atoms in f go to the conclusion of r.
      // - negative atoms in f go to the premisse of r.
      // - positive equalties go, as disequalities, to the premisse of r.
      // - negative equalities are stored separately in equalities.
      //      They have to be substituted away later.
      // - the false constant goes nowhere.
      // - existential quantifiers go to the conclusion of r. In that case,
      //      there must exist no other positive atom. 
      // - universal quantifiers generate create atoms that go to
      //      the premisse of r. (Most will be deleted later)

      void firstconversion( formula f,
	                    varmapping& vm,                  
	                    geometric::pseudo_rule& r,  
			    std::list< geometric::pseudo_atom > & equalities )
      {
         switch( f. getoperator( ))
         {
         case fol_true:
            std::cout << "cannot handle TRUE\n";
	    ASSERT( false ); exit(0);

         case fol_false:
            return;

         case fol_atom:
            r. conc. push_back( convert_atom( f, vm ));
	    return;

         case fol_equals:
            r. prem. push_back( convert_equality( f, vm ));
	    return;

         case fol_not:
            {
               formula sub = * f. subformulas_begin( );

	       if( sub. getoperator( ) != fol_atom && 
                   sub. getoperator( ) != fol_equals )
               {
                  std::cout << "formula is not in NNF\n";
		  ASSERT( false ); exit(0);
               }

	       if( sub. getoperator( ) == fol_equals )
                  equalities. push_back( convert_equality( sub, vm ));
               else
                  r. prem. push_back( convert_atom( sub, vm ));
               return;
            }

         case fol_or:
            {
               for( std::list< formula > :: const_iterator
                       p = f. subformulas_begin( );
		       p != f. subformulas_end( );
		       ++ p )
               {
                  firstconversion( *p, vm, r, equalities );
               }
               return;
            }

         case fol_forall:
            {
               for( std::list< function > :: const_iterator
                       v = f. variables_begin( );
		       v != f. variables_end( );
		       ++ v )
               {
                  vm. bind( *v );
		  r. prem. push_back( make_create( *v, vm )); 
               }

	       formula body = * f. subformulas_begin( );
               firstconversion( body, vm, r, equalities ); 

	       for( std::list< function > :: const_iterator
                       v = f. variables_begin( );
		       v != f. variables_end( );
		       ++ v )
               {
                  vm. release( *v );
               }
	       return;
            }

         case fol_exists:
            {
               std::list< function > :: const_iterator p = 
		             f. variables_begin( );
               
	       function v = * ( p ++ );
	       if( p != f. variables_end( ))
               {
                  std::cout << "Iterated existential quantifier\n";
		  ASSERT( false ); exit(0);
               }

               vm. bind(v);

	       formula body = * f. subformulas_begin( );
               if( body. getoperator( ) != fol_atom )
               {
                  std::cout << "body of existential quantifier is not atom\n";
		  ASSERT( false ); exit(0);
               }

               r. conc. push_back( make_create( v, vm ));
               r. conc. push_back( convert_atom( body, vm ));

               vm. release(v);
	       return; 
            }

         default:
            std::cout << "Formula is not factored (CUDEN) \n";
	    ASSERT( false ); exit(0);
         }
      }


   } 


   void geometric::addtherules( sequent& seq, 
			        rulesystem& rs ) 
   {
      for( std::list< formula > :: const_iterator
              p = seq. formulas. begin( );
	      p != seq. formulas. end( );
	      ++ p )
      {
         unsigned int nr_pos = nr_positive_atoms(*p);
      
         std::list< formula > expansion;
         if( nr_pos > 1 )
         {
            bindingstack bs;
            formula f = replace_existential( bs, *p, seq, expansion ); 
            ASSERT( bs. isempty( )); 
	    expansion. push_front(f); 
         }
	 else
            expansion. push_front(*p);

         for( std::list< formula > :: const_iterator
                 q = expansion. begin( );
	         q != expansion. end( );
	         ++ q )
         {
            pseudo_rule r; 
	    std::list< geometric::pseudo_atom > equalities;

	    varmapping vm; 
	    firstconversion( *q, vm, r, equalities );
	    ASSERT( vm. isempty( ));

	    // Now we must substitute the equalities away:
            // This is a somewhat dangerous activity, because
	    // it may result in quadratic time complexity.
	    // As far as I can see, the other steps in the
	    // transformation are n.log(n).

	    while( equalities. size( ))
            {
               std::list< variable > :: const_iterator 
		      arg = equalities. front( ). args. begin( ); 
	       variable v1 = *( arg ++ );
	       variable v2 = *( arg ++ );

               equalities. pop_front( );

	       for( std::list< geometric::pseudo_atom > :: iterator
                       p = r. prem. begin( );
		       p != r. prem. end( );
		       ++ p )
               {
                  p -> subst( v1, v2 );
               }

	       for( std::list< geometric::pseudo_atom > :: iterator
                       p = r. conc. begin( );
		       p != r. conc. end( );
		       ++ p )
               {
                  p -> subst( v1, v2 );
               }

	       for( std::list< geometric::pseudo_atom > :: iterator
                       p = equalities. begin( );
		       p != equalities. end( );
		       ++ p )
               {
                  p -> subst( v1, v2 );
               }
            }

	    ASSERT( r. check( ));
	    r. simplify( );

	    if( ! r. istautology( ))
            {
               r. insert_to( rs );
            }
         }
      }
   }


} 



