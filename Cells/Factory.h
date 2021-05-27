#pragma once
#include "Organelle.h"
#include "Organism.h"
#include "GeneticsManager.h"
#include "ReactionManager.h"
#include <string>
#include <sstream>
#include <vector>
#include <list>

class Organelle;

class Factory: public Organelle
{
public:
	std::vector<std::string> toDo;
	std::list<Compound> complete;
	std::vector<int> openborder;
	Compound progress;
	Compound current;

	void Activate();
	void Sense();
	void init(Compound struc, int crit, Organism* parentPtr);
	virtual int mass();
	virtual void SeverAllConnections();
	void AdvanceGoalCursor();
	virtual ~Factory() {/* stuff */ }
};