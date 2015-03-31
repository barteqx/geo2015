
// A generic hash function and equality tester for 
// atom<A,S> 
// January 2015.

#ifndef GEOMETRIC_ATOM_HASHEQ_INCLUDED
#define GEOMETRIC_ATOM_HASHEQ_INCLUDED  1

namespace geometric
{

   template< typename A, typename H, size_t S = 6 >
   struct atom_hash
   {
      H h;
      predicate::hash p;

      size_t operator( ) ( const atom<A,S> & at ) const
      {
         size_t val = p( at.p );
         for( size_t i = 0; i != at. getarity( ); ++ i )
         {
            val = val * 1009 + h( at[i] );
         }
         return val;
      }
   };


   template< typename A, typename E, size_t S = 6 >
   struct atom_equal_to 
   {
      E e;  

       
      bool operator( ) ( const atom<A,S> & at1, 
                         const atom<A,S> & at2 ) const
      {
         if( at1.p != at2.p ) return false;
         if( at1.t != at2.t ) return false;

         for( size_t i = 0; i != at1. getarity( ); ++ i )
         {
            if( !e( at1[i], at2[i] ))
               return false;
         }
         return true;
      }
   };

}

#endif  


