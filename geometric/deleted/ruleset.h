
// Written by Hans de Nivelle, June 2014.

#ifndef GEOMETRIC_RULESET_INCLUDED
#define GEOMETRIC_RULESET_INCLUDED   1

// A ruleset is a set of rules. It is not assumed that
// all rules are stored in a single rule set.
// Rules can be moved between different rulesets.
// As a consequence, a rule cannot be identified by its position in
// a ruleset. Instead, a rule is identified by a number, which
// is stored with the rule. Don't use permanent pointers to rules
// or iterators, because rules can be freely moved. 


#include "timepoints.h"
#include "rule.h"

namespace geometric
{

   // The possible ways in which a rule can take part in the
   // derivation of a new rule:

   enum class rule_use { 
      disj_res_active,     // The formula that extended the model.
      disj_res_passive,    // The formula that rejected the extension.
      ext_res_active,      // The formula that extended the model.
      ext_res_passive,     // The formula that rejected the extension. 
      copy,                // Rule is copied without change.
      inst                 // Rule was instantiated 
   };
    
   std::ostream& operator << ( std::ostream& , rule_use r );


   // Number of the rule, and the way it was used.
 
   struct parent
   {
      rule_use u;
      unsigned int n;

      parent( rule_use u, unsigned int n )
         : u{u}, n{n}
      { } 
   };

   std::ostream& operator << ( std::ostream& , parent p ); 
      // Prints p.u(p.n). 
 
   struct annotated_rule 
   {
      unsigned int number; 
      rule r; 

      timepoints active; 
         // Timepoints on which the rule was active. 
      timepoints applications{ 50 };
         // Time points on which the rule extended or closed an interpretation. 
      timepoints simpl{ 50 };  
         // Time points on which the rule simplified or subsumed another rule. 

      std::vector< parent > parents; 

      explicit annotated_rule( unsigned int number );
      annotated_rule( unsigned int number, rule&& r ); 
      annotated_rule( unsigned int number, rule&& r, 
                      std::vector< parent > && parents );
   };

   std::ostream& operator << ( std::ostream& , const annotated_rule& ar );

 
   class ruleset
   {
      std::vector< annotated_rule > rules;
         // The rules are always sorted by their number. 

      unsigned int findindex( unsigned int nr ) const;
         // Returns the smallest number i, s.t. 
         // for all i' with i <= i' < size( ), we have 
         // rules [ i' ]. number <= number. 

      struct insert_exists : public std::runtime_error 
      {
         insert_exists( )
            : std::runtime_error( 
                "tried to overwrite an existing rule in rulesystem" )
         { }
      };

   public:
      using iterator = std::vector< annotated_rule > :: iterator;
      using const_iterator = std::vector< annotated_rule > :: const_iterator;
         // All the fields of annotated_rule are available.

      iterator insert( annotated_rule&& ar );

      iterator find( unsigned int nr );
      const_iterator find( unsigned int nr ) const;
         // Return end( ) if nr does not exist.

      iterator begin( ) { return rules. begin( ); }
      const_iterator cbegin( ) const { return rules. cbegin( ); }
      const_iterator begin( ) const { return rules. cbegin( ); }

      iterator end( ) { return rules. end( ); }
      const_iterator cend( ) const { return rules. cend( ); }
      const_iterator end( ) const { return rules. cend( ); }

      iterator moveto( iterator it, ruleset& rs );
         // Move contents of it to rs. Returns the position in rs.

      unsigned int size( ) const { return rules. size( ); } 
   };

   std::ostream& operator << ( std::ostream& , const ruleset& );    
}

#endif

