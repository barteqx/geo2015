

// Code written by Hans de Nivelle, November 2005.
// Changed in May 2006. 


#include "false_instance.h"


bool false_instance::computesubst( const model& m )
{
   std::vector< unsigned int > :: const_iterator 
      nr = prem_match. begin( );
   for( rule::prem_iterator p = r. prems_begin( );
        p != r. prems_end( );
	++ p )
   {
      if( nr == prem_match. end( ))
         return false;

      groundatom ga = m. getatom( m. make_atom_iterator( *nr ));

      if( (*p). p == predicate::diseq )
      {
         std::vector< variable > :: const_iterator a = (*p). a1; 
         const element* e1 = s. lookup( *(a++) ); 
         const element* e2 = s. lookup( *a );

	 if( !e1 || !e2 || *e1 == *e2 )
            return false; 

         if( ga. p != predicate::create )
            return false;

	 if( *e1 > *e2 ) 
         {
            if( *(ga.a1) != *e1 )
               return false;
	 }
	 else
         {
            if( *(ga.a1) != *e2 )
               return false;
         }
      }
      else
      {
         bool b = s. match( *p, ga );
         if( !b )
            return false; 
      }
      ++ nr; 
   }
   if( nr != prem_match. end( ))
      return false; 
   return true; 
} 


bool false_instance::computematching( const model& m )
{
   prem_match. clear( );

   for( rule::prem_iterator p = r. prems_begin( );
           p != r. prems_end( );
	   ++ p )
   {
      if( (*p). p == predicate::diseq )
      {
         std::vector< variable > :: const_iterator a = (*p). a1;
	 const element* e1 = s. lookup( *(a++) );
	 const element* e2 = s. lookup( *a );

	 if( ! e1 || ! e2 || *e1 == *e2 )
            return false; 

         element e_max = *e1;
	 if( *e2 > *e1 )
            e_max = *e2;

	 unsigned int atom_nr = 0;
	 for( model::atom_iterator atom = m. atoms_begin( );
                 atom != m. atoms_end( );
		 ++ atom, ++ atom_nr )
         {
            groundatom ga = m. getatom( atom );
	    if( ga. p == predicate::create &&
                *( ga. a1 ) == e_max )
            {
               prem_match. push_back( atom_nr );
	       goto found;
            }
         }
	 return false; 
      }
      else
      {
         // We first check that *p is completely instantiated.

         for( std::vector< variable > :: const_iterator 
                 a = (*p). a1;
		 a != (*p). a2;
		 ++ a )
         {
            if( ! s. lookup( *a ))
               return false;
         }

	 groundatom ga_p = s. apply_on( *p );

         unsigned int atom_nr = 0;
         for( model::atom_iterator atom = m. atoms_begin( );
                 atom != m. atoms_end( );
		 ++ atom, ++ atom_nr )
         {
            groundatom ga = m. getatom( atom );
	    if( ga == ga_p )
            {
               prem_match. push_back( atom_nr ); 
	       goto found; 
            }
         }
	 return false;
      }
   found:
      ;
   }
   return true;
}


unsigned int false_instance::sup_prem_match( ) const
{
   unsigned int res = 0;
   for( std::vector< unsigned int > :: const_iterator 
	   p = prem_match. begin( );
	   p != prem_match. end( );
	   ++ p )
   {
      if( (*p) + 1 > res )
         res = (*p) + 1;
   }
   return res;
}



std::ostream& operator << ( std::ostream& stream, const false_instance& f )
{
   stream << "false_instance( ";
   stream << f. r << ",\n"; 
   stream << f. s << ", "; 
   stream << "atom nrs( ";
   for( std::vector< unsigned int > :: const_iterator 
        it = f. prem_match. begin( );  
        it != f. prem_match. end( );
	++ it )
   {
      if( it != f. prem_match. begin( ))
         stream << ", ";
      stream << *it;
   }
   stream << " )";
   stream << ")";

   return stream;
}



