
// Code written by Hans de Nivelle, November 2005. 
// Modified by Hans de Nivelle, February 2007.


#include "model.h"

#include "geometric/atom.h"


bool model::addatom( groundatom at, 
		     unsigned int rule_nr, 
		     const std::vector< unsigned int > & ps, 
		     unsigned int alternative,
		     unsigned int nr_alternatives )
{

   // We first check that atom is new. If not, we don't add:

   for( atom_iterator 
	   p = atoms_begin( );
           p != atoms_end( );
	   ++ p )
   {
      if( getatom(p) == at )
         return false;
   }

   // We compute the level and the weight of the new atom.

   unsigned int level = 0;
   unsigned int weight = 1;

   unsigned int nr = 0; 
   for( atom_iterator
           p = atoms_begin( );
	   p != atoms_end( );
	   ++ p, ++ nr )
   {
      for( std::vector< unsigned int > :: const_iterator
              q = ps. begin( );
	      q != ps. end( );
	      ++ q )
      {
         if( *q == nr )
         {
            if( level < getlevel(p) + 1 )
               level = getlevel(p) + 1;
            weight += getweight(p);
         }
      }
   }

   atoms. push_back( 
       atom_in_model( 
	          at. p, 
	          arguments. size( ), arguments. size( ) + at.p.getarity( ),
		  rule_nr, 
		  parents. size( ), parents. size( ) + ps. size( ),
                  closure_counter,
		  level, weight, 
		  alternative, nr_alternatives ));

   for( std::vector< element > :: const_iterator a = at. a1;
        a != at. a2;
	++ a )
   {
      arguments. push_back( *a );
   }

   for( std::vector< unsigned int > :: const_iterator p = ps. begin( );
        p != ps. end( );
	++ p )
   {
      parents. push_back(*p);
      ASSERT( *p < atoms. size( ) - 1 ); 
         // Parents should come before their children. 
   }

   return true; 
}


bool model::canbeadded( groundatom at ) const
{
   if( at. p != predicate::create )
   {
      for( std::vector< element > :: const_iterator 
	      a = at. a1;
	      a != at. a2;
	      ++ a )
      {
         for( atom_iterator 
	         i = atoms_begin( );
                 i != atoms_end( );
	         ++ i )
         {
            if( getatom(i). p == predicate::create &&
                *( getatom(i). a1 ) == *a )
            {
               goto found;
            }
         }
         return false;
      found:
         ;
      }
   }
   return true;
}


void model::backtrack( unsigned int s0 )
{
   ASSERT( atoms. size( ) >= s0 );

   if( s0 < atoms. size( ))
   {
      arguments. erase( arguments. begin( ) + atoms [ s0 ]. args1, 
		        arguments. end( ) ); 
      parents. erase( parents. begin( ) + atoms [ s0 ]. pars1, 
		      parents. end( ) ); 
      atoms. erase( atoms. begin( ) + s0, atoms. end( ) );
   }
}


unsigned int model::nr_elements( ) const
{
   unsigned nr = 0;

   for( model::atom_iterator a = atoms_begin( );
        a != atoms_end( );
	++ a )
   {
      if( getatom(a). p == predicate::create ) 
         ++ nr;
   }
   return nr;
}


geometric::index::standardsimple< element >
model::make_simple_index( geometric::signature& sig ) const
{
   geometric::index::standardsimple<element> res;

   for( auto at = atoms_begin( ); at != atoms_end( ); ++ at )
   {
      groundatom gr = getatom(at);

      // We convert to new representation:

      geometric::atom< element > gr2 =
         geometric::atom<element>( sig. lookup( { gr.p. getstring( ) },
                                                  gr.p. getarity( )),
                                geometric::truthval::T );

      unsigned int i = 0;
      for( auto arg = gr.a1; arg != gr.a2; ++ arg )
      {
         gr2 [i++] = *arg;
      }
      res. insert( gr2 );
   }
   return res;
}


std::ostream& operator << ( std::ostream& stream, const model& m )
{
   stream << "Interpretation:\n\n";

   unsigned int atomcntr = 0;

   for( model::atom_iterator a = m. atoms_begin( );
        a != m. atoms_end( );
	++ a )
   {
      stream << atomcntr << " : ";
      stream << m. getrulenr(a); 
      stream << "(";

      for( model::parent_iterator p = m. parents_begin(a);
           p != m. parents_end(a);
	   ++ p )
      {
         if( p != m. parents_begin(a) )
            stream << ","; 
         stream << *p;
      }
      stream << ") ";
      stream << "(" << m. getalternative(a) << "/";
      stream << m. getnralternatives(a) << ")    "; 

      stream << "[ " << m. gettimestamp(a) << "; ";
      stream << "L" << m. getlevel(a) << ", ";
      stream << "W" << m. getweight(a) << " ]      "; 

      ++ atomcntr;

      stream << m. getatom(a) << "\n"; 
   }
   stream << "\n"; 
   stream << "The model contains " << m. nr_elements( ) << " elements\n"; 
   stream << "It has been closed " << m. closure_counter << " times\n";

   return stream; 
}


