#pragma once
#include "Sector.h"
#include "Compound.h"
#include "ReactionManager.h"
#include "HexGrids.h"
#include <vector>
#include <unordered_map>

class Sector;
class Organism;
class Compound;
namespace Universe
{
	extern std::vector<Sector> worldHexes;
	extern std::vector<Organism*> allLife;
	extern std::vector<Organism*> newLife;
	void init(int numsectors, int sectorsize);
	Compound& getByRelation(int& editSector, int& editSquare, int direction);
	extern int numxsectors;
	extern int numysectors;
	extern int sectorPixels;
	Sector& getSectorAtLocation(int x, int y);
}

