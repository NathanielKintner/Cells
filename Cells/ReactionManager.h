#pragma once
#include <vector>
#include "Compound.h"
#include "Utilities.h"
#include "ReactionSpace.h"

class Compound;
//if null, failed to react. otherwise, first int is difference in stability and subsequent ints are compounds involved in reaction
int AttemptReaction(ReactionSpace& rs, int threshold, std::vector<Compound*>& reactants);
void AdjustEnergyValues(int stabilityChange, std::vector<Compound*>& reactants);
void DoReaction(Compound& c1, Compound& c2, int x1, int y1, int x2, int y2, int idxidxtoremove);
bool PerformReactionIfGoodEnough(Compound* comp1, Compound* comp2, int threshold, int& stabilityChange, int& ielem1, int& ielem2);
void CallReactionAndCleanUp(Compound* comp1, Compound* comp2, int& stabilityChange, int& ielem1, int& idxidx1, int& ielem2, bool useThreshold, int threshold, ReactionSpace& rs, std::vector<Compound*>& reactants, bool CompRemovedFromIsOrganelleStructure, int idxOfComp1InSolution, int idxOfComp2InSolution);
bool PerformReaction(Compound* comp1, Compound* comp2, int& stabilityChange, int& ielem1, int& idxidx1, int& ielem2, bool useThreshold, int threshold, Compound*& pieces, int& numPieces);
int FindElementIdxByFalling(Compound &c);