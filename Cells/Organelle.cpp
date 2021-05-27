#include "Organelle.h"
#include "Organism.h"
#include "Membrane.h"


void Organelle::init(Compound struc, int crit, Organism * parentPtr)
{
	parent = parentPtr;
	outerMembrane = nullptr;
	structure = struc;
	criticalRegion = crit;
	CriticalIdentity = GetCriticalCharge();
	UtilityMarker = 0;
}

int Organelle::mass()
{
	return structure.mass;
}

void Organelle::ConnectTo(Organelle* o)
{
	if (this == o)
	{
		return;
	}
	for (Organelle *oldO : connections)
	{
		if (oldO == o)
		{
			return;
		}
	}
	unsigned char metaData = connections.size();
	//try to connect, the other organelle may reject the connection
	char errorCode = o->ConnectOneWay(this, metaData);
	//if we are good, connect! metadata has been mutated and we can use it when we want the other organelle to disconnect from us
	if (errorCode == 0)
	{
		connections.emplace_back(o);
		connectionMetaData.emplace_back(metaData);
	}
}

//for a basic organelle, the connection metadata simply represents where the connection is sitting
//in that organelle's connections vector. that way, when we want that organelle to disconnect from us,
//they can send them their metadata so that they know exactly where to go to do the disconnect.
//effectively, this makes disconnecting into an O(1) process. Instead of searching through the list of
//connections to find us, we tell that organelle exactly where to look to delete us from their list.
//it is THEIR responsibility to give us metadata, and THEIR responsibility to interpret that data usefully
char Organelle::ConnectOneWay(Organelle* o, unsigned char & Metadata)
{
	//we must not connect to any membrane that is enveloping us, that does bad things to physics
	if (outerMembrane != nullptr)
	{
		Organelle* out = outerMembrane;
		do
		{
			if (out == o)
			{
				return -1;
			}
			//go to our outer membrane's outer membrane
			out = out->outerMembrane;
		} while (out != nullptr);
	}
	
	unsigned char out = connections.size();
	connections.emplace_back(o);
	connectionMetaData.emplace_back(Metadata);
	Metadata = out;
	return 0;
}

//severs all incoming connections, severing all outgoing connections is trivial and generally unnecessary
void Organelle::SeverAllConnections()
{
	isded = 1;
	int target = connections.size();//force an optimization, we dont care if connections changes size
	//tell all connections to disconnect from us
	for (int i = 0; i < target; i++)
	{
		connections[i]->DisconnectOneWay(connectionMetaData[i]);
	}
	//tell membrane to disconnect from us
	if (outerMembrane != nullptr)
	{
		outerMembrane->DisconnectOneWay(outerMembraneConnectionMetadata);
		if (structure.mass != 0)
		{
			Compound* c = new Compound();
			*c = structure;
			outerMembrane->innerSolution.emplace_back(c);
		}
	}
	else
	{
		if (parent->hasBeenPlaced)
		{
			Sector& s = Universe::getSectorAtLocation(xpos, ypos);
			if (structure.mass != 0)
			{
				Compound * c = new Compound();
				*c = structure;
				s.AddCompoundToRandomLocationInSolution(c);				
			}		
		}
	}
}

//disconnect from the organelle corresponding to the metadata
void Organelle::DisconnectOneWay(unsigned char Metadata)
{
	FastDelete(connections, Metadata);
	FastDelete(connectionMetaData, Metadata);
	if (connections.size() != Metadata)
	{
		//we just moved an entry, so if that entry tries to delete we will not be looking in the wrong place.
		//we send them an updated piece of metadata so that they can find us and then change our delete metadata
		connections[Metadata]->UpdateConnectionMetaData(connectionMetaData[Metadata], Metadata);
	}
}

void Organelle::UpdateConnectionMetaData(unsigned char MetadataToFindEntry, unsigned char MetadataToChangeEntryTo)
{
	connectionMetaData[MetadataToFindEntry] = MetadataToChangeEntryTo;
}

