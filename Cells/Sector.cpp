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
	return rand() % filledIdxs.size();
}

Compound* Sector::GetReactantWithKey(int key)
{
	return solution[filledIdxs[key]];
}

void Sector::ResolveSituation(Compound pieces[], int numpieces, int key)
{
	if (key != -1)
	{		
		int solutionLocation = filledIdxs[key];
		if (solution[solutionLocation]->mass == 0 || numpieces != 1)
		{
			Compound * removedComp = RemoveCompoundByIdxInList(key);
			delete removedComp;
		}
		for (int i = 0; i < numpieces; i++)
		{
			Compound* newComp = new Compound();
			*newComp = pieces[i];
			AddCompoundToLocationInSolution(newComp, key);
		}		
	}
	else
	{
		for (int i = 0; i < numpieces; i++)
		{
			Compound* newComp = new Compound();
			*newComp = pieces[i];
			AddCompoundToLocationInSolution(newComp, rand() % 25);
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