#pragma once
#include <vector>
#include "Compound.h"
#include "Utilities.h"
#include "ReactionSpace.h"

class Compound;
//if null, failed to react. otherwise, first int is difference in stability and subsequent ints are compounds involved in reaction
//int AttemptReaction(ReactionSpace& rs, int threshold, std::vector<Compound*>& reactants);
//void AdjustEnergyValues(int stabilityChange, std::vector<Compound*>& reactants);
//
//void CallReactionAndCleanUp(Compound* comp1, Compound* comp2, int& stabilityChange, int& ielem1, int& idxidx1, int& ielem2, bool useThreshold, int threshold, ReactionSpace& rs, std::vector<Compound*>& reactants, bool CompRemovedFromIsOrganelleStructure, int idxOfComp1InSolution, int idxOfComp2InSolution);
//bool PerformReaction(Compound* comp1, Compound* comp2, int& stabilityChange, int& ielem1, int& idxidx1, int& ielem2, bool useThreshold, int threshold, Compound*& pieces, int& numPieces);

bool RandomlyReactInSolution(ReactionSpace& rs, int threshold);
void AdjustEnergyValues(Compound* c1, Compound* c2, int& c1instabilityChange, int& c2instabilityChange);
bool RandomlyReactIfGoodEnough(Compound* c1, Compound* c2, int threshold, int& c1instabilityChange, int& c2instabilityChange);
void CreateRandomReactantGroup(Compound* c1, Compound* diff);
bool PerformReactionIfGoodEnough(Compound* c1, Compound* c2, Compound* diff, int threshold, int& c1instabilityChange, int& c2instabilityChange);
int DoReactionAndReport(Compound& c1, Compound& c2, Compound& diff, int& c1instabilityChange, int& c2instabilityChange);
void DoReaction(Compound& c1, Compound& c2, Compound& diff);