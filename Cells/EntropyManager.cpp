#include "EntropyManager.h"

bool ResolveMovement(int direction, int& startSector, int& startLocation, Compound& movingComp);

int xReturnSpot = 20;
int yReturnSpot = 20;
int xVelocity = 1;
int yVelocity = 1;

void DoEntropy(int secIdx)
{
	if (rand() % 5 != 0)
	{
		return;
	}
	Sector& s = Universe::worldHexes[secIdx];
	int xpos = secIdx % Universe::numxsectors;//need to double-check this
	int ypos = secIdx / Universe::numxsectors;//need to double-check this
	//a temporary, extremely simple, extremely random entropy routine
	if (s.filledIdxs.size() != 0)
	{
		int delidx = rand() % s.filledIdxs.size();
		int solutionLocation = s.filledIdxs[delidx];
		Compound * c = s.RemoveCompoundByIdxInList(delidx);
		int solutionx = solutionLocation % 5;
		int solutiony = solutionLocation / 5;
		solutionx += rand() % 3 - 1;
		solutiony += rand() % 3 - 1;
		while (solutionx > 4)
		{
			solutionx -= 5;
			xpos++;
		}
		while (solutionx < 0)
		{
			solutionx += 5;
			xpos--;
		}
		while (solutiony > 4)
		{
			solutiony -= 5;
			ypos++;
		}
		while (solutiony < 0)
		{
			solutiony += 5;
			ypos--;
		}
		//if a compound would go off the edge of the map, bring it around to the other side
		xpos = (xpos + Universe::numxsectors) % Universe::numxsectors;
		ypos = (ypos + Universe::numysectors) % Universe::numysectors;
		//sometimes, give an energy boost
		if (rand() % 125 == 0)
		{
			c->internalEnergy += 250;
		}
		Universe::worldHexes[xpos + ypos * Universe::numxsectors].AddCompoundToLocationInSolution(c, solutionx + solutiony*5);
		//s.AddCompoundToRandomLocationInSolution(c);
		return;
		//FastDelete(s.sol, delidx);
		//adjust the position randomly
		if ((rand() % 1) == 0)
		{
			xpos += (rand() % 3) - 1;
			//ypos += (rand() % 3) - 1;
			ypos += (rand() % 2) + (rand() % 2) + (rand() % 2) - 1;
			
		}
		//if a compound would go off the edge of the map, bring it around to the other side
		xpos = (xpos + Universe::numxsectors) % Universe::numxsectors;		
		ypos = (ypos + Universe::numysectors) % Universe::numysectors;

		if (ypos < 0)
		{
			ypos = 0;
		}
		if (ypos >= Universe::numxsectors)
		{
			ypos = Universe::numxsectors - 1;
		}
		
		//sometimes, put it in a new spot, and give it a boost of energy
		if (rand()%500 == 0)
		{
			c->internalEnergy += 300;
			if ((rand() % 50) == 0)
			{
				xReturnSpot += xVelocity;
				xReturnSpot = (xReturnSpot + Universe::numxsectors) % Universe::numxsectors;
				yReturnSpot += yVelocity;
				yReturnSpot = (yReturnSpot + Universe::numysectors) % Universe::numysectors;
				if (rand() % 3 == 0)
				{
					xVelocity += (rand() % 5) - 2;
					if (abs(xVelocity) > 3)
					{
						xVelocity = xVelocity / 2;
					}
				}
				if (rand() % 3 == 0)
				{
					yVelocity += (rand() % 5) - 2;
					if (abs(yVelocity) > 3)
					{
						yVelocity = yVelocity / 2;
					}
				}				
			}
			xpos = xReturnSpot;
			ypos = yReturnSpot;
		}
		
		//place the compound at the new position
		//Universe::worldHexes[xpos + ypos*Universe::numxsectors].sol.emplace_back(c);
	}
	return;/*
	if (s.filledIdxs.size() == 0)
	{
		return;
	}
	int entropyIdx = rand() % s.filledIdxs.size();

	int currsector = secIdx;
	int currlocation = s.filledIdxs[entropyIdx];

	Compound c = s.solution[currlocation];
	s.RemoveCompoundAtIdx(entropyIdx);

	int currVert = abs(c.Vvert / (c.mass * 10));
	int currHoriz = abs(c.Vhoriz / (c.mass * 10));
	int total = currHoriz + currVert;

	while (total != 0)
	{
		int selection = rand() % total;
		selection = selection - currHoriz;
		if (selection < 0)
		{
			currHoriz--;
			int direction = 0;
			if (c.Vhoriz < 0)
			{
				direction = 1;
			}
			else
			{
				direction = 3;
			}
			if (!ResolveMovement(direction, currsector, currlocation, c))
			{
				//Universe::worldHexes[currsector].AddCompoundToLocationInSolution(c, currlocation);
				//c = Universe::getByRelation(direction, currsector, currlocation);
				break;
			}
		}
		else
		{
			currVert--;
			int direction = 0;
			if (c.Vvert < 0)
			{
				direction = 0;
			}
			else
			{
				direction = 2;
			}
			if (!ResolveMovement(direction, currsector, currlocation, c))
			{
				//Universe::worldHexes[currsector].AddCompoundToLocationInSolution(c, currlocation);
				//c = Universe::getByRelation(direction, currsector, currlocation);
				break;
			}
		}
		total--;		
	}


	int xpos = currsector % Universe::numxsectors;
	if (xpos > Universe::numxsectors / 2)
	{
		c.Vhoriz += c.mass;
	}
	else
	{
		c.Vhoriz -= c.mass;
	}
	int ypos = currsector / Universe::numxsectors;
	if (ypos > Universe::numysectors / 2)
	{
		c.Vvert -= c.mass;
	}
	else
	{
		c.Vvert += c.mass;
	}
	if (abs(xpos - Universe::numxsectors / 2) > abs(ypos - Universe::numysectors / 2))
	{
		if (xpos > Universe::numxsectors / 2)
		{
			c.Vhoriz += c.mass;
		}
		else
		{
			c.Vhoriz -= c.mass;
		}
	}
	else
	{
		if (ypos > Universe::numysectors / 2)
		{
			c.Vvert -= c.mass;
		}
		else
		{
			c.Vvert += c.mass;
		}
	}
	c.Vvert = (rand() % 50 - 25) * c.mass;//GET RID OF THESE TO REACTIVATE GRAVITY
	c.Vhoriz = (rand() % 50 - 25) * c.mass;//GET RID OF THESE TO REACTIVATE GRAVITY
	c.Vvert += rand() % 50 - 25;
	c.Vhoriz += rand() % 50 - 25;
	Universe::worldHexes[currsector].AddCompoundToLocationInSolution(c, currlocation);*/
}

bool ResolveMovement(int direction, int& startSector, int &startLocation, Compound& movingComp)
{
	int editsecnum = startSector;
	int editlocation = startLocation;
	Compound &runinto = Universe::getByRelation(editsecnum, editlocation, direction);
	if (runinto.mass == 0)
	{
		startSector = editsecnum;
		startLocation = editlocation;
		return true;
	}
	else
	{
		///resolve bumping into something
		int horizVelDiff = runinto.Vhoriz - movingComp.Vhoriz;
		int vertVelDiff = runinto.Vvert - movingComp.Vvert;

		runinto.Vvert -= vertVelDiff;
		runinto.Vhoriz -= horizVelDiff;

		movingComp.Vvert += vertVelDiff;
		movingComp.Vhoriz += horizVelDiff;
		
		return false;
	}
}
