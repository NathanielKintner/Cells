#pragma once
#include <vector>
#include "Utilities.h"
#include "Compound.h"
#include "ReactionSpace.h"
#include "Organelle.h"



class Compound;
class Sector: public ReactionSpace
{
public:
	bool ContainsReactant();
	int GetReactantKey();
	Compound* GetReactantWithKey(int key);
	void ResolveSituation(int key);
	Compound* solution[25];
	std::vector<int> filledIdxs;
	std::vector<Organelle*> localPopulation;
	void AddCompoundToLocationInSolution(Compound * c, int location);
	void AddCompoundToRandomLocationInSolution(Compound* c);

	//THIS IS NOT THE INDEX IN THE SOLUTION, IT IS THE INDEX IN FILLEDIDXS
	Compound* RemoveCompoundByIdxInList(int idx);

	//temporary, perhaps
	//std::vector<Compound> sol;
};

