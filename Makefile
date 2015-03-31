
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++11 -O5 
CPP = /usr//bin/g++


geo  : Makefile   geometric/signpred.o geometric/lemma.o geometric/existential.o geometric/disjunctive.o geometric/rulebase.o geometric/age.o geometric/disctree.o geometric/unification.o geometric/examples.o geometric/permutationgenerator.o geometric/timepoints.o geometric/rulesystem.o geometric/interpretation.o geometric/false_instance.o geometric/best_false_instances.o geometric/deduction.o geometric/searchstate.o geo.o predicate.o groundatom.o varatom.o rule.o groundsubst.o varsubst.o normalizer.o model.o rulesystem.o false_instance.o varset.o application.o forwardreasoning.o search.o varmatching.o simplification.o parser/token.o parser/inputstream.o parser/errors.o parser/parser.o parser/tptp.o swll/unsignedsubst.o swll/valueset.o swll/occurrencematrix.o swll/vardata.o swll/lemma.o swll/lemmasystem.o swll/searchstate.o firstorder/function.o firstorder/term.o firstorder/formula.o firstorder/sequent.o firstorder/replacement.o firstorder/nnf.o firstorder/simplifications.o firstorder/antiskolemization.o firstorder/factor.o firstorder/geometric.o firstorder/testing.o prolog/expression.o prolog/syntacticdef.o prolog/syntacticdefsforidentifier.o prolog/syntax.o prolog/DFAclassification.o prolog/DFAstate.o prolog/DFAdelta.o prolog/token.o prolog/attribute.o prolog/scanner.o prolog/grammarrule.o prolog/reduction.o prolog/context.o prolog/parsestack.o prolog/reading.o prolog/printing.o prolog/funcpred.o analysis/dependencies.o analysis/dependency_system.o statistics/distribution.o statistics/reporter.o 
	$(CPP) $(Flags) -o geo    geometric/signpred.o geometric/lemma.o geometric/existential.o geometric/disjunctive.o geometric/rulebase.o geometric/age.o geometric/disctree.o geometric/unification.o geometric/examples.o geometric/permutationgenerator.o geometric/timepoints.o geometric/rulesystem.o geometric/interpretation.o geometric/false_instance.o geometric/best_false_instances.o geometric/deduction.o geometric/searchstate.o geo.o predicate.o groundatom.o varatom.o rule.o groundsubst.o varsubst.o normalizer.o model.o rulesystem.o false_instance.o varset.o application.o forwardreasoning.o search.o varmatching.o simplification.o parser/token.o parser/inputstream.o parser/errors.o parser/parser.o parser/tptp.o swll/unsignedsubst.o swll/valueset.o swll/occurrencematrix.o swll/vardata.o swll/lemma.o swll/lemmasystem.o swll/searchstate.o firstorder/function.o firstorder/term.o firstorder/formula.o firstorder/sequent.o firstorder/replacement.o firstorder/nnf.o firstorder/simplifications.o firstorder/antiskolemization.o firstorder/factor.o firstorder/geometric.o firstorder/testing.o prolog/expression.o prolog/syntacticdef.o prolog/syntacticdefsforidentifier.o prolog/syntax.o prolog/DFAclassification.o prolog/DFAstate.o prolog/DFAdelta.o prolog/token.o prolog/attribute.o prolog/scanner.o prolog/grammarrule.o prolog/reduction.o prolog/context.o prolog/parsestack.o prolog/reading.o prolog/printing.o prolog/funcpred.o analysis/dependencies.o analysis/dependency_system.o statistics/distribution.o statistics/reporter.o 

geometric/signpred.o : Makefile   geometric/signpred.cpp   geometric/signpred.h 
	$(CPP) -c $(Flags) geometric/signpred.cpp -o  geometric/signpred.o


geometric/lemma.o : Makefile   geometric/lemma.cpp   geometric/lemma.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/rulebase.h geometric/unification.h geometric/set.h 
	$(CPP) -c $(Flags) geometric/lemma.cpp -o  geometric/lemma.o


geometric/existential.o : Makefile   geometric/existential.cpp   geometric/existential.h geometric/rulebase.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/unification.h geometric/set.h 
	$(CPP) -c $(Flags) geometric/existential.cpp -o  geometric/existential.o


