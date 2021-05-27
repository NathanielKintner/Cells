#include "Membrane.h"


void Membrane::SendRepositionRequests()
{
	Organelle::SendRepositionRequests();
	for(Organelle * o: innerOrganelles)
	{
		int xdelta = xpos - o->xpos;
		int ydelta = ypos - o->ypos;
		double distance = sqrt(xdelta * xdelta + ydelta * ydelta);
		int desiredDistance = size() * 2 - o->size() * 2 - 3;
		if (desiredDistance - distance < 0)
		{
			double ratio = desiredDistance / (distance + 1);
			o->ReceiveRepositionRequest(xdelta - xdelta * ratio, ydelta - ydelta * ratio, 6);
		}
	}
}

void Membrane::Sense()
{
	Organelle::Sense();
	parent->Brain.SetNNInputForChar(metaData4, innerSolution.size()-5);
}

void Membrane::Activate()
{
	//if we are exhausted, we can't do anything
	if (parent->energy < 0)
	{
		return;
	}
	//think
	if (parent->Brain.GetNNOutputForChar(metaData2) < 10)
	{
		return;
	}
	Sector& s = Universe::getSectorAtLocation(xpos, ypos);
	if (!s.ContainsReactant())
	{
		return;
	}
	std::vector<int> out;
	int stability;
	std::vector<Compound*> reactants(0);
	//add missing elements
	for (int idx : idealArrangement.filledIndices)
	{
		int x, y;
		out.clear();
		idealArrangement.TranslateFromIdxToXY(idx, x, y);
		int strucpos = structure.TranslateFromXYToIdx(x, y);
		if (strucpos >= structure.composition.size())
		{
			continue;
		}
		structure.GetPopulatedNeighbors(strucpos, out);
		if (out.size() == 0)
		{
			continue;
		}
		Element elemStruc = structure.composition[strucpos];
		Element elemIdeal = idealArrangement.composition[idx];
		if (elemStruc.red == 0)
		{
			int chosenComp = s.GetReactantKey();
			Compound * compEnv = s.GetReactantWithKey(chosenComp);
			for (int idxidx1 = 0; idxidx1 < compEnv->filledIndices.size(); idxidx1++)
			{
				int ielem1 = compEnv->filledIndices[idxidx1];
				Element e = compEnv->composition[ielem1];
				if (e.red == 0)
				{
					continue;
				}
				if (e.name == elemIdeal.name)
				{
					CallReactionAndCleanUp(compEnv, &structure, stability, ielem1, idxidx1, strucpos, false, 0, s, reactants, false, chosenComp, chosenComp);
					if (structure.internalEnergy < 0)
					{
						parent->energy += structure.internalEnergy;
						structure.internalEnergy = 0;
					}
					/*
					int xenv, yenv;
					compEnv.TranslateFromIdxToXY(ielem1, xenv, yenv);
					int stability = compEnv.GetStabilityAtPoint(xenv, yenv) + structure.GetStabilityAtPoint(x, y);
					DoReaction(compEnv, structure, xenv, yenv, x, y, idxidx1);
					stability = compEnv.GetStabilityAtPoint(xenv, yenv) + structure.GetStabilityAtPoint(x, y) - stability;

					std::vector<Compound*> reactants;
					reactants.emplace_back(&structure);

					if (compEnv.mass == 0)
					{
						FastDelete(s.sol, chosenComp);
					}
					else
					{
						//manage breaking up the compound from the environment
						int numPieces = 0;
						Compound* pieces = compEnv.SplitCompound(ielem1, numPieces);
						if (pieces == nullptr)
						{
							reactants.emplace_back(&compEnv);
						}
						else
						{
							for (int j = 0; j < numPieces; j++)
							{
								s.sol.emplace_back(pieces[j]);
							}
							FastDelete(s.sol, chosenComp);
							int target = s.sol.size() - 1;
							for (int j = 0; j < numPieces; j++)
							{
								reactants.emplace_back(&s.sol[target - j]);
							}
							delete[] pieces;
						}
					}
					//drain energy if this was a disadvantageous reaction
					AdjustEnergyValues(stability, reactants);
					if (compEnv.internalEnergy < 0)
					{
						structure.internalEnergy += compEnv.internalEnergy;
						compEnv.internalEnergy = 0;
					}
					if (structure.internalEnergy < 0)
					{
						parent->energy += structure.internalEnergy;
						structure.internalEnergy = 0;
					}
					std::vector<int> neighbors(0);
					return;*/
					return;
				}
			}
		}
	}
	if (structure.filledIndices.size() < 2)
	{
		return;
	}
	//remove incorrect elements
	for (int idxidx = 0; idxidx < structure.filledIndices.size(); idxidx++)
	{
		int x, y;
		int idx = structure.filledIndices[idxidx];
		structure.TranslateFromIdxToXY(idx, x, y);
		Element elemIdeal;
		if (idealArrangement.TranslateFromXYToIdx(x, y) >= 0 && idealArrangement.TranslateFromXYToIdx(x, y) < idealArrangement.composition.size())
		{
			elemIdeal = idealArrangement.composition[idealArrangement.TranslateFromXYToIdx(x, y)];
		}
		else
		{
			elemIdeal = Element();
			elemIdeal.red == 0;
		}
		Element elemStruc = structure.composition[idx];
		if (elemIdeal.red == 0 || elemIdeal.name != elemStruc.name)
		{
			int chosenComp = s.GetReactantKey();
			Compound* compEnv = s.GetReactantWithKey(chosenComp);
			int envidxidx = rand() % compEnv->filledIndices.size();
			int chosenSpot = compEnv->filledIndices[envidxidx];
			int openSpots[8];
			int totalOpenSpots = compEnv->getUnPopulatedNeighborsIndices(chosenSpot, openSpots);
			if (totalOpenSpots == 0)
			{
				return;
			}
			chosenSpot = openSpots[rand()%totalOpenSpots];
			CallReactionAndCleanUp(&structure, compEnv, stability, idx, idxidx, chosenSpot, false, 0, s, reactants, true, chosenComp, chosenComp);
			if (compEnv->internalEnergy < 0)
			{
				parent->energy += compEnv->internalEnergy;
				compEnv->internalEnergy = 0;
			}
			/*
			int xenv, yenv;
			compEnv.TranslateFromIdxToXY(chosenSpot, xenv, yenv);
			int stability = compEnv.GetStabilityAtPoint(xenv, yenv) + structure.GetStabilityAtPoint(x, y);
			DoReaction(structure, compEnv, x, y, xenv, yenv, idxidx);
			stability = compEnv.GetStabilityAtPoint(xenv, yenv) + structure.GetStabilityAtPoint(x, y) - stability;

			std::vector<Compound*> reactants;
			reactants.emplace_back(&compEnv);

			//manage breaking up the structure
			int numPieces;
			Compound* pieces = structure.SplitCompound(x, y, numPieces);
			if (pieces == nullptr)
			{
				reactants.emplace_back(&structure);
				reactants.emplace_back(&compEnv);
			}
			else
			{
				int largest = -1;
				int size = -1;
				for (int i = 0; i < numPieces; i++)
				{
					if (pieces[i].mass > size)
					{
						largest = i;
						size = pieces[i].mass;
					}
				}
				structure = pieces[largest];
				Compound temp = compEnv;
				FastDelete(s.sol, chosenComp);
				for (int i = 0; i < numPieces; i++)
				{
					if (i != largest)
					{
						s.sol.emplace_back(pieces[i]);
					}
				}
				//note the unusual for loop indexing to avoid a later oboo
				for (int i = 1; i < numPieces; i++)
				{
					reactants.emplace_back(&s.sol[s.sol.size() - i]);
				}
				reactants.emplace_back(&structure);
				s.sol.emplace_back(temp);
				reactants.emplace_back(&s.sol[s.sol.size() - 1]);

				delete[] pieces;
			}
			//drain energy if this was a disadvantageous reaction
			AdjustEnergyValues(stability, reactants);
			if (compEnv.internalEnergy < 0)
			{
				structure.internalEnergy += compEnv.internalEnergy;
				compEnv.internalEnergy = 0;
			}
			if (structure.internalEnergy < 0)
			{
				parent->energy += structure.internalEnergy;
				structure.internalEnergy = 0;
			}
			std::vector<int> neighbors(0);
			return;*/
			return;
		}
	}
	
}

