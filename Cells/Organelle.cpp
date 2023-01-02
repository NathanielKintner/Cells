#include "Organelle.h"
#include "Organism.h"
#include "Membrane.h"
#include "NeuralNet.h"


void Organelle::init()
{
	//criticalRegion = crit;
	//CriticalIdentity = GetCriticalCharge();
	UtilityMarker = 0;
	connections.parent = this;
	outerMembrane.parent = this;
	localArea.parent = this;
}


//void Organelle::tempinit()
//{
//	outerMembrane = nullptr;
//	UtilityMarker = 0;
//	//make a random structure
//	structure = Compound();
//}

int Organelle::mass()
{
	return structure.mass;
}

//void Organelle::ConnectTo(Organelle* o)
//{
//	if (this == o)
//	{
//		return;
//	}
//	for (Organelle *oldO : connections)
//	{
//		if (oldO == o)
//		{
//			return;
//		}
//	}
//	unsigned char metaData = connections.size();
//	//try to connect, the other organelle may reject the connection
//	char errorCode = o->ConnectOneWay(this, metaData);
//	//if we are good, connect! metadata has been mutated and we can use it when we want the other organelle to disconnect from us
//	if (errorCode == 0)
//	{
//		connections.emplace_back(o);
//		connectionMetaData.emplace_back(metaData);
//	}
//}
//
////for a basic organelle, the connection metadata simply represents where the connection is sitting
////in that organelle's connections vector. that way, when we want that organelle to disconnect from us,
////they can send them their metadata so that they know exactly where to go to do the disconnect.
////effectively, this makes disconnecting into an O(1) process. Instead of searching through the list of
////connections to find us, we tell that organelle exactly where to look to delete us from their list.
////it is THEIR responsibility to give us metadata, and THEIR responsibility to interpret that data usefully
//char Organelle::ConnectOneWay(Organelle* o, unsigned char & Metadata)
//{
//	//we must not connect to any membrane that is enveloping us, that does bad things to physics
//	if (outerMembrane != nullptr)
//	{
//		Organelle* out = outerMembrane;
//		do
//		{
//			if (out == o)
//			{
//				return -1;
//			}
//			//go to our outer membrane's outer membrane
//			out = out->outerMembrane;
//		} while (out != nullptr);
//	}
//	
//	unsigned char out = connections.size();
//	connections.emplace_back(o);
//	connectionMetaData.emplace_back(Metadata);
//	Metadata = out;
//	return 0;
//}
//
////severs all incoming connections, severing all outgoing connections is trivial and generally unnecessary
//void Organelle::SeverAllConnections()
//{
//	for (Organelle* o : innerOrganelles)
//	{
//		if (o == nullptr)
//		{
//			int asdffdsa = 1;
//		}
//		o->outerMembrane = outerMembrane;
//		if (outerMembrane != nullptr)
//		{
//			outerMembrane->innerOrganelles.push_back(o);
//			unsigned char meta = (outerMembrane->innerOrganelles.size() * -1);
//			o->outerMembraneConnectionMetadata = meta;
//		}
//		else
//		{
//			o->outerMembraneConnectionMetadata = -1;
//		}
//	}
//	isded = 1;
//	int target = connections.size();//force an optimization, we dont care if connections changes size
//	//tell all connections to disconnect from us
//	for (int i = 0; i < target; i++)
//	{
//		connections[i]->DisconnectOneWay(connectionMetaData[i]);
//	}
//	//tell membrane to disconnect from us
//	if (outerMembrane != nullptr)
//	{
//		for (Compound* c : innerSolution)
//		{
//			outerMembrane->innerSolution.emplace_back(c);
//		}
//		outerMembrane->DisconnectOneWay(outerMembraneConnectionMetadata);
//		if (structure.mass != 0)
//		{
//			Compound* c = new Compound();
//			*c = structure;
//			outerMembrane->innerSolution.emplace_back(c);
//		}
//	}
//	else
//	{	
//		Sector& s = Universe::getSectorAtLocation(xpos, ypos);
//		for (Compound* c : innerSolution)
//		{
//			s.AddCompoundToRandomLocationInSolution(c);
//		}
//		if (structure.mass != 0)
//		{
//			Compound* c = new Compound();
//			*c = structure;
//			s.AddCompoundToRandomLocationInSolution(c);
//		}
//	}
//}
//
////disconnect from the organelle corresponding to the metadata
//void Organelle::DisconnectOneWay(unsigned char Metadata)
//{
//	if (((char)Metadata) >= 0)
//	{
//		FastDelete(connections, Metadata);
//		FastDelete(connectionMetaData, Metadata);
//		if (connections.size() != Metadata)
//		{
//			//we just moved an entry, so if that entry tries to delete we will not be looking in the wrong place.
//			//we send them an updated piece of metadata so that they can find us and then change our delete metadata
//			connections[Metadata]->UpdateConnectionMetaData(connectionMetaData[Metadata], Metadata);
//		}
//	}
//	else
//	{
//		unsigned char negMetaData = ((char)Metadata) * -1 - 1;
//		FastDelete(innerOrganelles, negMetaData);
//		if (innerOrganelles.size() != negMetaData)
//		{
//			//we just moved an entry, so if that entry tries to get us to delete them we will be looking in the wrong place.
//			//we send them an updated piece of metadata so that they can find us on their end and then change our delete metadata
//			innerOrganelles[negMetaData]->outerMembraneConnectionMetadata = Metadata;
//		}
//	}
//}
//
//void Organelle::UpdateConnectionMetaData(unsigned char MetadataToFindEntry, unsigned char MetadataToChangeEntryTo)
//{
//	//I don't understand this if statement right now, its from membrane.
//	//im sure I will find out why it matters later lol
//	/*if ((char)MetadataToFindEntry >= 0)
//	{
//		Organelle::UpdateConnectionMetaData(MetadataToFindEntry, MetadataToChangeEntryTo);
//	}*/
//	connectionMetaData[MetadataToFindEntry] = MetadataToChangeEntryTo;
//}