geometric/disjunctive.o : Makefile   geometric/disjunctive.cpp   tostring.h geometric/disjunctive.h geometric/rulebase.h geometric/atom.h geometric/signpred.h geometric/truthval.h geometric/variable.h geometric/element.h geometric/unification.h geometric/set.h 
	$(CPP) -c $(Flags) geometric/disjunctive.cpp -o  geometric/disjunctive.o


geometric/rulebase.o : Makefile   geometric/rulebase.cpp   tostring.h assert.h geometric/rulebase.h geometric/atom.h geometric/signpred.h geometric/truthval.h geometric/variable.h geometric/element.h geometric/unification.h geometric/set.h 
	$(CPP) -c $(Flags) geometric/rulebase.cpp -o  geometric/rulebase.o


geometric/age.o : Makefile   geometric/age.cpp   geometric/age.h 
	$(CPP) -c $(Flags) geometric/age.cpp -o  geometric/age.o


geometric/disctree.o : Makefile   geometric/disctree.cpp   geometric/disctree.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h 
	$(CPP) -c $(Flags) geometric/disctree.cpp -o  geometric/disctree.o


geometric/unification.o : Makefile   geometric/unification.cpp   geometric/unification.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h 
	$(CPP) -c $(Flags) geometric/unification.cpp -o  geometric/unification.o


geometric/examples.o : Makefile   geometric/examples.cpp   geometric/examples.h geometric/rulesystem.h geometric/rule.h geometric/rulebase.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/unification.h geometric/set.h geometric/age.h geometric/lemma.h geometric/disjunctive.h geometric/existential.h geometric/timepoints.h geometric/subst.h geometric/false_instance.h geometric/subststack.h geometric/index/simple.h geometric/range.h 
	$(CPP) -c $(Flags) geometric/examples.cpp -o  geometric/examples.o


geometric/permutationgenerator.o : Makefile   geometric/permutationgenerator.cpp   geometric/permutationgenerator.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/index/simple.h geometric/range.h geometric/permutation.h 
	$(CPP) -c $(Flags) geometric/permutationgenerator.cpp -o  geometric/permutationgenerator.o


geometric/timepoints.o : Makefile   geometric/timepoints.cpp   geometric/timepoints.h 
	$(CPP) -c $(Flags) geometric/timepoints.cpp -o  geometric/timepoints.o


geometric/rulesystem.o : Makefile   geometric/rulesystem.cpp   geometric/deduction.h geometric/rule.h geometric/rulebase.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/unification.h geometric/set.h geometric/age.h geometric/lemma.h geometric/disjunctive.h geometric/existential.h geometric/false_instance.h geometric/subststack.h geometric/subst.h geometric/matched.h geometric/matching/basics.h geometric/index/simple.h geometric/range.h geometric/rulesystem.h geometric/timepoints.h 
	$(CPP) -c $(Flags) geometric/rulesystem.cpp -o  geometric/rulesystem.o


geometric/interpretation.o : Makefile   geometric/interpretation.cpp   geometric/matching/basics.h geometric/subststack.h geometric/subst.h geometric/variable.h geometric/element.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/index/simple.h geometric/range.h geometric/interpretation.h geometric/age.h geometric/permutationgenerator.h geometric/permutation.h geometric/false_instance.h geometric/rule.h geometric/rulebase.h geometric/unification.h geometric/set.h geometric/lemma.h geometric/disjunctive.h geometric/existential.h 
	$(CPP) -c $(Flags) geometric/interpretation.cpp -o  geometric/interpretation.o


geometric/false_instance.o : Makefile   geometric/false_instance.cpp   geometric/false_instance.h geometric/rule.h geometric/rulebase.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/unification.h geometric/set.h geometric/age.h geometric/lemma.h geometric/disjunctive.h geometric/existential.h geometric/subststack.h geometric/subst.h 
	$(CPP) -c $(Flags) geometric/false_instance.cpp -o  geometric/false_instance.o


geometric/best_false_instances.o : Makefile   geometric/best_false_instances.cpp   geometric/best_false_instances.h geometric/false_instance.h geometric/rule.h geometric/rulebase.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/unification.h geometric/set.h geometric/age.h geometric/lemma.h geometric/disjunctive.h geometric/existential.h geometric/subststack.h geometric/subst.h 
	$(CPP) -c $(Flags) geometric/best_false_instances.cpp -o  geometric/best_false_instances.o


