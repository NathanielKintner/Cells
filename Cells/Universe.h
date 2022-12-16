#pragma once
#include "Sector.h"
#include "Compound.h"
#include "ReactionManager.h"
#include "HexGrids.h"
#include "Organelle.h"
#include <vector>
#include <unordered_map>

class Sector;
class Organelle;
class Compound;
namespace Universe
{
	extern std::vector<Sector> worldHexes;
	extern std::vector<Organelle*> allLife;
	extern std::vector<Organelle*> newLife;
	void init(int numsectors, int sectorsize);
	Compound& getByRelation(int& editSector, int& editSquare, int direction);
	extern int numxsectors;
	extern int numysectors;
	extern int sectorPixels;
	Sector& getSectorAtLocation(int x, int y);
	void addOrganelleToLocalPopulation(int xpos, int ypos, Organelle* o);
}