void Organelle::SendRepositionRequests()
{
	for (Organelle* o : connections)
	{
		int xdelta = xpos - o->xpos;
		int ydelta = ypos - o->ypos;
		double distance = sqrt(xdelta * xdelta + ydelta * ydelta);
		int desiredDistance = o->size() *2 + size() *2 + 10;
		if (abs(distance - desiredDistance) > 15)
		{
			double ratio = desiredDistance / (distance + 1);
			o->ReceiveRepositionRequest(xdelta - xdelta * ratio, ydelta - ydelta * ratio, 1);
		}
	}
	std::vector<Organelle*>* sameLevel = 0;
	if (outerMembrane == nullptr)
	{
		sameLevel = &(parent->AllOrganelles);
	}
	else
	{
		//reposition based on outermembrane
		int xdelta = xpos - outerMembrane->xpos;
		int ydelta = ypos - outerMembrane->ypos;
		int desiredDistance = outerMembrane->size() * 2 - size() * 2 - 3;
		if (abs(xdelta) + abs(ydelta) > desiredDistance)//if possible, do this much cheaper calulation and then skip
		{
			double distance = sqrt(xdelta * xdelta + ydelta * ydelta);
			if (distance > desiredDistance)
			{
				double ratio = desiredDistance / (distance + 1);
				outerMembrane->ReceiveRepositionRequest(xdelta - xdelta * ratio, ydelta - ydelta * ratio, 2);
			}
		}
		sameLevel = &(outerMembrane->innerOrganelles);
	}
	for (Organelle* o : *sameLevel)
	{
		if (outerMembrane == o->outerMembrane)
		{
			int xdelta = xpos - o->xpos;
			int ydelta = ypos - o->ypos;
			int desiredDistance = o->size() * 2 + size() * 2 + 10;
			if (abs(xdelta) + abs(ydelta) > desiredDistance)//if possible, do this much cheaper calulation and then skip
			{
				continue;
			}
			double distance = sqrt(xdelta * xdelta + ydelta * ydelta);
			if (distance < desiredDistance)
			{
				double ratio = desiredDistance / (distance + 1);
				o->ReceiveRepositionRequest(xdelta - xdelta * ratio, ydelta - ydelta * ratio, 3);
			}
		}
	}
}

void Organelle::ReceiveRepositionRequest(int deltax, int deltay, int force)
{
	xposRequest += deltax * force;
	yposRequest += deltay * force;
	totalRequestForce += force;
}

void Organelle::Reposition()
{
	int xmove = xposRequest / (totalRequestForce + 1);
	xpos += xmove;
	int ymove = yposRequest / (totalRequestForce + 1);
	ypos += ymove;
	xposRequest = xmove * 3;//continue to "coast" a little
	yposRequest = ymove * 3;
	totalRequestForce = 3;
}

void Organelle::Sense()
{
	parent->Brain.SetNNInputForChar(metaData3, Organelle::AlivenessPercentageGuess());
}

void Organelle::Activate()
{
	return;
}

int Organelle::size()
{
	return 5;
}


void Organelle::Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset)
{
	if (parent->center == this && false)
	{
		sf::Font font;
		font.loadFromFile("arial.ttf");
		sf::Text txt;
		txt.setFont(font);
		txt.setString(std::to_string(parent->energy));
		txt.setCharacterSize(20);
		txt.setPosition(staticXOffset + (xpos - size()) / zoom, staticYOffset - 30 + (ypos - size()) / zoom);
		txt.setOrigin(size() * 0.9 / zoom, size() * 0.9 / zoom);
		txt.setFillColor(sf::Color::Blue);
		window.draw(txt);
	}
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
}