int Membrane::mass()
{
	int mass = 0;
	for (Compound * c : innerSolution)
	{
		mass += c->mass;
	}
	return Organelle::mass() + mass;
}

int Membrane::size()
{
	return metaData1*6;
}

void Membrane::init(Compound struc, int crit, Organism* parentPtr)
{
	Organelle::init(struc, crit, parentPtr);
	firstConnectionMade = false;
	idealArrangement = struc;
	metaData1 = metaData1 % 4 + 1;
}

void Membrane::ConnectTo(Organelle* o)
{
	firstConnectionMade = true;
	Organelle::ConnectTo(o);
}

char Membrane::ConnectOneWay(Organelle* o, unsigned char& Metadata)
{
	if (firstConnectionMade)
	{
		//we also must not connect to any organelle that we are enveloping, does bad things to physics
		if (o->outerMembrane != nullptr)
		{
			Organelle* out = o->outerMembrane;
			do
			{
				if (out == this)
				{
					return -1;
				}
				//go to their outer membrane's outer membrane
				out = out->outerMembrane;
			} while (out != nullptr);
		}
		return Organelle::ConnectOneWay(o, Metadata);
	}
	else
	{
		firstConnectionMade = true;
		//this is the first connection we are making! We are a membrane, so this means trying to envelop some
		//amount of the preexisting organelles. First, lets see if the thing we are trying to envelop is too big 
		if (o->size() >= size())
		{
			//it was too big... oh well. Lets just connect normally.
			return Organelle::ConnectOneWay(o, Metadata);
		}
		//we CAN encompass the first organelle!
		//now, we need to make sure we arent inside a smaller or equal-sized membrane, so
		//if the first thing we intend to "eat" (that is, o) is surrounded by another membrane that we cant fit inside,
		//then we try to eat that membrane instead, getting bigger if necessary
		if (o->outerMembrane != nullptr && o->outerMembrane->size() <= size())
		{
			while (o->outerMembrane != nullptr && o->outerMembrane->size() <= size())
			{
				//change targets
				o = o->outerMembrane;
				//get bigger if we have to (happens if we were the same size as the membrane around what we were going to eat)
				if (o->size() == size())
				{
					metaData1++;
				}
			}
		}
		//ok, we are now 100% sure that we are not inside something that is too small, or trying to encompass something too big
		
		//this will be the number of layers of organelles that we will try to encompass in a breadth-first search
		unsigned char metacopy = metaData1;
		//first target gets marked with the max layer
		o->UtilityMarker = metacopy;
		std::list<Organelle*> border;
		border.push_back(o);
		std::list<Organelle*> neighbors;
		while (metacopy != 0 && border.size() != 0)
		{
			Organelle* oo = border.front();
			border.pop_front();
			//if we see an organelle that is marked with the current layer, go to the next layer
			if (oo->UtilityMarker == metacopy)
			{
				metacopy--;
			}
			oo->GetImmediateFamily(neighbors);
			//neighbors is now populated with all the adjacent organelles
			for (Organelle* ooo : neighbors)
			{
				//if it has an outer membrane that is not our outer membrane,
				if (ooo->outerMembrane != nullptr && ooo->outerMembrane != outerMembrane)
				{
					//find the outermost membrane that isn't also surrounding us
					while (ooo->outerMembrane != nullptr && ooo->outerMembrane != outerMembrane)
					{
						ooo = ooo->outerMembrane;
					}
				}
				//if it's too big, move on
				if (ooo->size() >= size())
				{
					continue;
				}
				//if we havent already marked this, do so and then add it to the border of the search
				if (ooo->UtilityMarker == 0)
				{
					ooo->UtilityMarker = metacopy;
					border.push_back(ooo);
				}
			}			
		}
		for (Organelle* oo : parent->AllOrganelles)
		{
			if (oo->UtilityMarker != 0)
			{
				if (oo->outerMembrane != nullptr)
				{
					oo->outerMembrane->DisconnectOneWay(oo->outerMembraneConnectionMetadata);
				}
				oo->outerMembrane = this;
				innerOrganelles.emplace_back(oo);
				unsigned char meta = (unsigned char)((char)(innerOrganelles.size()) * -1);
				//note, this is 1 indexed and negative,
				//therefore large and positive when interpreted as unsigned
				oo->outerMembraneConnectionMetadata = meta;
				oo->UtilityMarker = 0;
			}
		}
		return -1;
	}
}

