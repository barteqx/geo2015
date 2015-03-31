
// Written by Hans de Nivelle 26.09.2014.

#ifndef GEOMETRIC_PERMUTATION_INCLUDED
#define GEOMETRIC_PERMUTATION_INCLUDED  1

namespace geometric
{

   // A permutation is a bijection from
   // [ 0 ... N-1 ) to ( 0 ... N-1 ).
   //
   // It is possible to have N = 0, in which case the permutation is
   // defined nowhere. 
   // We represent permutations by functions of form 
   // ( A*i + B ) mod N, which should be good enough for our purpose.
   // A must be coprime to N. Taking A = 1 or A = N-1 is
   // always safe. If N = 0, then the values of A and B do not matter.
   

   struct permutation
   {
      const unsigned int A;
      const unsigned int B;
      const unsigned int N;

      // Default is identity permutation:

      permutation( unsigned int N )
         : A{1},
           B{0},
           N{N} 
      { }
 
      permutation( unsigned int A, unsigned int B, unsigned int N )
         : A{A}, 
           B{B},
           N{N}
      { }


      // i must be < N!

      unsigned int operator( ) ( unsigned int i ) const 
      {
         if( i >= N ) throw std::runtime_error( "permutation out of range" );

         return (( A * i ) + B ) % N;
      }


      // Permute a vector. Size should be equal to size of permutation. 


      template< typename T >
      std::vector<T> operator( ) ( const std::vector<T> & vect ) const
      {
         std::vector<T> res;
         for( unsigned int i = 0; i < vect. size( ); ++ i )
         {
            res. push_back( vect[ (*this)(i) ] );
         }
         return res;
      }
 
      unsigned int size( ) const { return N; } 

   };


   inline std::ostream& operator << ( std::ostream& out,
                                      const permutation& p )
   {
      // One can either print the function or the permutation itself

#if 0 
      out << "P(i) = ( " << p.A << "*i + " << p.B << " ) mod " << p. N;
#else
      out << '(';
      for( unsigned int i = 0; i < p. N; ++ i )
      {
         if( i != 0 ) out << ',';
         out << p(i);
      }
      out << ')';
#endif
      return out;
   }

}

#endif