geometric/deduction.o : Makefile   geometric/deduction.cpp   geometric/deduction.h geometric/rule.h geometric/rulebase.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/unification.h geometric/set.h geometric/age.h geometric/lemma.h geometric/disjunctive.h geometric/existential.h geometric/false_instance.h geometric/subststack.h geometric/subst.h geometric/matched.h 
	$(CPP) -c $(Flags) geometric/deduction.cpp -o  geometric/deduction.o


geometric/searchstate.o : Makefile   geometric/searchstate.cpp   geometric/matching/basics.h geometric/subststack.h geometric/subst.h geometric/variable.h geometric/element.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/index/simple.h geometric/range.h geometric/searchstate.h geometric/interpretation.h geometric/age.h geometric/permutationgenerator.h geometric/permutation.h geometric/false_instance.h geometric/rule.h geometric/rulebase.h geometric/unification.h geometric/set.h geometric/lemma.h geometric/disjunctive.h geometric/existential.h geometric/timepoints.h geometric/rulesystem.h geometric/atom_hasheq.h geometric/best_false_instances.h geometric/deduction.h geometric/matched.h 
	$(CPP) -c $(Flags) geometric/searchstate.cpp -o  geometric/searchstate.o


geo.o : Makefile   geo.cpp   geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/atom_hasheq.h geometric/matching/basics.h geometric/subststack.h geometric/subst.h geometric/matching/range_restriction.h geometric/deduction.h geometric/rule.h geometric/rulebase.h geometric/unification.h geometric/set.h geometric/age.h geometric/lemma.h geometric/disjunctive.h geometric/existential.h geometric/false_instance.h geometric/matched.h geometric/best_false_instances.h geometric/permutationgenerator.h geometric/permutation.h geometric/examples.h geometric/rulesystem.h geometric/timepoints.h geometric/interpretation.h geometric/searchstate.h geometric/substqueue.h prolog/funcpred.stat prolog/funcpred.h assert.h firstorder/function.stat firstorder/function.h predicate.h predicate.stat application.h false_instance.h groundsubst.h variable.h element.h varatom.h groundatom.h varset.h rule.h model.h partial.h rulesystem.h forwardreasoning.h varsubst.h label.h normalizer.h search.h varmatching.h simplification.h parser/tptp.h prolog/syntax.h prolog/syntacticdefsforidentifier.h prolog/syntacticdef.h prolog/expression.h firstorder/sequent.h firstorder/formula.h firstorder/term.h parser/inputstream.h parser/errors.h parser/token.h parser/parser.h swll/lemmasystem.h swll/lemma.h swll/unsignedsubst.h swll/searchstate.h swll/occurrencematrix.h swll/vardata.h swll/valueset.h firstorder/testing.h statistics/reporter.h statistics/timer.h statistics/distribution.h firstorder/geometric.h firstorder/bindingstack.h firstorder/factor.h firstorder/antiskolemization.h firstorder/replacement.h firstorder/simplifications.h firstorder/nnf.h analysis/dependency_system.h analysis/dependencies.h 
	$(CPP) -c $(Flags) geo.cpp -o  geo.o


predicate.o : Makefile   predicate.cpp   predicate.h assert.h 
	$(CPP) -c $(Flags) predicate.cpp -o  predicate.o


groundatom.o : Makefile   groundatom.cpp   groundatom.h predicate.h assert.h element.h 
	$(CPP) -c $(Flags) groundatom.cpp -o  groundatom.o


varatom.o : Makefile   varatom.cpp   varatom.h predicate.h assert.h variable.h 
	$(CPP) -c $(Flags) varatom.cpp -o  varatom.o


rule.o : Makefile   rule.cpp   groundsubst.h variable.h assert.h element.h varatom.h predicate.h groundatom.h varset.h rule.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h 
	$(CPP) -c $(Flags) rule.cpp -o  rule.o


groundsubst.o : Makefile   groundsubst.cpp   groundsubst.h variable.h assert.h element.h varatom.h predicate.h groundatom.h varset.h 
	$(CPP) -c $(Flags) groundsubst.cpp -o  groundsubst.o