void Membrane::DisconnectOneWay(unsigned char Metadata)
{
	if (((char)Metadata) >= 0)
	{
		Organelle::DisconnectOneWay(Metadata);
	}
	else
	{
		unsigned char negMetaData = ((char)Metadata) * -1 - 1;
		FastDelete(innerOrganelles, negMetaData);
		if (innerOrganelles.size() != negMetaData)
		{
			//we just moved an entry, so if that entry tries to get us to delete them we will be looking in the wrong place.
			//we send them an updated piece of metadata so that they can find us on their end and then change our delete metadata
			innerOrganelles[negMetaData]->outerMembraneConnectionMetadata = Metadata;
		}
	}

}

void Membrane::UpdateConnectionMetaData(unsigned char MetadataToFindEntry, unsigned char MetadataToChangeEntryTo)
{
	if ((char)MetadataToFindEntry >= 0)
	{
		Organelle::UpdateConnectionMetaData(MetadataToFindEntry, MetadataToChangeEntryTo);
	}

}

void Membrane::SeverAllConnections()
{	
	if (outerMembrane != nullptr)
	{
		for(Compound * c: innerSolution)
		{
			outerMembrane->innerSolution.emplace_back(c);
		}
	}
	else
	{
		if (parent->hasBeenPlaced)
		{
			Sector& s = Universe::getSectorAtLocation(xpos, ypos);		
			for (Compound * c : innerSolution)
			{
				s.AddCompoundToRandomLocationInSolution(c);
			}
		}
	}
	for (Organelle* o : innerOrganelles)
	{
		if (o == nullptr)
		{
			int asdffdsa = 1;
		}
		o->outerMembrane = outerMembrane;
		if (outerMembrane != nullptr)
		{
			outerMembrane->innerOrganelles.push_back(o);
			unsigned char meta = (outerMembrane->innerOrganelles.size() * -1);
			o->outerMembraneConnectionMetadata = meta;
		}
		else
		{
			o->outerMembraneConnectionMetadata = -1;
		}
	}
	Organelle::SeverAllConnections();
}

