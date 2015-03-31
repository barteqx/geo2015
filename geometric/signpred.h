
// June 2014, Hans de Nivelle.
// Added a predicate hasher Dec 2014.

#ifndef GEOMETRIC_SIGNPRED_INCLUDED 
#define GEOMETRIC_SIGNPRED_INCLUDED  1

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

// Predicate and signature are obviously closely related, so we 
// define them in the same file. In earlier versions of Geo,
// signature information was stored in a static field of predicate.
// Since we want thread safety, we cannot do this anymore. 
// A predicate contains a pointer to its signature.

namespace geometric
{
   class signature;

   class predicate
   {
      friend signature;

      const signature* sig;
      size_t index;

      predicate( ) = delete;

      // Only a signature can construct a new predicate: 

      predicate( const signature* sig, size_t index )
         : sig{sig}, index{index}
      { }

   public:
      size_t getarity( ) const;

      bool isequality( ) const;
      bool iscreate( ) const;


      predicate getcreatefromsignature( ) const;
         // Get the create symbol from our signature. It turns
         // out that we need create symbols quite often.


      // Predicates from different signatures cannot be compared
      // in a meaningful fashion.

      bool operator == ( const predicate& p ) const 
         { return index == p. index; }
      bool operator != ( const predicate& p ) const 
         { return index != p. index; }
      bool operator < ( const predicate& p ) const 
         { return index < p. index; }
      bool operator > ( const predicate& p ) const 
         { return index > p. index; }
      bool operator <= ( const predicate& p ) const 
         { return index <= p. index; }
      bool operator >= ( const predicate& p ) const 
         { return index >= p. index; }

      friend std::ostream& operator << ( std::ostream& , const predicate& ); 
      friend std::ostream& operator << ( std::ostream& , const signature& ); 

      struct hash
      {
         size_t operator( ) ( predicate p ) const { return p. index; }
      };

      struct equal_to
      {
         bool operator( ) ( predicate p1, predicate p2 ) const 
            { return p1. index == p2. index; }
      };

   };


   class signature
   {
   public: 
      using nameformat = std::vector< std::string >;
         // A single name has form A1:: ... ::An.

      struct name_arity
      {
         nameformat name;
         size_t arity;
      };

   private: 
      struct pred_equals 
      { bool operator( ) ( const name_arity& , const name_arity& ) const; };

      struct pred_hash 
      { size_t operator( ) ( const name_arity& ) const; }; 

      using maptype = 
      std::unordered_map< name_arity, size_t, pred_hash, pred_equals > ;

      std::vector< name_arity > symbols; 
      maptype lookuptable; 
         // Symbols and lookuptable are inverses of each other. 

   public: 
      const predicate equality;  // The equality symbol.
      const predicate create;    // The create symbol. Purpose of the create
                                 // symbol is to make rules range restricted.

      signature( );
      signature( const signature& ) = delete;
      void operator = ( const signature& ) = delete;

      bool isdefined( const nameformat& name, size_t arity ) const;
      predicate clookup( const nameformat& name, size_t arity ) const; 
      predicate lookup( const nameformat& name, size_t arity ) const
         { return clookup( name, arity ); }

      predicate lookup( const nameformat& name, size_t arity );
         // Non-const lookup creates (name,arity) if it doesn't exist yet. 

      friend std::ostream& operator << ( std::ostream& , const signature& ); 
      friend std::ostream& operator << ( std::ostream& , const predicate& ); 
      friend predicate; 
   };

      
   std::ostream& operator << ( std::ostream& out, const predicate& p );
      // Prints only name. No arity. 
      
   std::ostream& operator << ( std::ostream& out, const signature& s ); 

   inline size_t predicate::getarity( ) const
   {
      return sig -> symbols [ index ]. arity;
   }

   inline bool predicate::isequality( ) const
   {
      return *this == sig -> equality;
   }

   inline bool predicate::iscreate( ) const
   {
      return *this == sig -> create;
   }

   inline predicate predicate::getcreatefromsignature( ) const
   {
      return sig -> create;
   }

}

#endif 