varsubst.o : Makefile   varsubst.cpp   varsubst.h variable.h assert.h varatom.h predicate.h label.h normalizer.h groundsubst.h element.h groundatom.h varset.h 
	$(CPP) -c $(Flags) varsubst.cpp -o  varsubst.o


normalizer.o : Makefile   normalizer.cpp   normalizer.h variable.h assert.h label.h 
	$(CPP) -c $(Flags) normalizer.cpp -o  normalizer.o


model.o : Makefile   model.cpp   geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h model.h element.h assert.h predicate.h groundatom.h geometric/index/simple.h geometric/range.h 
	$(CPP) -c $(Flags) model.cpp -o  model.o


rulesystem.o : Makefile   rulesystem.cpp   simplification.h partial.h assert.h rulesystem.h varatom.h predicate.h variable.h rule.h varset.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h varmatching.h 
	$(CPP) -c $(Flags) rulesystem.cpp -o  rulesystem.o


false_instance.o : Makefile   false_instance.cpp   false_instance.h groundsubst.h variable.h assert.h element.h varatom.h predicate.h groundatom.h varset.h rule.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h model.h 
	$(CPP) -c $(Flags) false_instance.cpp -o  false_instance.o


varset.o : Makefile   varset.cpp   varset.h variable.h assert.h varatom.h predicate.h 
	$(CPP) -c $(Flags) varset.cpp -o  varset.o


application.o : Makefile   application.cpp   statistics/reporter.h statistics/timer.h statistics/distribution.h swll/matchcounter.h swll/searchstate.h swll/occurrencematrix.h variable.h assert.h swll/vardata.h swll/valueset.h swll/lemmasystem.h swll/lemma.h swll/unsignedsubst.h rule.h predicate.h varatom.h varset.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h model.h element.h groundatom.h groundsubst.h varmatching.h partial.h application.h false_instance.h rulesystem.h 
	$(CPP) -c $(Flags) application.cpp -o  application.o


forwardreasoning.o : Makefile   forwardreasoning.cpp   simplification.h partial.h assert.h rulesystem.h varatom.h predicate.h variable.h rule.h varset.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h varmatching.h varsubst.h label.h normalizer.h groundsubst.h element.h groundatom.h model.h forwardreasoning.h false_instance.h 
	$(CPP) -c $(Flags) forwardreasoning.cpp -o  forwardreasoning.o


search.o : Makefile   search.cpp   geometric/permutationgenerator.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h geometric/index/simple.h geometric/range.h geometric/permutation.h statistics/reporter.h simplification.h partial.h assert.h rulesystem.h varatom.h predicate.h variable.h rule.h varset.h varmatching.h application.h false_instance.h groundsubst.h element.h groundatom.h model.h forwardreasoning.h search.h 
	$(CPP) -c $(Flags) search.cpp -o  search.o


varmatching.o : Makefile   varmatching.cpp   varmatching.h variable.h assert.h varatom.h predicate.h varset.h 
	$(CPP) -c $(Flags) varmatching.cpp -o  varmatching.o


simplification.o : Makefile   simplification.cpp   statistics/reporter.h statistics/timer.h statistics/distribution.h forwardreasoning.h variable.h assert.h false_instance.h groundsubst.h element.h varatom.h predicate.h groundatom.h varset.h rule.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h model.h rulesystem.h swll/matchcounter.h swll/searchstate.h swll/occurrencematrix.h swll/vardata.h swll/valueset.h swll/lemmasystem.h swll/lemma.h swll/unsignedsubst.h varmatching.h partial.h simplification.h 
	$(CPP) -c $(Flags) simplification.cpp -o  simplification.o


parser/token.o : Makefile   parser/token.cpp   parser/token.h assert.h 
	$(CPP) -c $(Flags) parser/token.cpp -o  parser/token.o


parser/inputstream.o : Makefile   parser/inputstream.cpp   parser/inputstream.h 
	$(CPP) -c $(Flags) parser/inputstream.cpp -o  parser/inputstream.o


parser/errors.o : Makefile   parser/errors.cpp   parser/errors.h parser/inputstream.h 
	$(CPP) -c $(Flags) parser/errors.cpp -o  parser/errors.o


