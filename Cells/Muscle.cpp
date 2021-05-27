#include "Muscle.h"

void Muscle::Activate()
{
	if (parent->energy < 0)
	{
		return;
	}
	if (parent->Brain.GetNNOutputForChar(metaData4) < 0)
	{
		return;
	}
	int xdiff = parent->Brain.GetNNOutputForChar(metaData1)/15;
	int ydiff = parent->Brain.GetNNOutputForChar(metaData2)/15;
	parent->energy -= (abs(xdiff)+abs(ydiff))/10;
	xpos += xdiff;
	ypos += ydiff;
}
void Muscle::init(Compound struc, int crit, Organism* parentPtr)
{
	Organelle::init(struc, crit, parentPtr);
}