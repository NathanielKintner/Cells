#include "ReactionManager.h"
#include "Utilities.h"

#include <cstdio>
#include <iostream>
//int AttemptReaction(ReactionSpace& initial, int threshold, std::vector<Compound*>& reactants)
//{
//    if (initial.ContainsReactant())
//    {
//        int idx1 = initial.GetReactantKey();
//        int idx2 = initial.GetReactantKey();
//        //a tenth of the time, just try to react with yourself (i.e. break apart)
//        if (rand() % 10 == 0)
//        {
//            idx2 = idx1;
//        }
//        Compound* comp1 = initial.GetReactantWithKey(idx1);
//        Compound* comp2 = initial.GetReactantWithKey(idx2);
//        int stability = 0;
//        int ielem1;
//        int ielem2;
//
//        int idxidx1 = rand() % comp1->filledIndices.size();
//        int idxidx2 = rand() % comp2->filledIndices.size();
//        ielem1 = comp1->filledIndices[idxidx1];
//        ielem2 = comp2->filledIndices[idxidx2];
//        int emptyIdxs[8];
//        int spaces = comp2->getUnPopulatedNeighborsIndices(ielem2, emptyIdxs);
//        if (spaces == 0)
//        {
//            return false;
//        }
//        ielem2 = emptyIdxs[rand() % spaces];
//        CallReactionAndCleanUp(comp1, comp2, stability, ielem1, idxidx1, ielem2, true, threshold, initial, reactants, false, idx1, idx2);
//        return stability;
//    }
//}
//
//
//
//bool PerformReaction(Compound* comp1, Compound* comp2, int& instabilityChange, int& ielem1, int& idxidx1, int& ielem2, bool useThreshold, int threshold, Compound* & pieces, int& numPieces)
//{
//    int x1, y1, x2, y2;
//    comp1->TranslateFromIdxToXY(ielem1, x1, y1);
//    comp2->TranslateFromIdxToXY(ielem2, x2, y2);
//
//    // current instability
//    int stability = 0;
//    if (comp1 != comp2)
//        //stability = comp1.GetTotalStability() + comp2.GetTotalStability();
//        stability = comp1->GetStabilityAtPoint(x1, y1) + comp2->GetStabilityAtPoint(x2, y2);
//    else
//        stability = comp1->GetTotalInstability();
//    DoReaction(*comp1, *comp2, x1, y1, x2, y2, idxidx1);
//    // subtract the instability of the new, so if the result is positive we have increased stability
//    if (comp1 != comp2)
//        //stability = comp1.GetTotalStability() + comp2.GetTotalStability() - stability;
//        stability = (comp1->GetStabilityAtPoint(x1, y1) + comp2->GetStabilityAtPoint(x2, y2)) - stability;
//    else
//        stability = comp1->GetTotalInstability() - stability;
//
//    if (useThreshold && stability < threshold)
//    {
//        //undo the reaction
//        DoReaction(*comp2, *comp1, x2, y2, x1, y1, comp2->filledIndices.size() - 1);
//        ielem1 = comp1->TranslateFromXYToIdx(x1,y1);
//        return false;
//    }
//    ielem1 = comp1->TranslateFromXYToIdx(x1, y1);
//    instabilityChange = stability;
//    //if comp1 has been consumed entirely
//    if (comp1->mass == 0)
//    {
//        //one line energy calculation
//        comp2->internalEnergy += instabilityChange + comp1->internalEnergy;
//        //theres nothing else to do... no splitting, no identity checks, nothing
//    }
//    else
//    {
//        pieces = comp1->SplitCompound(ielem1, numPieces);
//        std::vector<Compound*> reactants;
//        if (comp1 == comp2)
//        {
//            //if this is the same compound twice
//            if (numPieces == 0)
//            {
//                //if we didnt break, just put the single compound in ONCE
//                reactants.emplace_back(comp1);
//            }
//            else
//            {
//                //just put in the pieces
//                for (int i = 0; i < numPieces; ++i)
//                {
//                    reactants.emplace_back(&(pieces[i]));
//                }
//            }
//
//        }
//        else
//        {
//            //there were two compounds
//            if (numPieces == 0)
//            {
//                //if there was no splitting, put in comp1
//                reactants.emplace_back(comp1);
//            }
//            else
//            {
//                //if there was splitting, just put in the pieces of comp1
//                for (int i = 0; i < numPieces; ++i)
//                {
//                    reactants.emplace_back(&(pieces[i]));
//                }
//            }
//            //either way, put in comp2
//            reactants.emplace_back(comp2);
//        }
//        //distribute energy evenly
//        if (numPieces != 0)
//        {
//            instabilityChange += comp1->internalEnergy;
//        }
//        AdjustEnergyValues(instabilityChange, reactants);
//        int negativeEnergy = 0;
//        for (Compound* c : reactants)
//        {
//            if (c->internalEnergy < 0)
//            {
//                negativeEnergy += c->internalEnergy;
//                c->internalEnergy = 0;
//            }
//        }
//        //always dump negative energy into comp2
//        comp2->internalEnergy += negativeEnergy;
//    }
//    return true;
//}