parser/parser.o : Makefile   parser/parser.cpp   parser/parser.h variable.h assert.h element.h varatom.h predicate.h groundatom.h rule.h varset.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h rulesystem.h model.h firstorder/sequent.h firstorder/formula.h firstorder/term.h firstorder/function.h parser/inputstream.h parser/errors.h parser/token.h 
	$(CPP) -c $(Flags) parser/parser.cpp -o  parser/parser.o


parser/tptp.o : Makefile   parser/tptp.cpp   prolog/printingreading.h prolog/syntax.h assert.h prolog/syntacticdefsforidentifier.h prolog/syntacticdef.h prolog/funcpred.h prolog/expression.h parser/errors.h parser/inputstream.h partial.h parser/tptp.h firstorder/sequent.h firstorder/formula.h predicate.h firstorder/term.h firstorder/function.h 
	$(CPP) -c $(Flags) parser/tptp.cpp -o  parser/tptp.o


swll/unsignedsubst.o : Makefile   swll/unsignedsubst.cpp   swll/unsignedsubst.h variable.h assert.h 
	$(CPP) -c $(Flags) swll/unsignedsubst.cpp -o  swll/unsignedsubst.o


swll/valueset.o : Makefile   swll/valueset.cpp   swll/valueset.h 
	$(CPP) -c $(Flags) swll/valueset.cpp -o  swll/valueset.o


swll/occurrencematrix.o : Makefile   swll/occurrencematrix.cpp   swll/occurrencematrix.h variable.h assert.h 
	$(CPP) -c $(Flags) swll/occurrencematrix.cpp -o  swll/occurrencematrix.o


swll/vardata.o : Makefile   swll/vardata.cpp   swll/vardata.h variable.h assert.h swll/valueset.h 
	$(CPP) -c $(Flags) swll/vardata.cpp -o  swll/vardata.o


swll/lemma.o : Makefile   swll/lemma.cpp   swll/lemma.h variable.h assert.h 
	$(CPP) -c $(Flags) swll/lemma.cpp -o  swll/lemma.o


swll/lemmasystem.o : Makefile   swll/lemmasystem.cpp   swll/lemmasystem.h variable.h assert.h swll/lemma.h swll/unsignedsubst.h 
	$(CPP) -c $(Flags) swll/lemmasystem.cpp -o  swll/lemmasystem.o


swll/searchstate.o : Makefile   swll/searchstate.cpp   swll/searchstate.h swll/occurrencematrix.h variable.h assert.h swll/vardata.h swll/valueset.h swll/lemmasystem.h swll/lemma.h swll/unsignedsubst.h rule.h predicate.h varatom.h varset.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h model.h element.h groundatom.h groundsubst.h varmatching.h partial.h 
	$(CPP) -c $(Flags) swll/searchstate.cpp -o  swll/searchstate.o


firstorder/function.o : Makefile   firstorder/function.cpp   firstorder/function.h predicate.h assert.h 
	$(CPP) -c $(Flags) firstorder/function.cpp -o  firstorder/function.o


firstorder/term.o : Makefile   firstorder/term.cpp   firstorder/term.h firstorder/function.h predicate.h assert.h 
	$(CPP) -c $(Flags) firstorder/term.cpp -o  firstorder/term.o


firstorder/formula.o : Makefile   firstorder/formula.cpp   firstorder/formula.h predicate.h assert.h firstorder/term.h firstorder/function.h 
	$(CPP) -c $(Flags) firstorder/formula.cpp -o  firstorder/formula.o


firstorder/sequent.o : Makefile   firstorder/sequent.cpp   firstorder/sequent.h firstorder/formula.h predicate.h assert.h firstorder/term.h firstorder/function.h 
	$(CPP) -c $(Flags) firstorder/sequent.cpp -o  firstorder/sequent.o


firstorder/replacement.o : Makefile   firstorder/replacement.cpp   firstorder/replacement.h firstorder/function.h predicate.h assert.h firstorder/formula.h firstorder/term.h firstorder/sequent.h firstorder/bindingstack.h 
	$(CPP) -c $(Flags) firstorder/replacement.cpp -o  firstorder/replacement.o


firstorder/nnf.o : Makefile   firstorder/nnf.cpp   firstorder/replacement.h firstorder/function.h predicate.h assert.h firstorder/formula.h firstorder/term.h firstorder/sequent.h firstorder/bindingstack.h firstorder/nnf.h 
	$(CPP) -c $(Flags) firstorder/nnf.cpp -o  firstorder/nnf.o


