#include "ReactionManager.h"
#include "Utilities.h"

#include <cstdio>
#include <iostream>
int AttemptReaction(ReactionSpace& initial, int threshold, std::vector<Compound*>& reactants)
{
    if (initial.ContainsReactant())
    {
        int idx1 = initial.GetReactantKey();
        int idx2 = initial.GetReactantKey();
        //a tenth of the time, just try to react with yourself (i.e. break apart or move things around)
        if (rand() % 10 == 0)
        {
            idx2 = idx1;
        }
        Compound* comp1 = initial.GetReactantWithKey(idx1);
        Compound* comp2 = initial.GetReactantWithKey(idx2);
        int stability = 0;
        int ielem1;
        int ielem2;

        int idxidx1 = rand() % comp1->filledIndices.size();
        int idxidx2 = rand() % comp2->filledIndices.size();
        ielem1 = comp1->filledIndices[idxidx1];
        ielem2 = comp2->filledIndices[idxidx2];
        int emptyIdxs[8];
        int spaces = comp2->getUnPopulatedNeighborsIndices(ielem2, emptyIdxs);
        if (spaces == 0)    
        {
            return false;
        }
        ielem2 = emptyIdxs[rand() % spaces];
        CallReactionAndCleanUp(comp1, comp2, stability, ielem1, idxidx1, ielem2, true, threshold, initial, reactants, false, idx1, idx2);
        return stability;
    }
}

//NOTE: deleted compounds dont show up here, must be dealt with elsewhere
void AdjustEnergyValues(int stabilityChange, std::vector<Compound*>& reactants)
{
    if (reactants.size() != 0)
    {
        int energyToTransfer = stabilityChange / (int)reactants.size();
        for (Compound* c : reactants)
        {
            c->internalEnergy += energyToTransfer;
        }
        reactants[0]->internalEnergy += stabilityChange - energyToTransfer * (int)reactants.size(); //leftover after int. division
        int asdf = energyToTransfer;
    }
}

bool PerformReaction(Compound* comp1, Compound* comp2, int& stabilityChange, int& ielem1, int& idxidx1, int& ielem2, bool useThreshold, int threshold, Compound* & pieces, int& numPieces)
{
    int x1, y1, x2, y2;
    comp1->TranslateFromIdxToXY(ielem1, x1, y1);
    comp2->TranslateFromIdxToXY(ielem2, x2, y2);

    // current instability
    int stability = 0;
    if (comp1 != comp2)
        //stability = comp1.GetTotalStability() + comp2.GetTotalStability();
        stability = comp1->GetStabilityAtPoint(x1, y1) + comp2->GetStabilityAtPoint(x2, y2);
    else
        stability = comp1->GetTotalStability();
    DoReaction(*comp1, *comp2, x1, y1, x2, y2, idxidx1);
    // subtract the instability of the new, so if the result is positive we have increased stability
    if (comp1 != comp2)
        //stability = comp1.GetTotalStability() + comp2.GetTotalStability() - stability;
        stability = (comp1->GetStabilityAtPoint(x1, y1) + comp2->GetStabilityAtPoint(x2, y2)) - stability;
    else
        stability = comp1->GetTotalStability() - stability;

    if (useThreshold && stability < threshold)
    {
        //undo the reaction
        DoReaction(*comp2, *comp1, x2, y2, x1, y1, comp2->filledIndices.size() - 1);
        ielem1 = comp1->TranslateFromXYToIdx(x1,y1);
        return false;
    }
    ielem1 = comp1->TranslateFromXYToIdx(x1, y1);
    stabilityChange = stability;
    //if comp1 has been consumed entirely
    if (comp1->mass == 0)
    {
        //one line energy calculation
        comp2->internalEnergy += stabilityChange + comp1->internalEnergy;
        //theres nothing else to do... no splitting, no identity checks, nothing
    }
    else
    {
        pieces = comp1->SplitCompound(ielem1, numPieces);
        std::vector<Compound*> reactants;
        if (comp1 == comp2)
        {
            //if this is the same compound twice
            if (numPieces == 0)
            {
                //if we didnt break, just put the single compound in ONCE
                reactants.emplace_back(comp1);
            }
            else
            {
                //just put in the pieces
                for (int i = 0; i < numPieces; ++i)
                {
                    reactants.emplace_back(&(pieces[i]));
                }
            }

        }
        else
        {
            //there were two compounds
            if (numPieces == 0)
            {
                //if there was no splitting, put in comp1
                reactants.emplace_back(comp1);
            }
            else
            {
                //if there was splitting, just put in the pieces of comp1
                for (int i = 0; i < numPieces; ++i)
                {
                    reactants.emplace_back(&(pieces[i]));
                }
            }
            //either way, put in comp2
            reactants.emplace_back(comp2);
        }
        //distribute energy evenly
        if (numPieces != 0)
        {
            stabilityChange += comp1->internalEnergy;
        }
        AdjustEnergyValues(stabilityChange, reactants);
        int negativeEnergy = 0;
        for (Compound* c : reactants)
        {
            if (c->internalEnergy < 0)
            {
                negativeEnergy += c->internalEnergy;
                c->internalEnergy = 0;
            }
        }
        //always dump negative energy into comp2
        comp2->internalEnergy += negativeEnergy;
    }
    return true;
}

