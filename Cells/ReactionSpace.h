#pragma once
#include "Compound.h";

class ReactionSpace
{
public:
	virtual bool ContainsReactant();
	virtual int GetReactantKey();
	virtual Compound* GetReactantWithKey(int key);
	//pass a -1 as the key if the solution does not need to break up the comp at key
	virtual void ResolveSituation(Compound pieces [], int numpieces, int key);
	virtual void AddCompoundToRandomLocationInSolution(Compound* c);
};

