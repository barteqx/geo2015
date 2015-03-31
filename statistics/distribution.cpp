
// Written by Hans de Nivelle, June 2014.

#include "distribution.h"
#include <stdexcept>
#include "../tostring.h"

statistics::distribution::distribution( 
     std::initializer_list< double > lowerbounds )
{
   for( auto p = lowerbounds. begin( ); p != lowerbounds. end( ); ++ p )
      distr. push_back( *p );
}

statistics::distribution::distribution(
   const std::vector< double > & lowerbounds )
{
   for( auto p = begin( lowerbounds ); p != end( lowerbounds ); ++ p )
      distr. push_back( *p );
}
   
statistics::distribution::distribution(  
     double start, double step, double end )
{
   if( start > end || step <= 0.0 )
      throw std::runtime_error( 
         std::string( "cannot construct distribution( " ) +
            tostring( start ) + ", " + tostring( step ) + ", " +
            tostring( end ) + " )" );
    
   while( start < end )
   {
      distr. push_back( start );
      start += step;
   }
}


void statistics::distribution::clear( ) 
{
   for( auto p = distr. begin( ); p != distr. end( ); ++ p )
   {
      ( p -> sum ) = 0.0;
      ( p -> nrelements ) = 0;
   }
}


void statistics::distribution::insert( double x )
{
   unsigned int i0 = 0;
   unsigned int i1 = distr. size( );

   // We continue search as long as there at least two elements
   // in the interval [ i0 .. i1 ).

   while( i0 + 1 < i1 )
   {
      unsigned int i = ( i0 + i1 ) / 2;
      if( distr[i]. lowerbound <= x )
         i0 = i;
      else
         i1 = i;
   }

   // We now have 0 or 1 element in interval [ i0, i1 ). 

   if( i0 == i1 )
      throw std::runtime_error( "distribution was empty, could not insert" ); 

   if( distr[i0]. lowerbound > x )
      throw std::range_error( std::string( "cannot insert " ) +
              tostring(x) + " because it is too low" );

   distr [i0]. sum += x;
   distr [i0]. nrelements += 1;
}
       

unsigned int statistics::distribution::nrelements( ) const
{
   unsigned int nr = 0;
   for( auto p = distr. begin( ); p != distr. end( ); ++ p )
      nr += ( p -> nrelements );
   return nr;
}


double statistics::distribution::sum( ) const
{
   double s = 0.0;
   for( auto p = distr. begin( ); p != distr. end( ); ++ p )
      s += ( p -> sum );
   return s;
}


std::ostream& statistics::operator << ( std::ostream& stream,
		                        const distribution& d )
{
   double partial_sum = 0.0;
   unsigned int partial_nrelements = 0; 

   double total_sum = d. sum( );
   unsigned int total_nrelements = d. nrelements( );

   stream << "interval       nrelements (cumulative %)";
   stream << "    average (cumulative sum %)\n";
   stream << "-----------------------------------------";
   stream << "---------------------------------\n";

   for( unsigned int i = 0; i < d. distr. size( ); ++ i )
   {
      if( d. distr[i]. nrelements )
      {
         stream << "[ " << d. distr[i]. lowerbound;

         if( i + 1 < d. distr. size( ))
            stream << ", " << d. distr[i+1]. lowerbound << " ) : ";
         else
            stream << ", .... ) : ";  

         double this_sum = d. distr[i]. sum; 
         unsigned int this_nrelements = d. distr[i]. nrelements;  

         stream << "     ";
         stream. width(6); 
         stream << this_nrelements; 

         partial_nrelements += this_nrelements;
         stream << "   (";
         stream. width(6);
         stream << static_cast<double>( partial_nrelements ) / 
                   static_cast<double>( total_nrelements );
         stream << ")";

         stream << "          ";
         stream. width(8); 
         stream << ( this_sum / this_nrelements ); 

         stream << "  (";

         partial_sum += this_sum; 
         stream. width(6); 
         stream << partial_sum / total_sum; 
         stream << ")\n"; 
      }
   }

   if( total_nrelements )
   {
      stream << "total           ";
      stream. width(8); 
      stream << total_nrelements; 
      stream << "                         ";
      stream. width(9); 
      stream << total_sum / total_nrelements;

      stream << "\n\n";
   }
   return stream;
}



