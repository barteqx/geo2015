
// Written by Hans de Nivelle, January 2015.

#ifndef GEOMETRIC_RULE_INCLUDED
#define GEOMETRIC_RULE_INCLUDED  1

#include <chrono>
#include "rulebase.h"
#include "age.h"
#include "lemma.h"
#include "disjunctive.h"
#include "existential.h"

namespace geometric
{

   class rule;

   // Infbase is the base class from which inference rules must inherit:

   struct infbase
   {
      age a;

      virtual ~infbase( ) { };

      virtual infbase* clone( ) const = 0;
      virtual bool check( ) const 
      {
         std::cerr << "checking of this rule was not implemented\n";
         return false;
      }
         // Checks correctness of the inference. (Only this step.)

      // Get the parents of this inference:

      virtual std::vector<rule> getparents( ) const = 0;

      virtual void print( std::ostream& ) const = 0;
         // Prints the inference in a short fashion.
         // It should print a mnemonic of the rule + 
         // keys of the parents, but it should not recurse. 
   };

   // Inference rule for rules that are part of the code:

   struct partofcode : public infbase
   {
      partofcode* clone( ) const override
         { return new partofcode{ *this }; }

      bool check( ) const override { return true; }
         // We have no choice than to trust the person who typed the code. 

      std::vector<rule> getparents( ) const override
      {
         return { };
      }

      void print( std::ostream& out ) const override
         { out << "partofcode"; }

   };


   class rule
   {
      rulebase* rul;
      infbase* inf;  
      size_t* counter;

   public: 
      rule( const rulebase& r, const infbase& i = partofcode( ))
         : rul{ r. clone( ) }, inf{ i. clone( ) }, counter{ new size_t{1} }
      { }

      rule( const rule& r )
         : rul{ r.rul }, inf{ r.inf }, counter{ r.counter }
      {
         ++ ( *counter );
      }

      void operator = ( rule&& r )
      {
         std::swap( rul, r. rul );
         std::swap( inf, r. inf );
         std::swap( counter, r. counter );
      }

      void operator = ( const rule& r )
      {
         *this = rule(r); 
      }

      ~rule( ) 
      {
         if( -- (*counter) == 0 ) 
         { 
            delete rul; delete inf; delete counter;
         }
      }

      const infbase& getinference( ) const { return *inf; }

      const std::vector<varatom> & getpassive( ) const 
         { return rul -> getpassive( ); }

      set<variable> getfreevariables( ) const 
         { return rul -> getfreevariables( ); } 

      void sortpassive( ) 
         { rul -> sortpassive( ); }

      bool isdisjunctive( ) const 
         { return dynamic_cast< disjunctive* > ( rul ); }
      bool isexistential( ) const
         { return dynamic_cast< existential* > ( rul ); } 
      bool islemma( ) const 
         { return dynamic_cast< lemma* > ( rul ); } 

      const disjunctive& getdisjunctive( ) const
         { return dynamic_cast< disjunctive& > ( *rul ); }
      const existential& getexistential( ) const
         { return dynamic_cast< existential& > ( *rul ); }
      const lemma& getlemma( ) const
         { return dynamic_cast< lemma& > ( *rul ); } 

      void printkey( std::ostream& out ) const
      {
         out << std::hex;
         out << std::uppercase;
         out << ( reinterpret_cast<size_t>(rul) & 0XFFFF );
         out << std::nouppercase; 
         out << std::dec; 
      } 
   
      friend std::ostream& operator << ( std::ostream& , const rule& ); 
   };

   inline std::ostream& operator << ( std::ostream& out, const rule& r ) 
   {
      out << "(#"; r.printkey( out ); out << ") ";
      r.inf -> print( out ); 
      out << ", " << r.inf -> a; 
      out << ", references = " << *(r.counter);
      out << ", passive size = " << r.rul -> passivesize( ) << ":\n";
      out << "   "; 
      r.rul -> print( out );
      return out;
   }

}

#endif