void Organelle::MakePresenceKnown()
{
	Universe::addOrganelleToLocalPopulation(xpos, ypos, this);
	Universe::addOrganelleToLocalPopulation(xpos + Universe::sectorPixels, ypos, this);
	Universe::addOrganelleToLocalPopulation(xpos - Universe::sectorPixels, ypos, this);
	Universe::addOrganelleToLocalPopulation(xpos, ypos + Universe::sectorPixels, this);
	Universe::addOrganelleToLocalPopulation(xpos, ypos - Universe::sectorPixels, this);
	Universe::addOrganelleToLocalPopulation(xpos + Universe::sectorPixels, ypos + Universe::sectorPixels, this);
	Universe::addOrganelleToLocalPopulation(xpos - Universe::sectorPixels, ypos + Universe::sectorPixels, this);
	Universe::addOrganelleToLocalPopulation(xpos + Universe::sectorPixels, ypos - Universe::sectorPixels, this);
	Universe::addOrganelleToLocalPopulation(xpos - Universe::sectorPixels, ypos - Universe::sectorPixels, this);
}

void Organelle::SendRepositionRequests()
{
	bool outerMembraneExists = outerMembrane.vec.size() == 1;
	for (Organelle* o : innerOrganelles.vec)
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
	for (Organelle* o : connections.vec)
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
	if (!outerMembraneExists)
	{
		Sector& s = Universe::getSectorAtLocation(xpos, ypos);
		for (Organelle* o : s.localPopulation.vec)
		{
			if (o == this)
			{
				continue;
			}
			int xdelta = xpos - o->xpos;
			int ydelta = ypos - o->ypos;
			double distance = sqrt(xdelta * xdelta + ydelta * ydelta);
			int desiredDistance = o->size() * 2 + size() * 2 + 10;
			if (distance < desiredDistance)
			{
				double ratio = desiredDistance / (distance + 1);
				o->ReceiveRepositionRequest(xdelta - xdelta * ratio, ydelta - ydelta * ratio, desiredDistance - distance);
			}
		}
	}
	else
	{
		//reposition based on outermembrane
		int xdelta = xpos - outerMembrane.vec[0]->xpos;
		int ydelta = ypos - outerMembrane.vec[0]->ypos;
		int desiredDistance = outerMembrane.vec[0]->size() * 2 - size() * 2 - 3;
		if (abs(xdelta) + abs(ydelta) > desiredDistance)//if possible, do this much cheaper calulation and then skip
		{
			double distance = sqrt(xdelta * xdelta + ydelta * ydelta);
			if (distance > desiredDistance)
			{
				double ratio = desiredDistance / (distance + 1);
				outerMembrane.vec[0]->ReceiveRepositionRequest(xdelta - xdelta * ratio, ydelta - ydelta * ratio, 2);
			}
		}
		for (Organelle* o : outerMembrane.vec[0]->innerOrganelles.vec)
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
	xposRequest += deltax * force * 100;
	yposRequest += deltay * force * 100;
	totalRequestForce += force * 100;
}

void Organelle::Reposition()
{
	int xmove = xposRequest / (totalRequestForce + 1) + rand() % 3 - 1;//add a bit of random drift
	xpos += xmove; //change position 
	int ymove = yposRequest / (totalRequestForce + 1) + rand() % 3 - 1;
	ypos += ymove;
	xposRequest = xmove * 3;//continue to "coast" a little
	yposRequest = ymove * 3;
	totalRequestForce = 3;
	if (Universe::numxsectors * 320 < xpos || Universe::numysectors * 320 < ypos || xpos < 0 || ypos < 0)
	{
		SetPosition((xpos + Universe::numxsectors * 320) % (Universe::numxsectors * 320), (ypos + Universe::numysectors * 320) % (Universe::numysectors * 320));
	}
}

void Organelle::SetPosition(int x, int y)
{
	xpos = x + rand() % 200 - 100;
	ypos = y + rand() % 200 - 100;
}

void Organelle::Sense()
{
	//parent->Brain.SetNNInputForChar(metaData3, Organelle::AlivenessPercentageGuess());
	Brain.SetNNInputForChar(metaData3, Organelle::AlivenessPercentageGuess());
	Brain.SetNNInputForChar(1, (structure.internalEnergy / 10) - 9);
	Brain.SetNNInputForChar(36, connections.vec.size());
	//for each connected organelle
	for (Organelle* o : connections.vec)
	{
		//for each communication channel, of which there are 4
		for (int i = 0; i < NUM_COMMUNICATION_CHANNELS; i++)
		{
			//read some of their outputs into our inputs
			Brain.SetNNInputForChar(communication_inputs[i], o->Brain.GetNNOutputForChar(communication_outputs[i]));
		}
	}
}

void Organelle::Activate()
{
	if (structure.CalculateFitScore(&criticalRegion) > 3)
	{
		DoDeath();
		return;
	}
	if (structure.internalEnergy < 1)
	{
		//DoDeath();
		return;
	}
	else
	{
		structure.internalEnergy -= 1;
	}
	//energy += 5;
	
	Sense();
	Brain.RunNet();
	for (int i = 0; i < NUM_ACTIVATION_OPTIONS; i++)
	{		
		TakeAction(Brain.GetNNOutputForChar(activation_channels[i]), activation_locations[i]);
	}
	//if (structure.internalEnergy < 1)
	//{
	//	structure.internalEnergy = 0;
	//	DoDeath();
	//}
	return;
}

//the heart of the new system
//NNOutput is a char with some value equal to the output of our neural net
//location is a pre-defined location that we decided on when this organelle was created
//we use location to pick a spot on the DNA, which we take the modulus of to determine what the action will be
//we use the location, the decision, and the NNoutput to determine the behaviour of the organelle will be
void Organelle::TakeAction(int NNOutput, unsigned char location)
{
	char decision = geneticCode.dna[location];

	Chromosome newChrom;

	switch (decision % 5)
	{
	case 0:
		//eat
		//structure.internalEnergy += 1;
		break;
	case 1:
	{
		//reproduce?
		Compound target = ParseCompoundFromGeneticCode(&geneticCode, location + 1);
		int x = innerSolution.size();
		if (x > 0)
		{
			int reactionCompIdx = rand() % x;
			Compound* reactionComp = innerSolution[reactionCompIdx];
			int reactionCompInstability = 0;
			int productCompInstability = 0;
			bool didReaction = RipAwaySpecifiedElements(reactionComp, &productInProgress, &target, reactionCompInstability, productCompInstability, structure.internalEnergy);
			if (didReaction)
			{
				if (productInProgress.internalEnergy < 0)
				{
					structure.internalEnergy += productInProgress.internalEnergy;
					productInProgress.internalEnergy = 0;
				}
				if (reactionComp->sum == 0)
				{
					delete(reactionComp);
					FastDelete(innerSolution, reactionCompIdx);
				}
			}
			productInProgress.CalculateProduct();
			target.CalculateProduct();
			if (productInProgress.product == target.product)
			{
				CopyChromosome(&geneticCode, &newChrom);
				MutateChromosome(&newChrom);
				Organelle* o = CreateOrganelle(&newChrom, location + 1);
				o->structure = productInProgress;
				productInProgress = Compound();
				o->xpos = xpos + rand() % 800 - 400;
				o->ypos = ypos + rand() % 800 - 400;

				FastConnect(&connections, &(o->connections));

				Universe::newLife.emplace_back(o);
			}
		}

		//if (structure.internalEnergy > 10000)
		/*if (structure.internalEnergy > 25)
		{
			structure.internalEnergy -= 25;
			CopyChromosome(&geneticCode, &newChrom);
			MutateChromosome(&newChrom);
			Organelle* o = CreateOrganelle(&newChrom, location + 1);

			o->xpos = xpos + rand() % 800 - 400;
			o->ypos = ypos + rand() % 800 - 400;

			FastConnect(&connections,&(o->connections));

			Universe::newLife.emplace_back(o);
		}*/
		break;
	}
	case 2:
		//tbd
		break;
	case 3:
		//tbd
		break;
	default:
		//tbd
		break;
	}
}

int Organelle::size()
{
	return 15;
}

void Organelle::DoDiffusion()
{
	bool outerMembraneExists = outerMembrane.vec.size() == 1;
	if (!outerMembraneExists)
	{		
		Sector& s = Universe::getSectorAtLocation(xpos, ypos);
		if (s.ContainsReactant() || innerSolution.size() > 0)
		{
			int diffuseidx = rand() % (s.filledIdxs.size() + innerSolution.size());
			if (diffuseidx < s.filledIdxs.size())
			{
				innerSolution.emplace_back(s.RemoveCompoundByIdxInList(diffuseidx));
			}
			else
			{
				int inneridx = diffuseidx - s.filledIdxs.size();
				Compound* tosolution = innerSolution[inneridx];
				FastDelete(innerSolution, inneridx);
				s.AddCompoundToRandomLocationInSolution(tosolution);
			}
			
		}
	}
}

void Organelle::Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset)
{
	/*if (parent->center == this && false)
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
	}*/
	sf::CircleShape drawOrg((size() * 1.8) / zoom, 20);
	drawOrg.setFillColor(sf::Color(0, 0, 0, 0));
	drawOrg.setOutlineThickness(30.0 / zoom);
	drawOrg.setOutlineColor(structure.GetColor());
	drawOrg.setPosition(staticXOffset + (xpos - size()) / zoom, staticYOffset + (ypos - size()) / zoom);
	drawOrg.setOrigin(size() * 0.9 / zoom, size() * 0.9 / zoom);
	window.draw(drawOrg);
	for (Organelle* ooo : connections.vec)
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
	//ReactionSpace* solptr;
	//if (outerMembrane == nullptr)
	//{
	//	Sector& s = Universe::getSectorAtLocation(xpos, ypos);
	//	solptr = &s;
	//}
	//else
	//{
	//	solptr = outerMembrane;
	//}
	////if our solution has no reactant, and neither do we, then return.
	//if (!solptr->ContainsReactant() && !ContainsReactant())
	//{
	//	return;
	//}
	//int randIdx = solptr->GetReactantKey();
	//Compound * toReactWith = solptr->GetReactantWithKey(randIdx);
	//int stability = 0;
	//int ielem1;
	//int ielem2;
	//bool Comp1IsStructure = rand() % 2 == 1;
	//int idxidx1 = 0;
	//int idxidx2 = 0;
	//Compound* comp1;
	//Compound* comp2;

	//int idxidxStructure = rand() % structure.filledIndices.size();
	//int idxidxReactWith = rand() % toReactWith->filledIndices.size();

	//if (Comp1IsStructure)
	//{
	//	idxidx1 = idxidxStructure;
	//	idxidx2 = idxidxReactWith;
	//	comp1 = &structure;
	//	comp2 = toReactWith;
	//}
	//else
	//{
	//	idxidx1 = idxidxReactWith;
	//	idxidx2 = idxidxStructure;
	//	comp1 = toReactWith;
	//	comp2 = &structure;
	//}
	//
	//ielem1 = comp1->filledIndices[idxidx1];
	//ielem2 = comp2->filledIndices[idxidx2];
	//
	//int emptyIdxs[8];
	//int spaces = comp2->getUnPopulatedNeighborsIndices(ielem2, emptyIdxs);
	//
	//if (spaces == 0)	
	//{
	//	return;
	//}
	//ielem2 = emptyIdxs[rand() % spaces];
	//int x1, y1, x2, y2;
	//comp1->TranslateFromIdxToXY(ielem1, x1, y1);
	//comp2->TranslateFromIdxToXY(ielem2, x2, y2);
	//int threshold = rand() % 21 - 10 + rand() % 21 - 10 + rand() % 21 - 10 + rand() % 21 - 10 + rand() % 21 - 10 + rand() % 35 - 35;
	//
	//CallReactionAndCleanUp(comp1, comp2, stability, ielem1, idxidx1, ielem2, true, threshold, *solptr, reactants, Comp1IsStructure, randIdx, randIdx);




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
	//Element e = GetCriticalCharge();
	//if (e.red < 1)
	//{
	//	return false;
	//}
	////as red increases (and therefore so does mass and size), so does the tolerance for damage
	//int threshold = CriticalIdentity.red / 2;
	//if (abs(e.red - CriticalIdentity.red) > threshold)
	//{
	//	return false;
	//}
	//if (abs(e.blue - CriticalIdentity.blue) > threshold)
	//{
	//	return false;
	//}
	//if (abs(e.green - CriticalIdentity.green) > threshold)
	//{
	//	return false;
	//}
	return true;
}