void Organelle::DoChemistry(std::vector<Compound*>& reactants)
{
	ReactionSpace* solptr;
	if (outerMembrane == nullptr)
	{
		Sector& s = Universe::getSectorAtLocation(xpos, ypos);
		solptr = &s;
	}
	else
	{
		solptr = outerMembrane;
	}	
	if (!solptr->ContainsReactant())
	{
		return;
	}
	int randIdx = solptr->GetReactantKey();
	Compound * toReactWith = solptr->GetReactantWithKey(randIdx);
	int stability = 0;
	int ielem1;
	int ielem2;
	bool Comp1IsStructure = rand() % 2 == 1;
	int idxidx1 = 0;
	int idxidx2 = 0;
	Compound* comp1;
	Compound* comp2;

	int idxidxStructure = rand() % structure.filledIndices.size();
	int idxidxReactWith = rand() % toReactWith->filledIndices.size();

	if (Comp1IsStructure)
	{
		idxidx1 = idxidxStructure;
		idxidx2 = idxidxReactWith;
		comp1 = &structure;
		comp2 = toReactWith;
	}
	else
	{
		idxidx1 = idxidxReactWith;
		idxidx2 = idxidxStructure;
		comp1 = toReactWith;
		comp2 = &structure;
	}
	
	ielem1 = comp1->filledIndices[idxidx1];
	ielem2 = comp2->filledIndices[idxidx2];
	
	int emptyIdxs[8];
	int spaces = comp2->getUnPopulatedNeighborsIndices(ielem2, emptyIdxs);
	
	if (spaces == 0)	
	{
		return;
	}
	ielem2 = emptyIdxs[rand() % spaces];
	int x1, y1, x2, y2;
	comp1->TranslateFromIdxToXY(ielem1, x1, y1);
	comp2->TranslateFromIdxToXY(ielem2, x2, y2);
	int threshold = rand() % 21 - 10 + rand() % 21 - 10 + rand() % 21 - 10 + rand() % 21 - 10 + rand() % 21 - 10 + rand() % 35 - 35;
	
	CallReactionAndCleanUp(comp1, comp2, stability, ielem1, idxidx1, ielem2, true, threshold, *solptr, reactants, Comp1IsStructure, randIdx, randIdx);
	/*
	if (rand() % 2 == 1)//remove from structure	
	{
		if (!PerformReactionIfGoodEnough(&structure, &toReactWith, , stability, ielem1, ielem2))
		{
			return;
		}
		if (structure.mass == 0)
		{
			reactants.emplace_back(&toReactWith);
		}
		else
		{
			int numPieces;
			Compound* pieces = structure.SplitCompound(ielem1, numPieces);
			if(pieces == nullptr){
				reactants.emplace_back(&structure);
				reactants.emplace_back(&toReactWith);
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
				Compound temp = toReactWith;
				FastDelete(sol, randIdx);
				for (int i = 0; i < numPieces; i++)
				{
					if (i != largest)
					{
						sol.emplace_back(pieces[i]);
					}
				}
				//note the unusual for loop indexing to avoid a later oboo
				for (int i = 1; i < numPieces; i++)
				{
					reactants.emplace_back(&sol[sol.size()-i]);
				}
				reactants.emplace_back(&structure);
				sol.emplace_back(temp);
				reactants.emplace_back(&sol[sol.size() - 1]);

				delete[] pieces;
			}
		}
	}
	else //add to structure
	{
		if (!PerformReactionIfGoodEnough(&toReactWith, &structure, rand() % 15 - 5, stability, ielem1, ielem2))
		{
			return;
		}
		if (toReactWith.mass == 0)
		{
			FastDelete(sol, randIdx);
			reactants.emplace_back(&structure);
		}
		else
		{

			int numPieces;
			Compound* pieces = toReactWith.SplitCompound(ielem1, numPieces);
			if (pieces == nullptr)
			{
				reactants.emplace_back(&structure);
				reactants.emplace_back(&toReactWith);
			}
			else
			{
				FastDelete(sol, randIdx);
				for (int i = 0; i < numPieces; i++)
				{
					sol.emplace_back(pieces[i]);
				}
				//note the unusual for loop indexing to avoid a later oboe
				for (int i = 1; i < numPieces+1; i++)
				{
					reactants.emplace_back(&sol[sol.size() - i]);
				}
				reactants.emplace_back(&structure);
				delete[] pieces;
			}
		}
	}*/
	//AdjustEnergyValues(stability, reactants);
	//reactants.clear();
}

bool Organelle::IsAlive()
{
	Element e = GetCriticalCharge();
	if (e.red < 1)
	{
		return false;
	}
	//as red increases (and therefore so does mass and size), so does the tolerance for damage
	int threshold = CriticalIdentity.red / 2;
	if (abs(e.red - CriticalIdentity.red) > threshold)
	{
		return false;
	}
	if (abs(e.blue - CriticalIdentity.blue) > threshold)
	{
		return false;
	}
	if (abs(e.green - CriticalIdentity.green) > threshold)
	{
		return false;
	}
	return true;
}


//an estimation for how close this organelle is to dying, based on the same calculations as IsAlive()
int Organelle::AlivenessPercentageGuess()
{
	Element e = GetCriticalCharge();
	if (e.red < 1)
	{
		return 0;
	}
	int threshold = CriticalIdentity.red / 2;
	return threshold * 3 - abs(e.red - CriticalIdentity.red) - abs(e.blue - CriticalIdentity.blue) - abs(e.green - CriticalIdentity.green);
}

Element Organelle::GetCriticalCharge()
{
	Element ret = Element();
	int structsize = structure.composition.size();
	if (structsize < criticalRegion*criticalRegion)
	{
		ret.red = -1;
		return ret;
	}
	for (int i = (-1*((criticalRegion-1)/2)); i <= criticalRegion/2; i++)
	{
		for (int j = (-1 * ((criticalRegion - 1) / 2)); j <= criticalRegion / 2; j++)
		{			
			int idx = structure.TranslateFromXYToIdx(i, j);
			if (idx > -1 && idx < structsize)
			{
				const Element& e = structure.composition[idx];
				if (e.red != 0)
				{
					ret.red += e.red;
					ret.blue += e.blue;
					ret.green += e.green;
				}
			}			
		}
	}
	return ret;
}

void Organelle::CheckConnectionDeath(std::list<Organelle*>& border)
{
	for (Organelle* o : connections)
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
	if (outerMembrane != nullptr && outerMembrane->UtilityMarker == 0)
	{
		outerMembrane->UtilityMarker = 2;
		border.push_back(outerMembrane);
	}
	else if (outerMembrane != nullptr && outerMembrane->UtilityMarker == 1)
	{
		int asdf = 1;
	}
}

void Organelle::GetImmediateFamily(std::list<Organelle*>& retList)
{
	for (Organelle* o : connections)
	{
		retList.push_back(o);
	}
}

bool Organelle::CheckRep()
{
	return true;
}