//void CallReactionAndCleanUp(Compound* comp1, Compound* comp2, int& instabilityChange, int& ielem1, int& idxidx1, int& ielem2, bool useThreshold, int threshold, ReactionSpace& rs, std::vector<Compound*>& reactants, bool CompRemovedFromIsOrganelleStructure, int idxOfComp1InSolution, int idxOfComp2InSolution)
//{
//    Compound* pieces = nullptr;
//    int numPieces = 0;
//
//    //std::cout << "before" << "\n";
//    //comp1->PrintPhysicalStructure();
//    //comp2->PrintPhysicalStructure();
//    bool didreaction = PerformReaction(comp1, comp2, instabilityChange, ielem1, idxidx1, ielem2, useThreshold, threshold, pieces, numPieces);
//
//    //this is the section where we adjust the identity of organelle structures, move things in and out of solution, etc
//    if (!didreaction)
//    {
//        //we did nothing... lets just
//        return;
//    }
//    if (numPieces == 0 && comp1->mass != 0)
//    {
//        //nothing broke up or disappeared, soo lets
//        return;
//    }
//    if (CompRemovedFromIsOrganelleStructure && comp1->mass == 0)
//    {
//        //nothing in solution will change as a result of this... we will just need to kill the organelle when we
//        return;
//    }
//    //now that we've gotten the do-nothing cases out of the way....
//    if (CompRemovedFromIsOrganelleStructure && numPieces != 0)
//    {
//        //the organelle's structure broke! oh no!
//        //lets make sure that whatever becomes the new structure is the largest leftover piece
//        //comp1 is the structure of the organelle, so....
//        int largest = -1;
//        int size = -1;
//        for (int i = 0; i < numPieces; i++)
//        {
//            if (pieces[i].mass > size)
//            {
//                largest = i;
//                size = pieces[i].mass;
//            }
//        }
//        //set the structure to that piece
//        *comp1 = pieces[largest];
//        //now, get rid of that piece! we dont want to put it in solution. we'll just take one off the end of the list
//        //(this also works if the largest piece IS the one at the end of the list)
//        pieces[largest] = pieces[numPieces - 1];
//        numPieces -= 1;
//    }
//    //now we need to add/remove things to/from solution.    
//    if (CompRemovedFromIsOrganelleStructure)
//    {
//        //if the thing we removed from was an organelle, we wont delete anything from solution, so we give a negative key
//        rs.ResolveSituation(pieces, numPieces, -1);        
//    }
//    else
//    {
//        //we may delete a compound!
//        rs.ResolveSituation(pieces, numPieces, idxOfComp1InSolution);
//    }
//    delete[] pieces;
//    return;
//}

//bool PerformReactionIfGoodEnough(Compound* comp1, Compound* comp2, int threshold, int& instabilityChange, int& ielem1, int& ielem2)
//{
//    int idxidx1 = rand() % comp1->filledIndices.size();
//    int idxidx2 = rand() % comp2->filledIndices.size();
//    ielem1 = comp1->filledIndices[idxidx1];
//    ielem2 = comp2->filledIndices[idxidx2];
//    int emptyIdxs[8];
//    int spaces = comp2->getUnPopulatedNeighborsIndices(ielem2, emptyIdxs);
//    if (spaces == 0)
//    {
//        return false;
//    }
//    ielem2 = emptyIdxs[rand() % spaces];
//    int x1, y1, x2, y2;
//    comp1->TranslateFromIdxToXY(ielem1, x1, y1);
//    comp2->TranslateFromIdxToXY(ielem2, x2, y2);
//
//    // current instability
//    int stability = 0;
//    if (comp1 != comp2)
//        //stability = comp1.GetTotalStability() + comp2.GetTotalStability();
//        stability = comp1->GetStabilityAtPoint(x1, y1) + comp2->GetStabilityAtPoint(x2, y2);
//    else
//        stability = comp1->GetTotalStability();
//    DoReaction(*comp1, *comp2, x1, y1, x2, y2, idxidx1);
//    // subtract the instability of the new, so if the result is positive we have increased stability
//    if (comp1 != comp2)
//        //stability = comp1.GetTotalStability() + comp2.GetTotalStability() - stability;
//        stability = (comp1->GetStabilityAtPoint(x1, y1) + comp2->GetStabilityAtPoint(x2, y2)) - stability;
//    else
//        stability = comp1->GetTotalStability() - stability;
//
//    if (stability < threshold)
//    {
//        //undo the reaction
//        DoReaction(*comp2, *comp1, x2, y2, x1, y1, comp2->filledIndices.size() - 1);
//        ielem1 = comp1->TranslateFromXYToIdx(x1, y1);
//        return false;
//    }
//    ielem1 = comp1->TranslateFromXYToIdx(x1, y1);
//    instabilityChange = stability;
//    return true;
//}



