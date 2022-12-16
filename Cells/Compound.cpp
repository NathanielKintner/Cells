#include "Compound.h"
#include "HexGrids.h"

int Compound::GetTotalInstability()
{
	return sum % 2 + sum % 3 + sum % 5 + sum % 7 + elementCount - (5 * (elementCount != 0)) + (2 * elementCount * (elementCount > 10));
}

int Compound::GetActivationInstability()
{
	return sum % 2 + sum % 3 + sum % 5 + sum % 7;
}

void Compound::CalculateSum()
{
	sum = elements[0] * 2 + elements[1] * 3 + elements[2] * 5 + elements[3] * 7;
	elementCount = elements[0] + elements[1] + elements[2] + elements[3];
}

void Compound::CalculateProduct()
{
	product = 1;
	for (int i = 0; i < elements[0]; i++)
	{
		product *= 2;
	}
	for (int i = 0; i < elements[1]; i++)
	{
		product *= 3;
	}
	for (int i = 0; i < elements[2]; i++)
	{
		product *= 5;
	}
	for (int i = 0; i < elements[3]; i++)
	{
		product *= 7;
	}
}






//negative x is to the left, negative y is to the top
//       xaxis
//        2  3  4  5
//y
//a -2    0  1  2  3
//x -1    4  5  6  7
//i  0    8  9  10 11
//s  1    12 13 14 15
//
//allows interpretation of a simple vector as a square vector, with arbitrarily defined axes
//EVERYTHING AFTER THIS POINT WAS RECENTLY COMMENTED OUT
//int GetIndexOfConceptuallyShiftedSquareVector(int xpos, int ypos, int minX, int minY, int maxX)
//{//        distance from left     distance from top     length of each row
//    return (xpos - minX)       + ((ypos - minY)    *    (1 + maxX - minX));
//}
//
////xpos and ypos are return values, pass in whatever you like
//void GetXAndYOfConceptuallyShiftedSquareVector(int idx, int& xpos, int& ypos, int minX, int minY, int maxX)
//{
//    int rowlen = maxX - minX + 1;
//    xpos = (idx % rowlen) + minX;
//    ypos = (idx / rowlen) + minY;
//}
//
//void Compound::TranslateFromIdxToXY(int idx, int& x, int& y)
//{
//    GetXAndYOfConceptuallyShiftedSquareVector(idx, x, y, minX, minY, maxX);
//}
//int Compound::TranslateFromXYToIdx(int x, int y)
//{
//    return GetIndexOfConceptuallyShiftedSquareVector(x, y, minX, minY, maxX);
//}
//
////redefines the section layout of composition, adding an additional layer to the top, bottom, or one of the sides
//void Compound::ReformComposition(int newMinX, int newMinY, int newMaxX, int newMaxY)
//{
//    std::vector<Element> newComp((1 + newMaxX - newMinX) * (1 + newMaxY - newMinY));
//    std::vector<int> newFilledIndices(0);
//    for (int idx: filledIndices)
//    {
//        if (composition[idx].red != 0)
//        {
//            int xpos = 0;
//            int ypos = 0;
//            GetXAndYOfConceptuallyShiftedSquareVector(idx, xpos, ypos, minX, minY, maxX);
//            int newIdx = GetIndexOfConceptuallyShiftedSquareVector(xpos, ypos, newMinX, newMinY, newMaxX);
//            newComp[newIdx] = composition[idx];
//            newFilledIndices.emplace_back(newIdx);
//        }
//    }
//    filledIndices = newFilledIndices;
//    composition = newComp;
//    minX = newMinX;
//    maxX = newMaxX;
//    minY = newMinY;
//    maxY = newMaxY;
//    lastNeighborhoodQueried = -1;
//}
//
//bool Compound::CheckRep()
//{
//    if (filledIndices.size() == 1)
//    {
//        return true;
//    }
//    if (filledIndices.size() == 0)
//    {
//        return false;
//    }
//    for (int e : filledIndices)
//    {
//        getPopulatedNeighbors(e);
//        if (neighborCache[0] < 0)
//        {
//            return false;
//        }
//    }
//    return true;
//}
//
//void Compound::AddElementAtIndex(Element e, int idx)
//{
//    int x = 0;
//    int y = 0;
//    GetXAndYOfConceptuallyShiftedSquareVector(idx, x, y, minX, minY, maxX);
//    AddElementAtIndex(e, x, y);
//}
//
//void Compound::AddElementAtIndex(Element e, int x, int y)
//{	
//    mass += e.red;
//    e.currentGreen = 0;
//    e.currentBlue = e.blue;
//    bool doReform = false;
//    int newMaxX = maxX;
//    int newMaxY = maxY;
//    int newMinX = minX;
//    int newMinY = minY;
//    if (maxX < x + 1)
//    {
//        doReform = true;
//        newMaxX = x + 1;
//    }
//    if (maxY < y + 1)
//    {
//        doReform = true;
//        newMaxY = y + 1;
//    }
//    if (minX > x - 1)
//    {
//        doReform = true;
//        newMinX = x - 1;
//    }
//    if (minY > y - 1)
//    {
//        doReform = true;
//        newMinY = y - 1;
//    }
//    if (doReform)
//    {
//        ReformComposition(newMinX, newMinY, newMaxX, newMaxY);
//    }
//    int idx = GetIndexOfConceptuallyShiftedSquareVector(x, y, minX, minY, maxX);
//    for (int asdf : filledIndices)
//    {
//        if (idx == asdf)
//        {
//            int asdf2 = 1;
//        }
//    }
//    filledIndices.emplace_back(idx);
//    composition[idx] = e;
//    getPopulatedNeighbors(idx);
//    int i = 0;
//    while (neighborCache[i] > -1)
//    {
//        Element* eNeighbor = &composition[neighborCache[i]];
//        composition[idx].currentGreen++;
//        composition[idx].currentBlue += eNeighbor->blue;
//        eNeighbor->currentGreen++;
//        eNeighbor->currentBlue += e.blue;
//        i++;
//    }
//}
//
//Element Compound::RemoveElementAtIndex(int x, int y)
//{
//    return RemoveElementAtIndex(GetIndexOfConceptuallyShiftedSquareVector(x, y, minX, minY, maxX));
//}
//
//Element Compound::RemoveElementAtIndex(int idx)
//{    
//    Element e = composition[idx];
//    mass -= e.red;
//    //filledIndices.erase(idx); FILLED INDICES HAS LAZY DELETION NOW, BEWARE
//    getPopulatedNeighbors(idx);
//    composition[idx].red = 0; //this will be treated as unpopulated
//
//    int i = 0;
//    while (neighborCache[i] > -1)
//    {
//        Element* eNeighbor = &composition[neighborCache[i]];
//        eNeighbor->currentGreen--;
//        eNeighbor->currentBlue -= e.blue;
//        i++;
//    }
//    return e;
//}
//
//void Compound::GetPopulatedNeighbors(int idx, std::vector<int>& out)
//{
//    getPopulatedNeighbors(idx);
//    int i = 0;
//    while (neighborCache[i] > -1)
//    {
//        out.emplace_back(neighborCache[i]);
//        i++;
//    }
//}
//
//void Compound::getPopulatedNeighbors(int idx)
//{
//    lastNeighborhoodQueried = idx;
//    int filledIdx = 0;
//    int xpos = 0;
//    int ypos = 0;
//    int width = 1 + maxX - minX;
//    int height = 1 + maxY - minY;
//    GetXAndYOfConceptuallyShiftedSquareVector(idx, xpos, ypos, minX, minY, maxX);
//    
//    //where we start in the circle. Starting on a diagonal will always be wrong if we are near an edge
//    //int startpos = 0;
//    // 0
//    //3 1
//    // 2
//    
//    //if the top is missing, set to 1, otherwise 0
//    int notAtTop = minY != ypos;
//    int notAtBot = maxY != ypos;
//    int notAtLeft = minX != xpos;
//    int notAtRight = maxX != xpos;
//    //int totalwalls = atTop + atBot + atLeft + atRight;
//    /*//branchless set startpos to the right place if there is one wall
//    startpos += (totalwalls == 1) * atTop;
//    startpos += (totalwalls == 1) * atRight * 2;
//    startpos += (totalwalls == 1) * atBot * 3;
//    //branchless set startpos to the right place if there are two walls
//    startpos += (totalwalls == 2) * atLeft * atTop;
//    startpos += (totalwalls == 2) * atTop * atRight * 2;
//    startpos += (totalwalls == 2) * atRight * atBot * 3;*/
//
//    int idxnum = 0;
//    neighborCache[idxnum] = idx - 1;
//    idxnum += notAtLeft * composition[(idx - 1) * notAtLeft].red != 0;
//    neighborCache[idxnum] = idx + 1;
//    idxnum += notAtRight * composition[(idx + 1) * notAtRight].red != 0;
//    neighborCache[idxnum] = idx + width - 1;
//    idxnum += notAtLeft * notAtBot * composition[(idx + width - 1) * notAtLeft * notAtBot].red != 0;
//    neighborCache[idxnum] = idx + width;
//    idxnum += notAtBot * composition[(idx + width) * notAtBot].red != 0;
//    neighborCache[idxnum] = idx + width + 1;
//    idxnum += notAtRight * notAtBot * composition[(idx + width + 1) * notAtRight * notAtBot].red != 0;
//    neighborCache[idxnum] = idx - width - 1;
//    idxnum += notAtLeft * notAtTop * composition[(idx - width - 1) * notAtLeft * notAtTop].red != 0;
//    neighborCache[idxnum] = idx - width;
//    idxnum += notAtTop * composition[(idx - width) * notAtTop].red != 0;
//    neighborCache[idxnum] = idx - width + 1;
//    idxnum += notAtRight * notAtTop * composition[(idx - width + 1) * notAtRight * notAtTop].red != 0;
//    neighborCache[idxnum] = -1;
//}
//
//int Compound::getUnPopulatedNeighborsIndices(int x, int y, int* ret)
//{
//    return getUnPopulatedNeighborsIndices(GetIndexOfConceptuallyShiftedSquareVector(x, y, minX, minY, maxX), ret);
//}
//
//int Compound::getUnPopulatedNeighborsIndices(int idx, int* ret)
//{
//    getPopulatedNeighbors(idx);
//    int xpos = 0;
//    int ypos = 0;
//    int width = 1 + maxX - minX;
//    int height = 1 + maxY - minY;
//    GetXAndYOfConceptuallyShiftedSquareVector(idx, xpos, ypos, minX, minY, maxX);
//    int allSurroundingIndices[8] = { idx - 1, idx + 1, idx + width - 1, idx + width, idx + width + 1, idx - width - 1, idx - width, idx - width + 1 };
//    /*std::pair<int, int> allSurroundingCoords[8] = { std::pair<int,int>(xpos - 1, ypos),
//                                                    std::pair<int,int>(xpos + 1, ypos), 
//                                                    std::pair<int,int>(xpos - 1, ypos + 1), 
//                                                    std::pair<int,int>(xpos, ypos + 1), 
//                                                    std::pair<int,int>(xpos + 1, ypos + 1), 
//                                                    std::pair<int,int>(xpos - 1, ypos - 1), 
//                                                    std::pair<int,int>(xpos, ypos - 1), 
//                                                    std::pair<int,int>(xpos + 1, ypos - 1)};*/
//    int neighborIdx = 0;
//    int retIdx = 0;
//    for (int i = 0; i < 8; i++)
//    {
//        if (neighborCache[neighborIdx] != allSurroundingIndices[i])
//        {
//            ret[retIdx] = allSurroundingIndices[i];
//            retIdx++;
//        }
//        else
//        {
//            neighborIdx++;
//        }
//    }
//    return retIdx;
//}
//
//int Compound::GetStabilityAtPoint(int x, int y)
//{
//    return GetStabilityAtPoint(GetIndexOfConceptuallyShiftedSquareVector(x, y, minX, minY, maxX));
//}
//
//int Compound::GetStabilityAtPoint(int idx)
//{    
//    Element e = composition[idx];
//    getPopulatedNeighbors(idx);
//    int stability = 0;
//    if (e.red != 0)
//    {
//        stability -= e.GetInstability();
//    }
//    int i = 0;
//    while (neighborCache[i] > -1)
//    {
//        stability -= composition[neighborCache[i]].GetInstability();
//        i++;
//    }
//    return stability;
//}
//
//int Compound::GetTotalStability()
//{
//    int stability = 0;
//    int target = filledIndices.size();
//    for (int i = 0; i < target; i++)
//    {
//        stability -= composition[filledIndices[i]].GetInstability();
//    }
//    return stability;
//}
//
//Element Compound::GetColorCharges()
//{
//    Element ret = Element();
//    for (int idx : filledIndices)
//    {
//        const Element& e = composition[idx];
//        ret.red += e.red;
//        ret.blue += e.blue;
//        ret.green += e.green;
//    }
//    return ret;
//}
//
//void Compound::ReCenter()
//{
//    int minFoundX = 0;
//    int minFoundY = 0;
//    TranslateFromIdxToXY(filledIndices[0], minFoundX, minFoundY);
//    int maxFoundX = minFoundX;
//    int maxFoundY = minFoundY;
//    for (int idx : filledIndices)
//    {
//        int x, y;
//        TranslateFromIdxToXY(idx, x, y);
//        if (maxFoundX < x + 1)
//        {
//            maxFoundX = x + 1;
//        }
//        if (maxFoundY < y + 1)
//        {
//            maxFoundY = y + 1;
//        }
//        if (minFoundX > x - 1)
//        {
//            minFoundX = x - 1;
//        }
//        if (minFoundY > y - 1)
//        {
//            minFoundY = y - 1;
//        }        
//    }
//    ReformComposition(minFoundX, minFoundY, maxFoundX, maxFoundY);
//    int xadj = (minFoundX + maxFoundX) / 2;
//    int yadj = (minFoundY + maxFoundY) / 2;
//    minX -= xadj;
//    maxX -= xadj;
//    minY -= yadj;
//    maxY -= yadj;
//}
//
sf::Color Compound::GetColor()
{
    int r = 0;
    int g = 0;
    int b = 0;
    /*for (int i : filledIndices)
    {
        Element e = composition[i];
        r += e.red;
        g += e.green;
        b += e.blue;
    }*/

	/*r = (r * 10) % 255;
	g = (g * 10) % 255;
	b = (b * 10) % 255;*/

	r = elements[0];
	g = elements[1];
	b = elements[2];

    r = (r * 50) % 255;
    g = (g * 50) % 255;
    b = (b * 50) % 255;
    
    sf::Color ret = sf::Color::Black;
    ret.r += r;
    ret.b += b;
    ret.g += g;
    return ret;
}
//
//
//Compound* Compound::SplitCompound(int x, int y, int& numPieces)
//{
//    return SplitCompound(GetIndexOfConceptuallyShiftedSquareVector(x, y, minX, minY, maxX), numPieces);
//}
//
//Compound* Compound::SplitCompound(int removeIdx, int& numPieces)
//{
//    //get an array representation of all the indices around the index that we removed an element from.
//    //basically, if we removed at X, this will find everything that is at the Os at put it in neighborCache
//    // ............
//    //.....OOO....
//    // ....OXO......
//    //.....OOO......
//    //.............
//    getPopulatedNeighbors(removeIdx);
//    //now, check to see if we have some specific possiblities
//    if (neighborCache[1] < 0 || //if there is only 1 neighbor
//        (neighborCache[2] > -1 && neighborCache[3] > -1 && neighborCache[4] > -1 && neighborCache[5] > -1 && neighborCache[6] > -1)) //or if there are 7 or 8 neighbors
//    {
//        //it is impossible for a split to have occurred, so just
//        return nullptr;
//    }
//    //however, with 2, 3, 4, 5, or 6 neighbors, it is possible that removing an element at this index would cause the compound to split
//    //this is a very sepcial array. Each index represents a group of elements that could form a unique compound.
//    int flags[6]{ -1,-1,-1,-1,-1,-1 };
//    //target is just the total number of indices in the compound's virtual space
//    int target = composition.size();
//    //we create a representation of the space using integers
//    int* GroupMap = new int[target];
//    //mark all areas with -2, meaning "empty"
//    for (int i = 0; i < target; i++)
//    {
//        GroupMap[i] = -2;
//    }
//    //overwrite specific locations with -1, meaning "not empty but not grouped yet"
//    for (int i : filledIndices)
//    {
//        GroupMap[i] = -1;
//    }
//    //a list to keep track of the open search border (similar to dijkstra's algorithm)
//    //this is keeping track of a list of indices (locations in the compound's space)
//    std::list<int> borders;
//    //go through neighborCache, create initial groups
//    for (int i = 0; neighborCache[i] > -1; ++i)
//    {
//        //add each index in the cache to the border
//        borders.push_back(neighborCache[i]);
//        //set a flag, meaning that we are creating a new group
//        flags[i] = i;
//        //on the int representation, mark each newly-grouped index with it's group number
//        GroupMap[neighborCache[i]] = i;
//    }
//    //while we have spaces to investigate
//    while (borders.size() != 0)
//    {
//        int curr = borders.front();//current location we are searching from
//        borders.pop_front();
//        int groupnum = flags[GroupMap[curr]];//group of current location (it WILL have one)
//        //find all the neighbors of this location. Some of these may be grouped already.
//        getPopulatedNeighbors(curr);
//        //starting at the first index in the cache, going through until the end of the cache (all filled adjacent locations)
//        int neighboridx = 0;
//        while (neighborCache[neighboridx] > -1)
//        {
//            //the location of this neighbor
//            int spaceidx = neighborCache[neighboridx];
//            //the value of this index on the int representation of the space, -1 for empty and otherwise already grouped
//            int mapval = GroupMap[spaceidx];
//            switch (mapval)
//            {
//            //note that case(-2) would indicate a non-filled position, which won't be returned by getPopulatedNeighbors anyway
//            case(-1):
//                //ungrouped index. add it to the open border, and set its value to the group of the index we are searching from
//                borders.push_back(spaceidx);
//                GroupMap[spaceidx] = groupnum;
//                break;
//            default:
//                //this has been grouped already, meaning that something has already searched here
//                //if this value has a different group than we do
//                int otherGroup = flags[mapval];
//                if (otherGroup != groupnum)
//                {
//                    //find everything that has the other group, change it to have our group instead
//                    for (int flagIdx = 0; flagIdx < 6; ++flagIdx)
//                    {
//                        if (flags[flagIdx] == otherGroup)
//                        {
//                            flags[flagIdx] = groupnum;
//                        }
//                    }
//                }
//                
//                break;
//            }
//            neighboridx++;
//        }
//    }
//    //borders now changes meaning, it is instead a list of the pieces of the compound
//    for (int i = 0; i < 6; i++)
//    {
//        if (flags[i] == i)
//        {
//            borders.push_back(i);
//        }
//    }
//    if (borders.size() == 1)
//    {
//        delete[] GroupMap;
//        return nullptr;
//    }
//    //create an array to act as a mapping between group numbers and the indices of the return array
//    //i.e. each unique flagnumber will correspond to a output unique compound
//    int flagDests[6]{ -1, -1, -1, -1, -1, -1 };
//    numPieces = borders.size();
//    Compound* ret = new Compound[numPieces];
//    //for each group
//    for (int i = 0; i < numPieces; i++)
//    {
//        //the groupnumber that dominates this piece
//        int group = borders.front();
//        borders.pop_front();
//        //denote that this group will be at place i in the return array
//        flagDests[group] = i;
//    }
//    for (int i : filledIndices)
//    {
//        //in the group map at this element, you find a group number.
//        //go to flags to see what groupnumber is dominating this groupnumber, if any
//        //go to flagdests to see where the dominant number is being returned in ret
//        //add the element there
//        int x, y;
//        GetXAndYOfConceptuallyShiftedSquareVector(i, x, y, minX, minY, maxX);
//        ret[flagDests[flags[GroupMap[i]]]].AddElementAtIndex(composition[i], x, y);
//    }
//    delete[] GroupMap;
//    return ret;
//}
//
//std::string Compound::ChemicalString()
//{
//    char buf[5000];//in practice, this is sufficient
//    int width = maxX - minX + 1;
//    buf[0] = (char)minX + 128;
//    buf[1] = (char)minY + 128;
//    int cursor = 2;
//    unsigned char numzeros = 0;
//    int target = composition.size() - (width + 1); //force optimization, and ignore last row
//    //start at the second row, second idx
//    for (int i = width+1; i < target; i++)
//    {
//        //moving to next line (skip last index, which will be open)
//        if ((i + 1) % width == 0)
//        {
//            //forget about zeroes
//            numzeros = 0;
//            //carrige return symbol
//            buf[cursor] = '>';
//            cursor++;
//            i += 2;//skip the last idx of this row and the first idx of the next
//        }
//        if (composition[i].red == 0) //empty
//        {
//            //increase the current count of 0s
//            numzeros++;
//        }
//        else
//        {
//            //dump all current 0s
//            if (numzeros != 0)
//            {
//                if (numzeros == 1)
//                {
//                    buf[cursor] = '.';
//                    cursor++;
//                }
//                else
//                {
//                    buf[cursor] = '!';
//                    cursor++;
//                    buf[cursor] = numzeros;
//                    cursor++;
//                }
//                numzeros = 0;
//            }
//            buf[cursor] = composition[i].name;
//            cursor++;
//        }
//    }
//    std::string ret = std::string(buf, cursor);
//    Compound test(ret);
//    if (test.mass != mass)
//    {
//        int asdf = 1;
//    }
//    return ret;
//}
//
//void Compound::PrintPhysicalStructure()
//{
//    std::cout << '\n';
//    int idx = 0;
//    for (int i = minY; i <= maxY; i++)
//    {
//        for (int j = minX; j <= maxX; j++)
//        {
//            if (composition[idx].red < 1)
//            {
//                std::cout << '.';
//            }
//            else
//            {
//                std::cout << composition[idx].name;
//            }
//            idx++;
//        }
//        std::cout << '\n';
//    }
//    std::cout << '\n';
//}