void CallReactionAndCleanUp(Compound* comp1, Compound* comp2, int& stabilityChange, int& ielem1, int& idxidx1, int& ielem2, bool useThreshold, int threshold, ReactionSpace& rs, std::vector<Compound*>& reactants, bool CompRemovedFromIsOrganelleStructure, int idxOfComp1InSolution, int idxOfComp2InSolution)
{
    Compound* pieces = nullptr;
    int numPieces = 0;

    //std::cout << "before" << "\n";
    //comp1->PrintPhysicalStructure();
    //comp2->PrintPhysicalStructure();
    bool didreaction = PerformReaction(comp1, comp2, stabilityChange, ielem1, idxidx1, ielem2, useThreshold, threshold, pieces, numPieces);

    //this is the section where we adjust the identity of organelle structures, move things in and out of solution, etc
    if (!didreaction)
    {
        //we did nothing... lets just
        return;
    }
    if (numPieces == 0 && comp1->mass != 0)
    {
        //nothing broke up or disappeared, soo lets
        return;
    }
    if (CompRemovedFromIsOrganelleStructure && comp1->mass == 0)
    {
        //nothing in solution will change as a result of this... we will just need to kill the organelle when we
        return;
    }
    //now that we've gotten the do-nothing cases out of the way....
    if (CompRemovedFromIsOrganelleStructure && numPieces != 0)
    {
        //the organelle's structure broke! oh no!
        //lets make sure that whatever becomes the new structure is the largest leftover piece
        //comp1 is the structure of the organelle, so....
        int largest = -1;
        int size = -1;
        for (int i = 0; i < numPieces; i++)
        {
            if (pieces[i].mass > size)
            {
                largest = i;
                size = pieces[i].mass;
            }
        }
        //set the structure to that piece
        *comp1 = pieces[largest];
        //now, get rid of that piece! we dont want to put it in solution. we'll just take one off the end of the list
        //(this also works if the largest piece IS the one at the end of the list)
        pieces[largest] = pieces[numPieces - 1];
        numPieces -= 1;
    }
    //now we need to add/remove things to/from solution.    
    if (CompRemovedFromIsOrganelleStructure)
    {
        //if the thing we removed from was an organelle, we wont delete anything from solution, so we give a negative key
        rs.ResolveSituation(pieces, numPieces, -1);        
    }
    else
    {
        //we may delete a compound!
        rs.ResolveSituation(pieces, numPieces, idxOfComp1InSolution);
    }
    delete[] pieces;
    return;
}

bool PerformReactionIfGoodEnough(Compound* comp1, Compound* comp2, int threshold, int& stabilityChange, int& ielem1, int& ielem2)
{
    int idxidx1 = rand() % comp1->filledIndices.size();
    int idxidx2 = rand() % comp2->filledIndices.size();
    ielem1 = comp1->filledIndices[idxidx1];
    ielem2 = comp2->filledIndices[idxidx2];
    int emptyIdxs[8];
    int spaces = comp2->getUnPopulatedNeighborsIndices(ielem2, emptyIdxs);
    if (spaces == 0)
    {
        return false;
    }
    ielem2 = emptyIdxs[rand() % spaces];
    int x1, y1, x2, y2;
    comp1->TranslateFromIdxToXY(ielem1, x1, y1);
    comp2->TranslateFromIdxToXY(ielem2, x2, y2);

    // current instability
    int stability = 0;
    if (comp1 != comp2)
        //stability = comp1.GetTotalStability() + comp2.GetTotalStability();
        stability = comp1->GetStabilityAtPoint(x1, y1) + comp2->GetStabilityAtPoint(x2, y2);
    else
        stability = comp1->GetTotalStability();
    DoReaction(*comp1, *comp2, x1, y1, x2, y2, idxidx1);
    // subtract the instability of the new, so if the result is positive we have increased stability
    if (comp1 != comp2)
        //stability = comp1.GetTotalStability() + comp2.GetTotalStability() - stability;
        stability = (comp1->GetStabilityAtPoint(x1, y1) + comp2->GetStabilityAtPoint(x2, y2)) - stability;
    else
        stability = comp1->GetTotalStability() - stability;

    if (stability < threshold)
    {
        //undo the reaction
        DoReaction(*comp2, *comp1, x2, y2, x1, y1, comp2->filledIndices.size() - 1);
        ielem1 = comp1->TranslateFromXYToIdx(x1, y1);
        return false;
    }
    ielem1 = comp1->TranslateFromXYToIdx(x1, y1);
    stabilityChange = stability;
    return true;
}

//may eventually be changed to actually traverse the structure of a molecule and find a location on the outside.
//however, the number of cycles that this would require to complete may simply make this untenable
//fast and bad may be better than good but really slow? the goal is complex behaviour, if going faster lets us
//achieve more progress, im all about it
int FindElementIdxByFalling(Compound& c)
{
    int ret;
    while (c.composition[c.filledIndices[(ret = rand() % c.filledIndices.size())]].red == 0)
    {
        FastDelete(c.filledIndices, ret);
    }
    ret = c.filledIndices[ret];
    return ret;
}

void DoReaction(Compound& c1, Compound& c2, int x1, int y1, int x2, int y2, int idxidxtoremove)
{    
    Element e =c1.RemoveElementAtIndex(x1, y1);
    FastDelete(c1.filledIndices, idxidxtoremove);
    c2.AddElementAtIndex(e, x2, y2);
}