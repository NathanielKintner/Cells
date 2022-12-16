#pragma once
#include "Compound.h";

class ReactionSpace
{
public:
	virtual bool ContainsReactant();
	virtual int GetReactantKey();
	virtual Compound* GetReactantWithKey(int key);
	//the following comment has been made irrelevant, but we'll keep it for now in case im looking at old code
	//pass a -1 as the key if the solution does not need to break up the comp at key
	virtual void ResolveSituation(int key);
	virtual void AddCompoundToRandomLocationInSolution(Compound* c);
};

