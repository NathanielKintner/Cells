#include "ReactionSpace.h"

bool ReactionSpace::ContainsReactant()
{
	return false;
}

int ReactionSpace::GetReactantKey()
{
	return -1;
}

Compound* ReactionSpace::GetReactantWithKey(int key)
{
	return nullptr;
}

void ReactionSpace::ResolveSituation(int key)
{
	return;
}

void ReactionSpace::AddCompoundToRandomLocationInSolution(Compound* c)
{
	return;
}