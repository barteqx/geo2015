

// Code written by Hans de Nivelle, July 2006.

// This file contains a few simplifications on first-order sequents
// and formulas. 
// The more interesting transformations are in separate files. 


#ifndef FIRSTORDER_SIMPLIFICATIONS_INCLUDED
#define FIRSTORDER_SIMPLIFICATIONS_INCLUDED   1


#include <map>
#include <list>
#include <vector>


#include "function.h"
#include "formula.h"
#include "sequent.h"


namespace firstorder
{

   namespace simplifications 
   {

      formula flatten( formula f );
         // Contract nested ands, ors, foralls, exists in f. 

      formula truth_constants( formula f );
         // Propagate truth-constants. 

      bool iscontained( function v, term t );
         // True if v occurs in t. 

      bool isfree( function v, formula f );
         // True if v occurs free in f.
	 // This works also for higher-arity functions. 


      // The implementation of antiprenexing is probably overdone, 
      // especially absorb is too complicated. 
      // It should also be observed that antiprenexing is somewhat
      // unsound in the case one allows empty models.

      formula absorb( formula f );
         // Applies on toplevel of f the following simplifications. 
	 // Q ! ( A \/ B ) => Q ! A /\ ! B,
	 // Q ! ( A /\ B ) => Q ! A \/ ! B, 
	 // Q ! ( A -> B ) => Q A /\ ! B, 
	 // Q ( A -> B ) => Q ! A \/ B,
	 // Q ! ! A => A.
	 // Q1 Q2 A => (Q1+Q2) A, if the quantifiers are the same.
         //
	 // At this point, we do not flatten, so it is probably
	 // useful to flatten the formula before, and also after
	 // absorb. 
      
      formula remove_unused( formula f );
         // Formula f should have form [ v1 ... vn ] A or < v1 ... vn > A.
	 // Remove the unused (or repeated) variables from v1 .... vn. 
	 // In case, there are no variables vi left, the quantification
	 // is removed completely. 

      formula distrusedunused( formula f );
         // This is the essential antiprenexing procedure: 
	 // If we have Q v1 ... vn  A1 + ... + Ap, where
	 // + equals /\ or \/, we check for each vi, in which of the
	 // Aj it occurs. If it is a strict subset, we push it inwards.
         // The vi occurring in the smallest subset of { A1, ..., Ap } 
	 // is pushed first.
         // This is done on toplevel only.

      formula distrquantifier( formula f );
         // Apply distributive laws of quantifiers on toplevel. 
         // These are: [v1, ..., vn ] A1 /\ ... /\ Ap =>
	 //   [ v1, ..., vn ] A1 /\ ... [ v1, ..., vn ] Ap.
	 //            <v1, ..., vn > A1 \/ ... \/ Ap =>
	 //   < v1, ..., vn > A1 \/ ... < v1, ..., vn > Ap.
         // Whether it is a good idea to do this, is a thing that
	 // is totally unclear to me.

      formula antiprenex( formula f );
         // Try to limit the range of quantifiers.
         // The best results are obtained on formulas in NNF.
	 // Note that antiprenexing is potentially unsound
	 // if you care about empty models. 

     
      void flatten( sequent& seq );
         // Flatten the formulas in seq. The resulting formulas 
	 // look much better when printed, and the result 
	 // of antiprenexing may be better. 

      void fully_simplify( sequent& seq );
         // Applies all of the above on seq. 

   } 



} 


#endif


