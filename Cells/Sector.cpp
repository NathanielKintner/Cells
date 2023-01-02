#include "Sector.h"
#include "Compound.h"



void Sector::AddCompoundToLocationInSolution(Compound * c, int location)
{
	//if space is empty here
	if (solution[location] == nullptr)
	{
		//point this space at the compound
		solution[location] = c;
		filledIdxs.emplace_back(location);
	}
	else
	{
		//the space is not empty. Point c's "stacked pointer" at whatever is stacked on the compound in the space,
		//and then point the compound in this space at c, thereby putting c second from the bottom
		c->StackedCompound = solution[location]->StackedCompound;
		solution[location]->StackedCompound = c;
	}
}



void Sector::AddCompoundToRandomLocationInSolution(Compound* c)
{
	AddCompoundToLocationInSolution(c, rand() % 25);
}

bool Sector::ContainsReactant()
{
	return filledIdxs.size() != 0;
}

int Sector::GetReactantKey()
{
	return rand() % (filledIdxs.size() + localPopulation.vec.size());
	//return rand() % (filledIdxs.size());
}

Compound* Sector::GetReactantWithKey(int key)
{
	size_t innersolutionsize = filledIdxs.size();
	if (key >= innersolutionsize)
	{
		return &((localPopulation.vec[key - innersolutionsize])->structure);
	}
	else
	{
		return solution[filledIdxs[key]];
	}
}

void Sector::ResolveSituation(int key)
{
	size_t innersolutionsize = filledIdxs.size();
	if (key >= innersolutionsize)
	{
		if ((localPopulation.vec[key - innersolutionsize])->structure.elementCount == 0)
		{
			(localPopulation.vec[key - innersolutionsize])->DoDeath();
		//FastDelete(localPopulation, key - innersolutionsize);
		}
	}
	else
	{
		int solutionLocation = filledIdxs[key];
		if (solution[solutionLocation]->elementCount == 0)
		{
			Compound* removedComp = RemoveCompoundByIdxInList(key);
			delete removedComp;
		}
	}
	
}

//WARNING: this idx is an index in filledIdxs
//This simply deletes from the filledIdxs list at this index, and nulls the solution at this location
//ALL COMPOUNDS MUST BE DISPOSED SEPERATELY
Compound* Sector::RemoveCompoundByIdxInList(int idx)
{
	int solutionLocation = filledIdxs[idx];
	Compound* ret = solution[solutionLocation];	
	solution[solutionLocation] = ret->StackedCompound;
	if (ret->StackedCompound == nullptr)
	{
		FastDelete(filledIdxs, idx);
	}
	else
	{
		ret->StackedCompound = nullptr;
	}
	return ret;
}