//an estimation for how close this organelle is to dying, based on the same calculations as IsAlive()
int Organelle::AlivenessPercentageGuess()
{
	/*Element e = GetCriticalCharge();
	if (e.red < 1)
	{
		return 0;
	}
	int threshold = CriticalIdentity.red / 2;
	return threshold * 3 - abs(e.red - CriticalIdentity.red) - abs(e.blue - CriticalIdentity.blue) - abs(e.green - CriticalIdentity.green);*/
	return -1;
}

//Element Organelle::GetCriticalCharge()
//{
//	Element ret = Element();
//	int structsize = structure.composition.size();
//	if (structsize < criticalRegion*criticalRegion)
//	{
//		ret.red = -1;
//		return ret;
//	}
//	for (int i = (-1*((criticalRegion-1)/2)); i <= criticalRegion/2; i++)
//	{
//		for (int j = (-1 * ((criticalRegion - 1) / 2)); j <= criticalRegion / 2; j++)
//		{			
//			int idx = structure.TranslateFromXYToIdx(i, j);
//			if (idx > -1 && idx < structsize)
//			{
//				const Element& e = structure.composition[idx];
//				if (e.red != 0)
//				{
//					ret.red += e.red;
//					ret.blue += e.blue;
//					ret.green += e.green;
//				}
//			}			
//		}
//	}
//	return ret;
//}