firstorder/simplifications.o : Makefile   firstorder/simplifications.cpp   firstorder/simplifications.h firstorder/function.h predicate.h assert.h firstorder/formula.h firstorder/term.h firstorder/sequent.h 
	$(CPP) -c $(Flags) firstorder/simplifications.cpp -o  firstorder/simplifications.o


firstorder/antiskolemization.o : Makefile   firstorder/antiskolemization.cpp   firstorder/bindingstack.h firstorder/function.h predicate.h assert.h firstorder/antiskolemization.h firstorder/formula.h firstorder/term.h firstorder/sequent.h 
	$(CPP) -c $(Flags) firstorder/antiskolemization.cpp -o  firstorder/antiskolemization.o


firstorder/factor.o : Makefile   firstorder/factor.cpp   firstorder/replacement.h firstorder/function.h predicate.h assert.h firstorder/formula.h firstorder/term.h firstorder/sequent.h firstorder/bindingstack.h firstorder/factor.h 
	$(CPP) -c $(Flags) firstorder/factor.cpp -o  firstorder/factor.o


firstorder/geometric.o : Makefile   firstorder/geometric.cpp   firstorder/replacement.h firstorder/function.h predicate.h assert.h firstorder/formula.h firstorder/term.h firstorder/sequent.h firstorder/bindingstack.h firstorder/geometric.h rulesystem.h varatom.h variable.h rule.h varset.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h 
	$(CPP) -c $(Flags) firstorder/geometric.cpp -o  firstorder/geometric.o


firstorder/testing.o : Makefile   firstorder/testing.cpp   firstorder/testing.h firstorder/formula.h predicate.h assert.h firstorder/term.h firstorder/function.h 
	$(CPP) -c $(Flags) firstorder/testing.cpp -o  firstorder/testing.o


prolog/expression.o : Makefile   prolog/expression.cpp   prolog/expression.h prolog/funcpred.h assert.h 
	$(CPP) -c $(Flags) prolog/expression.cpp -o  prolog/expression.o


prolog/syntacticdef.o : Makefile   prolog/syntacticdef.cpp   assert.h prolog/syntacticdef.h 
	$(CPP) -c $(Flags) prolog/syntacticdef.cpp -o  prolog/syntacticdef.o


prolog/syntacticdefsforidentifier.o : Makefile   prolog/syntacticdefsforidentifier.cpp   prolog/syntacticdefsforidentifier.h assert.h prolog/syntacticdef.h 
	$(CPP) -c $(Flags) prolog/syntacticdefsforidentifier.cpp -o  prolog/syntacticdefsforidentifier.o


prolog/syntax.o : Makefile   prolog/syntax.cpp   prolog/syntax.h assert.h prolog/syntacticdefsforidentifier.h prolog/syntacticdef.h prolog/funcpred.h 
	$(CPP) -c $(Flags) prolog/syntax.cpp -o  prolog/syntax.o


prolog/DFAclassification.o : Makefile   prolog/DFAclassification.cpp   prolog/DFAclassification.h 
	$(CPP) -c $(Flags) prolog/DFAclassification.cpp -o  prolog/DFAclassification.o


prolog/DFAstate.o : Makefile   prolog/DFAstate.cpp   prolog/DFAstate.h assert.h 
	$(CPP) -c $(Flags) prolog/DFAstate.cpp -o  prolog/DFAstate.o


prolog/DFAdelta.o : Makefile   prolog/DFAdelta.cpp   prolog/DFAclassification.h prolog/DFAdelta.h prolog/DFAstate.h assert.h 
	$(CPP) -c $(Flags) prolog/DFAdelta.cpp -o  prolog/DFAdelta.o


prolog/token.o : Makefile   prolog/token.cpp   assert.h prolog/token.h prolog/expression.h prolog/funcpred.h prolog/syntacticdef.h prolog/attribute.h 
	$(CPP) -c $(Flags) prolog/token.cpp -o  prolog/token.o


prolog/attribute.o : Makefile   prolog/attribute.cpp   prolog/attribute.h prolog/syntacticdef.h prolog/expression.h prolog/funcpred.h assert.h 
	$(CPP) -c $(Flags) prolog/attribute.cpp -o  prolog/attribute.o


