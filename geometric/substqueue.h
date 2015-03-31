
// Written by Hans de Nivelle, September 2014.

#ifndef GEOMETRIC_SUBSTQUEUE_INCLUDED
#define GEOMETRIC_SUBSTQUEUE_INCLUDED  1

// A substqueue is a sequence of assignments that can forget
// from the front. This kind of substitution is required by
// discrimination tree lookup in order to avoid exponentiality. 
// A substqueue can be used both for usual substitutions, and for
// renamings. For the representation of renamings, there are inverse 
// lookup functions.
// SUBSTQUEUE IS NOT FINISHED. WE DONT HAVE THE HASH FUNCTIONS YET.

namespace geometric
{

   template< typename D > 
   class substqueue
   {

      struct assignment
      {
         variable v;
         D d;  

         assignment( variable v, D d )
            : v{v}, d{d}
         { }
      };

   // Use auto:
      struct state
      {
         size_t b;
         size_t l;

         state( size_t b, size_t l )
            : b{b}, l{l}
         { }
      };

      size_t base;
      std::vector< assignment > repr;

   public:
      substqueue( ) : base{0} { }

      substqueue( const substqueue& s )
         : base{0}, repr{ s. repr. begin( ) + s. base, s. repr. end( )} { }

      void operator = ( const substqueue& s )
      {
         base = 0;  // Because we don't copy the invisible part.
         repr = { s. repr. begin( ) + s. base, s. repr. end( )};
                    // Use iterators to construct a new vector. 
      }
 
         // The two last operators copy only the visible part.

      void clear( ) { base = 0; repr. clear( ); }
      state getstate( ) const { return { base, repr. size( ) }; } 
      void restore( const state& s );

      const D* clookup( variable v ) const; 
      const D* lookup( variable v ) const { return clookup(v); } 

      const variable* inv_clookup( D d ) const;
      const variable* inv_lookup( D d ) const { return inv_clookup(d); }
         // Only meaningful on renamings.

      void push_back( variable v, D d ) { repr. push_back( { v, d } ); }
         // Don't reassign, it won't work!

      template< unsigned int S >
      bool match( const varatom& from, const atom<D,S> & into );

      template< unsigned int S > 
      bool match_rename( const varatom& at1, const atom<D,S> & at2 );

      unsigned int size( ) const { return repr. size( ) - base; }
         // Size of visible part.

      void limit_size( unsigned int s )
      {
         if( repr. size( ) - base > s )
            base = repr. size( ) - s;
      }
         // Limit our size to s by forgetting things from the front, if 
         // required. Limiting to a size greater than our current size
         // has no effect.

      void print( std::ostream& out ) const;  
   };


   template< typename D >
   void substqueue<D> :: restore( const state& s )
   {
      base = s. b;
      while( repr. size( ) > s.l )
         repr. pop_back( );
   }


   template< typename D >
   const D* substqueue<D> :: clookup( variable v ) const
   {
      for( auto p = repr. begin( ) + base; p != repr. end( ); ++ p )
      {
         if( ( p -> v ) == v ) 
            return & ( p -> d );
      }
      return nullptr;
   }


   template< typename D >
   const variable* substqueue<D> :: inv_clookup( D d ) const
   {
      for( auto p = repr. begin( ) + base; p != repr. end( ); ++ p )
      {  
         if( ( p -> d ) == d )
            return & ( p -> v );
      }
      return nullptr;
   }


   template< typename D >
   void substqueue<D>::print( std::ostream& out ) const
   {
      out << "substqueue(";
      auto p1 = repr. begin( ) + base;
      auto p2 = repr. end( );
      for( auto p = p1; p != p2; ++ p )
      {
         if( p != p1 )
            out << ", ";
         else
            out << " ";
         out << ( p -> v ) << " := " << ( p -> d );
      }
      out << " )";
   }


   template< typename D >
   inline std::ostream& 
   operator << ( std::ostream& out, const substqueue<D> & s )
   {
      s. print( out );
      return out;
   }
   
}

#endif 


