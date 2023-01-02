#pragma once

class Sector;

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
	Compound* solution[25] = { nullptr };
	std::vector<int> filledIdxs;
	void AddCompoundToLocationInSolution(Compound * c, int location);
	void AddCompoundToRandomLocationInSolution(Compound* c);

	//THIS IS NOT THE INDEX IN THE SOLUTION, IT IS THE INDEX IN FILLEDIDXS
	Compound* RemoveCompoundByIdxInList(int idx);


	ConnectionNode<Sector*, Organelle*> localPopulation;

	Sector()
	{
		localPopulation.parent = this;
		//memset(solution,0,sizeof(Compound) * 25);
	}

	//temporary, perhaps
	//std::vector<Compound> sol;
};