prolog/scanner.o : Makefile   prolog/scanner.cpp   prolog/scanner.h assert.h prolog/DFAstate.h prolog/DFAdelta.h prolog/token.h prolog/expression.h prolog/funcpred.h prolog/syntacticdef.h prolog/attribute.h parser/inputstream.h parser/errors.h 
	$(CPP) -c $(Flags) prolog/scanner.cpp -o  prolog/scanner.o


prolog/grammarrule.o : Makefile   prolog/grammarrule.cpp   prolog/grammarrule.h assert.h prolog/syntax.h prolog/syntacticdefsforidentifier.h prolog/syntacticdef.h prolog/funcpred.h prolog/token.h prolog/expression.h prolog/attribute.h 
	$(CPP) -c $(Flags) prolog/grammarrule.cpp -o  prolog/grammarrule.o


prolog/reduction.o : Makefile   prolog/reduction.cpp   prolog/reduction.h prolog/grammarrule.h assert.h prolog/syntax.h prolog/syntacticdefsforidentifier.h prolog/syntacticdef.h prolog/funcpred.h prolog/token.h prolog/expression.h prolog/attribute.h 
	$(CPP) -c $(Flags) prolog/reduction.cpp -o  prolog/reduction.o


prolog/context.o : Makefile   prolog/context.cpp   prolog/context.h assert.h prolog/token.h prolog/expression.h prolog/funcpred.h prolog/syntacticdef.h prolog/attribute.h 
	$(CPP) -c $(Flags) prolog/context.cpp -o  prolog/context.o


prolog/parsestack.o : Makefile   prolog/parsestack.cpp   prolog/parsestack.h assert.h prolog/context.h prolog/token.h prolog/expression.h prolog/funcpred.h prolog/syntacticdef.h prolog/attribute.h 
	$(CPP) -c $(Flags) prolog/parsestack.cpp -o  prolog/parsestack.o


prolog/reading.o : Makefile   prolog/reading.cpp   prolog/scanner.h assert.h prolog/DFAstate.h prolog/DFAdelta.h prolog/token.h prolog/expression.h prolog/funcpred.h prolog/syntacticdef.h prolog/attribute.h parser/inputstream.h parser/errors.h prolog/reduction.h prolog/grammarrule.h prolog/syntax.h prolog/syntacticdefsforidentifier.h prolog/parsestack.h prolog/context.h prolog/printingreading.h partial.h 
	$(CPP) -c $(Flags) prolog/reading.cpp -o  prolog/reading.o


prolog/printing.o : Makefile   prolog/printing.cpp   prolog/DFAclassification.h prolog/context.h assert.h prolog/token.h prolog/expression.h prolog/funcpred.h prolog/syntacticdef.h prolog/attribute.h prolog/printingreading.h prolog/syntax.h prolog/syntacticdefsforidentifier.h parser/errors.h parser/inputstream.h partial.h 
	$(CPP) -c $(Flags) prolog/printing.cpp -o  prolog/printing.o


prolog/funcpred.o : Makefile   prolog/funcpred.cpp   prolog/funcpred.h assert.h 
	$(CPP) -c $(Flags) prolog/funcpred.cpp -o  prolog/funcpred.o


analysis/dependencies.o : Makefile   analysis/dependencies.cpp   analysis/dependencies.h assert.h predicate.h rule.h variable.h varatom.h varset.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h 
	$(CPP) -c $(Flags) analysis/dependencies.cpp -o  analysis/dependencies.o


analysis/dependency_system.o : Makefile   analysis/dependency_system.cpp   analysis/dependency_system.h rulesystem.h varatom.h predicate.h assert.h variable.h rule.h varset.h geometric/index/simple.h geometric/range.h geometric/atom.h geometric/signpred.h geometric/truthval.h tostring.h geometric/variable.h geometric/element.h analysis/dependencies.h 
	$(CPP) -c $(Flags) analysis/dependency_system.cpp -o  analysis/dependency_system.o


statistics/distribution.o : Makefile   statistics/distribution.cpp   tostring.h statistics/distribution.h 
	$(CPP) -c $(Flags) statistics/distribution.cpp -o  statistics/distribution.o


statistics/reporter.o : Makefile   statistics/reporter.cpp   statistics/reporter.h 
	$(CPP) -c $(Flags) statistics/reporter.cpp -o  statistics/reporter.o


