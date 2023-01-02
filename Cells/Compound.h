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
    char elements[4] = { 0 };
    unsigned long product = 0;
    int sum = 0;
    int elementCount = 0;
    Compound()
    {
        Vvert = 0;
        Vhoriz = 0;
        mass = 0;
        internalEnergy = 0;
    };
    Compound(unsigned char code)
    {
        Vvert = 0;
        Vhoriz = 0;
        mass = 0;
        internalEnergy = 0;
        //initialize composition to random values between 0 and 3 (inclusive) for each element
        elements[0] = code % 4;//first and second bits
        elements[1] = (code >> 2) % 4;//third and fourth bits
        elements[2] = (code >> 4) % 4;//fifth and sixth bits
        elements[3] = code >> 6;//seventh and eigth bits
    };
    //generally used to make 'masks' for calculating fit scores
    Compound(unsigned char code1, unsigned char code2)
    {
        Vvert = 0;
        Vhoriz = 0;
        mass = 0;
        internalEnergy = 0;
        //initialize composition to random values between 0 and 7 (inclusive) for each element, then subtract 3
        //this gives us a range from -3 to +4, including 0.
        //each value specifies a range of possible values that will not increase the return the value of CalculateFitScore:
        //-3: no less than 3, no more than 7
        //-2: no less than 2, no more than 6
        //-1: no less than 1, no more than 5
        // 0: no more than 4
        // 1: no more than 3
        // 2: no more than 2
        // 3: no more than 1
        // 4: no more than 0 (cannot include this element)
        elements[0] = (code1 % 8) - 3;//first -> 4th bits
        elements[1] = ((code1 >> 4) % 8) - 3;//third and fourth bits
        elements[2] = (code2 % 8) - 3;//fifth and sixth bits
        elements[3] = ((code2 >> 4) % 8) - 3;//seventh and eigth bits
    };
    void CalculateSum();
    void CalculateProduct();
    void FindDifference(Compound* tocomparewith, Compound* ret);
    char CalculateFitScore(Compound* c);

    int id;
    //Compound()
    //{
    //    composition = std::vector<Element>(1);
    //    filledIndices = std::vector<int>(0);
    //    minX = 0;
    //    minY = 0;
    //    maxX = 0;
    //    maxY = 0;
    //    neighborCache[8] = -1;
    //    Vvert = 0;
    //    Vhoriz = 0;
    //    mass = 0;
    //    internalEnergy = 0;
    //    lastNeighborhoodQueried = -1;
    //};
    //Compound(std::string elems)
    //{
    //    composition = std::vector<Element>(1);
    //    filledIndices = std::vector<int>(0);
    //    minX = 0;
    //    minY = 0;
    //    maxX = 0;
    //    maxY = 0;
    //    neighborCache[8] = -1;
    //    Vvert = 0;
    //    Vhoriz = 0;
    //    mass = 0;
    //    internalEnergy = 0;
    //    lastNeighborhoodQueried = -1;
    //    int xstart = (unsigned char)elems[0] - 128 + 1;
    //    int ystart = (unsigned char)elems[1] - 128 + 1;
    //    int xpos = xstart;
    //    int ypos = ystart;
    //    for (size_t i = 2; i < elems.size(); i++)
    //    {
    //        switch (elems[i])
    //        {
    //        case '.':
    //            xpos++;
    //            break;
    //        case '!':
    //            i++;
    //            xpos += elems[i];
    //            break;
    //        case '>':
    //            xpos = xstart;
    //            ypos++;
    //            break;
    //        default:
    //            AddElementAtIndex(PeriodicTable::ptableHash[elems[i]], xpos, ypos);
    //            xpos++;
    //            break;
    //        }
    //    }
    //};
    
    //std::vector<Element> composition;

    /*int minX;
    int minY;
    int maxX;
    int maxY;

    std::vector<int> filledIndices;
    int neighborCache[9];
    int lastNeighborhoodQueried;*/

    int mass;

    int Vvert;//not important for chemistry, just macro-movement
    int Vhoriz;

    int internalEnergy = 0;

    Compound* StackedCompound = nullptr;

    /*void TranslateFromIdxToXY(int idx, int& x, int& y);
    int TranslateFromXYToIdx(int x, int y);
    bool CheckRep();
    void AddElementAtIndex(Element e, int x, int y);
    void AddElementAtIndex(Element e, int idx);
    Element RemoveElementAtIndex(int x, int y);
    Element RemoveElementAtIndex(int idx);*/
    //ret should be an array of length 6 at least, returns number of array indeces filled
    /*int getUnPopulatedNeighborsIndices(int x, int y, int* ret);
    int getUnPopulatedNeighborsIndices(int idx, int* ret);
    int GetStabilityAtPoint(int idx);
    int GetStabilityAtPoint(int x, int y);*/
    int GetTotalInstability();
    int GetActivationInstability();
    /*Element GetColorCharges();
    void GetPopulatedNeighbors(int idx, std::vector<int>& out);
    void ReCenter();
    void PrintPhysicalStructure();*/

    sf::Color GetColor();

    /*Compound* SplitCompound(int x, int y, int& numPieces);
    Compound* SplitCompound(int removeIdx, int& numPieces);
    std::string ChemicalString();*/

//private:
//    void getPopulatedNeighbors(int idx);
//    void ReformComposition(int newMinSectionX, int newMinSectionY, int newMaxSectionX, int newMaxSectionY);
};