//void Organelle::CheckConnectionDeath(std::list<Organelle*>& border)
//{
//	for (Organelle* o : connections)
//	{
//		if (o->UtilityMarker == 0)
//		{
//			o->UtilityMarker = 2;
//			border.push_back(o);
//		}
//		else if (o->UtilityMarker == 1)
//		{
//			int asdf = 1;
//		}
//	}
//	if (outerMembrane != nullptr && outerMembrane->UtilityMarker == 0)
//	{
//		outerMembrane->UtilityMarker = 2;
//		border.push_back(outerMembrane);
//	}
//	else if (outerMembrane != nullptr && outerMembrane->UtilityMarker == 1)
//	{
//		int asdf = 1;
//	}
//}

void Organelle::DoDeath()
{
	if (isded == 1)
	{
		return;
		//std::cout << "FUCK" << std::endl;
	}
	isded = 1;
	bool outerMembraneExists = outerMembrane.vec.size() == 1;
	while (innerOrganelles.vec.size() != 0)
	{
		Organelle* o = innerOrganelles.vec[0];
		FastDisconnect(&innerOrganelles, 0);
		if (outerMembraneExists)
		{
			FastConnect(&(o->outerMembrane), outerMembrane.cons[0]);
		}
	}
	//disconnect from all connected organelles
	for (int i = connections.vec.size(); i > 0; i--)
	{
		FastDisconnect(&connections, i-1);
	}

	//tell membrane to disconnect from us
	if (outerMembraneExists)
	{
		for (Compound* c : innerSolution)
		{
			outerMembrane.vec[0]->innerSolution.emplace_back(c);
		}
		if (structure.elementCount != 0)
		{
			Compound* c = new Compound();
			//hard copy the compound from the stack onto the heap
			*c = structure;
			outerMembrane.vec[0]->innerSolution.emplace_back(c);
		}
		else if (structure.internalEnergy > 0)
		{
			std::cout << "energy loss" << std::endl;
		}
		if (productInProgress.elementCount != 0)
		{
			Compound* c = new Compound();
			//hard copy the compound from the stack onto the heap
			*c = productInProgress;
			outerMembrane.vec[0]->innerSolution.emplace_back(c);
		}
		FastDisconnect(&outerMembrane, 0);
	}
	else
	{	
		Sector& s = Universe::getSectorAtLocation(xpos, ypos);
		for (Compound* c : innerSolution)
		{
			s.AddCompoundToRandomLocationInSolution(c);
		}
		if (structure.elementCount != 0)
		{
			Compound* c = new Compound();
			//hard copy the compound from the stack onto the heap
			*c = structure;
			s.AddCompoundToRandomLocationInSolution(c);
		}
		if (productInProgress.elementCount != 0)
		{
			Compound* c = new Compound();
			//hard copy the compound from the stack onto the heap
			*c = productInProgress;
			s.AddCompoundToRandomLocationInSolution(c);
		}

		//disconnect from all connected sectors
		for (int i = localArea.vec.size(); i > 0; i--)
		{
			FastDisconnect(&localArea, i - 1);
		}
	}
}

void Organelle::GetImmediateFamily(std::list<Organelle*>& retList)
{
	for (Organelle* o : connections.vec)
	{
		retList.push_back(o);
	}
}






bool Organelle::ContainsReactant()
{
	return innerSolution.size() != 0;
}

int Organelle::GetReactantKey()
{
	return rand() % innerSolution.size();
}

Compound* Organelle::GetReactantWithKey(int key)
{
	return innerSolution[key];
}

void Organelle::ResolveSituation(int key)
{
	/*if (key != -1)
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
	}*/
	if (innerSolution[key]->elementCount == 0)
	{
		delete innerSolution[key];
		FastDelete(innerSolution, key);
	}
}

void Organelle::AddCompoundToRandomLocationInSolution(Compound* c)
{
	innerSolution.emplace_back(c);
}







bool Organelle::CheckRep()
{
	if (structure.sum <= 0 || structure.elements[0] < 0 || structure.elements[1] < 0 || structure.elements[2] < 0 || structure.elements[3] < 0 || structure.internalEnergy < 0)
	{
		std::cout << "FUCK" << std::endl;
		return false;
	}
	else 
	{
		return true;
	}
}