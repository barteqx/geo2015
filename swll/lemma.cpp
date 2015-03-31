
// Code written by Hans de Nivelle, April 2006.


#include "lemma.h"

namespace swll
{

   lemma::lemma( std::vector< atom > & atoms_base,
		 const std::list< atom > & p )
      : atoms_base( atoms_base ),
        atom1( atoms_base. size( )), atom2( atoms_base. size( ) + p. size( ))
   {
      for( std::list< atom > :: const_iterator 
              q = p. begin( );
	      q != p. end( );
	      ++ q )
      {
         atoms_base. push_back(*q); 
      }
   }


   lemma lemma::clone_into( std::vector< atom > & atoms_base ) const 
   {
      ASSERT( & atoms_base != & ( this -> atoms_base ) );
      lemma res = lemma( atoms_base, 
		         atoms_base. size( ),
			 atoms_base. size( ) + 
			 ( this -> atom2 ) - ( this -> atom1 ));

      for( unsigned int i = ( this -> atom1 );
              i != ( this -> atom2 );
	      ++ i )
      {
         atoms_base. push_back( ( this -> atoms_base ) [i] );
      }

      return res; 
   }


   lemma lemma::resolve( std::vector< atom > & atoms_base,
		         const std::list< lemma > & parents, 
			 const std::list< variable > & hypotheses,
			 const std::list< atom > & conflicts )
   {
      lemma res( atoms_base );

      for( std::list< atom > :: const_iterator 
              p = conflicts. begin( );
	      p != conflicts. end( );
	      ++ p )
      {
         res. addatom( *p );
      }

      for( std::list< lemma > :: const_iterator 
	      par = parents. begin( );
	      par != parents. end( );
	      ++ par )
      {
         for( unsigned int i = ( par -> atom1 ); i != ( par -> atom2 ); ++ i )
         {
            variable v = ( par -> atoms_base [i]. v ); 
            for( std::list< variable > :: const_iterator 
                    h = hypotheses. begin( );
		    h != hypotheses. end( );
		    ++ h )
            {
               if( v == *h ) goto skipped;
            }
            res. addatom( par -> atoms_base [i] );
         skipped:
	    ;
         }
      }

      return res;
   }

   lemma lemma::resolve2( std::vector< atom > & atoms_base,
		          const std::list< lemma > & parents,
			  const std::list< variable > & hypotheses,
			  const std::list< atom > & conflicts )
   {
      std::cout << "computing lemma resolvent: \n";
      std::cout << "conflicting assignments: [";
      for( std::list< lemma::atom > :: const_iterator 
              p = conflicts. begin( );
	      p != conflicts. end( );
	      ++ p )
      {
         if( p != conflicts. begin( ))
            std::cout << ", ";
	 std::cout << "(" << (p -> v) << ":=" << (p -> val) << ")";
      }
      std::cout << "]\n";

      std::cout << "assumed variables: [";
      for( std::list< variable > :: const_iterator
              p = hypotheses. begin( );
	      p != hypotheses. end( );
	      ++ p )
      {
         if( p != hypotheses. begin( ))
            std::cout << ",";
	 std::cout << *p;
      }
      std::cout << "]\n";

      std::cout << "parents:\n";
      for( std::list< lemma > :: const_iterator 
              p = parents. begin( );
	      p != parents. end( );
	      ++ p )
      {
         std::cout << "   " << *p << "\n";
      }
      std::cout << "\n";

      lemma lem = resolve( atoms_base, parents, hypotheses, conflicts );
      std::cout << " --> generated: " << lem << "\n\n\n";
      return lem;
   }


   bool lemma::subsumes( const lemma& other ) const 
   {
      for( std::vector< atom > :: const_iterator 
              p = atoms_base. begin( ) + atom1; 
	      p != atoms_base. begin( ) + atom2; 
	      ++ p )
      {
         for( std::vector< atom > :: const_iterator
                 q = other. atoms_base. begin( ) + other. atom1;
		 q != other. atoms_base. begin( ) + other. atom2;
		 ++ q )
         {
            if( p -> v == q -> v && p -> val == q -> val ) goto found;
         }
	 return false;
      found:
	 ; 
      }
      return true; 
   }


   std::ostream& operator << ( std::ostream& stream, const lemma& l )
   {
      for( lemma::atom_iterator 
	      p = l. atoms_begin( ); 
	      p != l. atoms_end( );
	      ++ p )
      {
         if( p != l. atoms_begin( ))
            stream << " /\\ ";
	 stream << (p -> v) << " := " << (p -> val); 
      }
      stream << " -> FALSE";

      return stream;
   }

} 