//may eventually be changed to actually traverse the structure of a molecule and find a location on the outside.
//however, the number of cycles that this would require to complete may simply make this untenable
//fast and bad may be better than good but really slow? the goal is complex behaviour, if going faster lets us
//achieve more progress, im all about it
//int FindElementIdxByFalling(Compound& c)
//{
//    int ret;
//    while (c.composition[c.filledIndices[(ret = rand() % c.filledIndices.size())]].red == 0)
//    {
//        FastDelete(c.filledIndices, ret);
//    }
//    ret = c.filledIndices[ret];
//    return ret;
//}


bool RandomlyReactInSolution(ReactionSpace& rs, int threshold)
{
    if (rs.ContainsReactant())
    {
        int idx1 = rs.GetReactantKey();
        Compound* comp1 = rs.GetReactantWithKey(idx1);
        int idx2 = rs.GetReactantKey();
        Compound* comp2;
        //a tenth of the time, just try to react with yourself (i.e. break apart)
        if (rand() % 10 == 0)
        {
            comp2 = new Compound();
            idx2 = -1;
        }
        else
        {
            comp2 = rs.GetReactantWithKey(idx2);
        }
        if (idx1 == idx2)
        {
            //we return because doing chemistry with yourself causes infinite energy bugs
            //NOTE: we cannot get here if idx2 has been set to -1! this is important, if we could then
            //this would cause a memory leak.
            return false;
        }
        int stability = 0;
        int c1instabilityChange = 0;
        int c2instabilityChange = 0;
        if (RandomlyReactIfGoodEnough(comp1, comp2, threshold, c1instabilityChange, c2instabilityChange))
        {
            AdjustEnergyValues(comp1, comp2, c1instabilityChange, c2instabilityChange);
        }
        else
        {
            if (idx2 == -1)
            {
                delete comp2;
            }
            return false;
        }
        if (comp1->elementCount == 0)
        {
            comp2->internalEnergy += comp1->internalEnergy;
        }
        rs.ResolveSituation(idx1);
        if (idx2 == -1)
        {
            rs.AddCompoundToRandomLocationInSolution(comp2);
        }
        return true;
    }
    return false;
}
//as instability decreases, energy increases
//if something would have negative energy as a result of this, move energy from the other one to balance it out
void AdjustEnergyValues(Compound* c1, Compound* c2, int& c1instabilityChange, int& c2instabilityChange)
{
    c1->internalEnergy -= c1instabilityChange;
    c2->internalEnergy -= c2instabilityChange;
    if (c1->internalEnergy < 0)
    {
        c2->internalEnergy += c1->internalEnergy;
        c1->internalEnergy = 0;
    }
    else if (c2->internalEnergy < 0)
    {
        c1->internalEnergy += c2->internalEnergy;
        c2->internalEnergy = 0;
    }
}

bool RandomlyReactIfGoodEnough(Compound* c1, Compound* c2, int threshold, int& c1instabilityChange, int& c2instabilityChange)
{
    Compound diff;
    CreateRandomReactantGroup(c1, &diff);
    //calculate the activation energy, make the threshold harder to overcome
    threshold -= diff.GetActivationInstability();
    //calculate the internal kinetic energy, make the threshold easier to overcome
    threshold += c1->internalEnergy;
    threshold += c2->internalEnergy;
    return PerformReactionIfGoodEnough(c1, c2, &diff, threshold, c1instabilityChange, c2instabilityChange);
}

void CreateRandomReactantGroup(Compound* c1, Compound* diff)
{
    do
    {
        for (int i = 0; i < 4; i++)
        {
            diff->elements[i] = rand() % (c1->elements[i] + 1);
        }
        diff->CalculateSum();
    } while (diff->sum == 0);
}

bool PerformReactionIfGoodEnough(Compound* c1, Compound* c2, Compound* diff, int threshold, int& c1instabilityChange, int& c2instabilityChange)
{
    int totalinstabilitychange = DoReactionAndReport(*c1, *c2, *diff, c1instabilityChange, c2instabilityChange);
    if (totalinstabilitychange > threshold)
    {
        //undo the reaction
        DoReaction(*c2, *c1, *diff);
        return false;
    }
    return true;
}

int DoReactionAndReport(Compound& c1, Compound& c2, Compound& diff, int& c1instabilityChange, int& c2instabilityChange)
{
    //get instability prior to reaction
    int c1instabilityPreReaction = c1.GetTotalInstability();
    int c2instabilityPreReaction = c2.GetTotalInstability();
    DoReaction(c1, c2, diff);
    //get instability after reaction
    int c1instabilityPostReaction = c1.GetTotalInstability();
    int c2instabilityPostReaction = c2.GetTotalInstability();

    //instability after - instability before = change in instability (negative numbers indicate an increase in overall stability)
    c1instabilityChange = c1instabilityPostReaction - c1instabilityPreReaction;
    c2instabilityChange = c2instabilityPostReaction - c2instabilityPreReaction;
    return (c1instabilityPostReaction + c2instabilityPostReaction) - (c1instabilityPreReaction + c2instabilityPreReaction);
}

void DoReaction(Compound& c1, Compound& c2, Compound& diff)
{
    for (int i = 0; i < 4; i++)
    {
        c1.elements[i] -= diff.elements[i];
        c2.elements[i] += diff.elements[i];
    }
    c1.CalculateSum();
    c2.CalculateSum();
}