void Membrane::Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset)
{
	sf::CircleShape drawOrg((size() * 1.8) / zoom, 20);
	drawOrg.setFillColor(sf::Color(0, 0, 0, 0));
	drawOrg.setOutlineThickness(10.0 / zoom);
	drawOrg.setOutlineColor(structure.GetColor());
	drawOrg.setPosition(staticXOffset + (xpos - size()) / zoom, staticYOffset + (ypos - size()) / zoom);
	drawOrg.setOrigin(size() * 0.9 / zoom, size() * 0.9 / zoom);
	window.draw(drawOrg);
	for (Organelle* ooo : connections)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(staticXOffset + xpos / zoom, staticYOffset + ypos / zoom), sf::Color(0,0,0,255)),
			sf::Vertex(sf::Vector2f(staticXOffset + ooo->xpos / zoom, staticYOffset + ooo->ypos / zoom), sf::Color(0,0,0,255))
		};

		window.draw(line, 2, sf::Lines);
	}
	for (Organelle* ooo : innerOrganelles)
	{
		int xdif = abs(xpos - ooo->xpos) + 2;
		int xjump = rand() % (2 * xdif) + 1 - xdif;
		int xmid = ((xpos + ooo->xpos) / 2) + xjump;
		int ydif = abs(ypos - ooo->ypos) + 2;
		int yjump = rand() % (2 * ydif) + 1 - ydif;
		int ymid = ((ypos + ooo->ypos) / 2) + yjump;

		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(staticXOffset + xpos / zoom, staticYOffset + ypos / zoom), sf::Color(200,0,0,255)),
			sf::Vertex(sf::Vector2f(staticXOffset + xmid / zoom, staticYOffset + ymid / zoom), sf::Color(200,0,0,255))
		};

		window.draw(line, 2, sf::Lines);

		sf::Vertex line2[] =
		{
			sf::Vertex(sf::Vector2f(staticXOffset + xmid / zoom, staticYOffset + ymid / zoom), sf::Color(200, 0, 0, 255)),
			sf::Vertex(sf::Vector2f(staticXOffset + ooo->xpos / zoom, staticYOffset + ooo->ypos / zoom), sf::Color(200, 0, 0, 255))
		};

		window.draw(line2, 2, sf::Lines);
	}
}

void Membrane::CheckConnectionDeath(std::list<Organelle*>& border)
{
	Organelle::CheckConnectionDeath(border);
	for (Organelle* o : innerOrganelles)
	{
		if (o->UtilityMarker == 0)
		{
			o->UtilityMarker = 2;
			border.push_back(o);
		}
		else if (o->UtilityMarker == 1)
		{
			int asdf = 1;
		}
	}
}

bool Membrane::CheckRep()
{
	if (outerMembrane != nullptr && metaData1 >= outerMembrane->metaData1)
	{
		return false;
	}
	for (Organelle* o : innerOrganelles)
	{
		if (o == nullptr)
		{
			return false;
		}
		if (o->outerMembrane != this)
		{
			return false;
		}
		for (Organelle* oo : connections)
		{
			if (oo == o)
			{
				return false;
			}
		}
	}
	return true;
}



bool Membrane::ContainsReactant()
{
	return innerSolution.size() != 0;
}

int Membrane::GetReactantKey()
{
	return rand() % innerSolution.size();
}

Compound* Membrane::GetReactantWithKey(int key)
{
	return innerSolution[key];
}

void Membrane::ResolveSituation(Compound pieces[], int numpieces, int key)
{
	if (key != -1)
	{
		if (innerSolution[key]->mass == 0 || numpieces != 1)
		{
			delete innerSolution[key];
			FastDelete(innerSolution, key);
		}
	}
	for (int i = 0; i < numpieces; i++)
	{
		Compound* newComp = new Compound();
		*newComp = pieces[i];
		innerSolution.emplace_back(newComp);
	}
}

void Membrane::AddCompoundToRandomLocationInSolution(Compound* c)
{
	innerSolution.emplace_back(c);
}