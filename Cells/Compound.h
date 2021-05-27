#pragma once
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Element.h"
#include "Utilities.h"

class Compound
{
public:
    int id;
    Compound()
    {
        composition = std::vector<Element>(1);
        filledIndices = std::vector<int>(0);
        minX = 0;
        minY = 0;
        maxX = 0;
        maxY = 0;
        neighborCache[8] = -1;
        Vvert = 0;
        Vhoriz = 0;
        mass = 0;
        internalEnergy = 0;
        lastNeighborhoodQueried = -1;
    };
    Compound(std::string elems)
    {
        composition = std::vector<Element>(1);
        filledIndices = std::vector<int>(0);
        minX = 0;
        minY = 0;
        maxX = 0;
        maxY = 0;
        neighborCache[8] = -1;
        Vvert = 0;
        Vhoriz = 0;
        mass = 0;
        internalEnergy = 0;
        lastNeighborhoodQueried = -1;
        int xstart = (unsigned char)elems[0] - 128 + 1;
        int ystart = (unsigned char)elems[1] - 128 + 1;
        int xpos = xstart;
        int ypos = ystart;
        for (size_t i = 2; i < elems.size(); i++)
        {
            switch (elems[i])
            {
            case '.':
                xpos++;
                break;
            case '!':
                i++;
                xpos += elems[i];
                break;
            case '>':
                xpos = xstart;
                ypos++;
                break;
            default:
                AddElementAtIndex(PeriodicTable::ptableHash[elems[i]], xpos, ypos);
                xpos++;
                break;
            }
        }
    };
    
    std::vector<Element> composition;

    int minX;
    int minY;
    int maxX;
    int maxY;

    std::vector<int> filledIndices;
    int mass;
    int neighborCache[9];
    int lastNeighborhoodQueried;

    int Vvert;//not important for chemistry, just macro-movement
    int Vhoriz;

    int internalEnergy = 0;

    Compound* StackedCompound = nullptr;

    void TranslateFromIdxToXY(int idx, int& x, int& y);
    int TranslateFromXYToIdx(int x, int y);
    bool CheckRep();
    void AddElementAtIndex(Element e, int x, int y);
    void AddElementAtIndex(Element e, int idx);
    Element RemoveElementAtIndex(int x, int y);
    Element RemoveElementAtIndex(int idx);
    //ret should be an array of length 6 at least, returns number of array indeces filled
    int getUnPopulatedNeighborsIndices(int x, int y, int* ret);
    int getUnPopulatedNeighborsIndices(int idx, int* ret);
    int GetStabilityAtPoint(int idx);
    int GetStabilityAtPoint(int x, int y);
    int GetTotalStability();
    Element GetColorCharges();
    void GetPopulatedNeighbors(int idx, std::vector<int>& out);
    void ReCenter();
    void PrintPhysicalStructure();

    sf::Color GetColor();

    Compound* SplitCompound(int x, int y, int& numPieces);
    Compound* SplitCompound(int removeIdx, int& numPieces);
    std::string ChemicalString();

private:
    void getPopulatedNeighbors(int idx);
    void ReformComposition(int newMinSectionX, int newMinSectionY, int newMaxSectionX, int newMaxSectionY);
};

