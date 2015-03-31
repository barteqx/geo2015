% Probably a good problem. Geo cannot solve it.

%------------------------------------------------------------------------------
% File     : SWV555-1.010 : TPTP v4.0.1. Released v4.0.0.
% Domain   : Software Verification
% Problem  : Store inverse (t1_pp_sf_ai_00010)
% Version  : Especial.
% English  : If the arrays resulting from swapping elements of array a with
%            the elements of array b occurring in the same positions are equal,
%            then a and b must have been equal to begin with.

% Refs     : [Arm08] Armando (2008), Email to Geoff Sutcliffe
%          : [AB+09] Armando et al. (2009), New Results on Rewrite-based Sa
% Source   : [Arm08]
% Names    : storeinv_t1_pp_sf_ai_00010_001 [Arm08]

% Status   : Unsatisfiable
% Rating   : 0.89 v4.0.1, 0.75 v4.0.0
% Syntax   : Number of clauses     :   47 (   1 non-Horn;  46 unit;  45 RR)
%            Number of atoms       :   48 (  48 equality)
%            Maximal clause size   :    2 (   1 average)
%            Number of predicates  :    1 (   0 propositional; 2-2 arity)
%            Number of functors    :   58 (  55 constant; 0-3 arity)
%            Number of variables   :    7 (   2 singleton)
%            Maximal term depth    :    3 (   2 average)

% Comments :
%------------------------------------------------------------------------------
cnf(a1,axiom,
    ( select(store(A,I,E),I) = E )).

cnf(a2,axiom,
    ( I = J
    | select(store(A,I,E),J) = select(A,J) )).

cnf(hyp0,hypothesis,
    ( a_44 = store(a1,i1,e_43) )).

cnf(hyp1,hypothesis,
    ( a_46 = store(a2,i1,e_45) )).

cnf(hyp2,hypothesis,
    ( a_48 = store(a_44,i2,e_47) )).

cnf(hyp3,hypothesis,
    ( a_50 = store(a_46,i2,e_49) )).

cnf(hyp4,hypothesis,
    ( a_52 = store(a_48,i3,e_51) )).

cnf(hyp5,hypothesis,
    ( a_54 = store(a_50,i3,e_53) )).

cnf(hyp6,hypothesis,
    ( a_56 = store(a_52,i4,e_55) )).

cnf(hyp7,hypothesis,
    ( a_58 = store(a_54,i4,e_57) )).

cnf(hyp8,hypothesis,
    ( a_60 = store(a_56,i5,e_59) )).

cnf(hyp9,hypothesis,
    ( a_62 = store(a_58,i5,e_61) )).

cnf(hyp10,hypothesis,
    ( a_64 = store(a_60,i6,e_63) )).

cnf(hyp11,hypothesis,
    ( a_66 = store(a_62,i6,e_65) )).

cnf(hyp12,hypothesis,
    ( a_68 = store(a_64,i7,e_67) )).

cnf(hyp13,hypothesis,
    ( a_70 = store(a_66,i7,e_69) )).

cnf(hyp14,hypothesis,
    ( a_72 = store(a_68,i8,e_71) )).

cnf(hyp15,hypothesis,
    ( a_74 = store(a_70,i8,e_73) )).

cnf(hyp16,hypothesis,
    ( a_76 = store(a_72,i9,e_75) )).

cnf(hyp17,hypothesis,
    ( a_78 = store(a_74,i9,e_77) )).

cnf(hyp18,hypothesis,
    ( a_80 = store(a_76,i10,e_79) )).

cnf(hyp19,hypothesis,
    ( a_82 = store(a_78,i10,e_81) )).

cnf(hyp20,hypothesis,
    ( e_43 = select(a2,i1) )).

cnf(hyp21,hypothesis,
    ( e_45 = select(a1,i1) )).

cnf(hyp22,hypothesis,
    ( e_47 = select(a_46,i2) )).

cnf(hyp23,hypothesis,
    ( e_49 = select(a_44,i2) )).

cnf(hyp24,hypothesis,
    ( e_51 = select(a_50,i3) )).

cnf(hyp25,hypothesis,
    ( e_53 = select(a_48,i3) )).

cnf(hyp26,hypothesis,
    ( e_55 = select(a_54,i4) )).

cnf(hyp27,hypothesis,
    ( e_57 = select(a_52,i4) )).

cnf(hyp28,hypothesis,
    ( e_59 = select(a_58,i5) )).

cnf(hyp29,hypothesis,
    ( e_61 = select(a_56,i5) )).

cnf(hyp30,hypothesis,
    ( e_63 = select(a_62,i6) )).

cnf(hyp31,hypothesis,
    ( e_65 = select(a_60,i6) )).

cnf(hyp32,hypothesis,
    ( e_67 = select(a_66,i7) )).

cnf(hyp33,hypothesis,
    ( e_69 = select(a_64,i7) )).

cnf(hyp34,hypothesis,
    ( e_71 = select(a_70,i8) )).

cnf(hyp35,hypothesis,
    ( e_73 = select(a_68,i8) )).

cnf(hyp36,hypothesis,
    ( e_75 = select(a_74,i9) )).

cnf(hyp37,hypothesis,
    ( e_77 = select(a_72,i9) )).

cnf(hyp38,hypothesis,
    ( e_79 = select(a_78,i10) )).

cnf(hyp39,hypothesis,
    ( e_81 = select(a_76,i10) )).

cnf(hyp40,hypothesis,
    ( e_84 = select(a1,i_83) )).

cnf(hyp41,hypothesis,
    ( e_85 = select(a2,i_83) )).

cnf(hyp42,hypothesis,
    ( i_83 = sk(a1,a2) )).

cnf(hyp43,hypothesis,
    ( a_80 = a_82 )).

cnf(goal,negated_conjecture,
    ( e_84 != e_85 )).

%------------------------------------------------------------------------------
