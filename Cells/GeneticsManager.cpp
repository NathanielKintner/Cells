#include "GeneticsManager.h"

void RandomizeChromosome(Chromosome * chr)
{
	for (int i = 0; i < 256; i++)
	{
		chr->dna[i] = rand() % 255;
	}
}

void CopyChromosome(Chromosome* copyfrom, Chromosome* copyto)
{
	for (int i = 0; i < 256; i++)
	{
		copyto->dna[i] = copyfrom->dna[i];
	}
}

void MutateChromosome(Chromosome* chr)
{
	//start at somewhere between 3 and negative 3, repeat until you hit 0 (3, 2, 1, or 0 times)
	for (int i = rand() % 7 - 3; i > 0; i--)
	{
		chr->dna[rand() % 256] = rand() % 256;
	}
}

Organelle* CreateOrganelle(Chromosome* chr, unsigned char location)
{
	//pointer to new organelle
	Organelle* ret = new Organelle;

	int idx = 0;
	ret->metaData1 = chr->dna[(location + idx) % 256];
	idx++;
	ret->metaData2 = chr->dna[(location + idx) % 256];
	idx++;
	ret->metaData3 = chr->dna[(location + idx) % 256];
	idx++;
	ret->metaData4 = chr->dna[(location + idx) % 256];
	idx++;

	for (int i = 0; i < NUM_COMMUNICATION_CHANNELS; i++)
	{
		ret->communication_channels[i] = chr->dna[(location + idx) % 256];
		idx++;
	}

	for (int i = 0; i < NUM_ACTIVATION_OPTIONS; i++)
	{
		ret->activation_channels[i] = chr->dna[(location + idx) % 256];
		idx++;
	}

	for (int i = 0; i < NUM_ACTIVATION_OPTIONS; i++)
	{
		ret->activation_locations[i] = chr->dna[(location + idx) % 256];
		idx++;
	}

	for (int i = 0; i < NN_TOTAL_SIZE; i++)
	{
		ret->Brain.netLayers[i] = chr->dna[(location + idx) % 256];
		idx++;
	}



	//      structure  critical region
	//ret->init(orgComp, parts[partIdx + 1] % 3 + 2);
	ret->tempinit();
	ret->geneticCode = *chr;
	//init called last to allow the organelle to access its metadata before deciding how to initialize
	return ret;
}



class ConnectionGroup
{
public:
	unsigned char codeIdx = 0;
	std::vector<ConnectionGroup*> subgroups;
	std::vector<bool> presentThings;
	std::vector<std::pair<int,int>> internalConnections;
};

//250, 251, 252, 253, 254, 255 are reserved as markers
std::string CreateRandomCode(std::vector<std::string> universeComps)
{
	char buf[5000];
	int bufspot = 0;
	
	//compound layer
	for (int i = 0; i < 150; i++)
	{
		int idx = rand() % universeComps.size();
		for (char c : universeComps[idx])
		{
			buf[bufspot] = c;
			bufspot++;
		}
		buf[bufspot] = 254;
		bufspot++;
	}
	buf[bufspot] = 255;
	bufspot++;
	//parts layer
	for (int i = 0; i < 250 + 64; i++)
	{
		//ensure no \0 chars and no 250+ chars
		buf[bufspot] = 1 + rand() % 250;
		bufspot++;
	}

	buf[bufspot] = 255;
	bufspot++;
	//link layer
	for (int i = 0; i < 30; i++)
	{
		//ensure no \0 chars and no 250+ chars
		buf[bufspot] = 1 + rand() % 250;
		bufspot++;
	}

	buf[bufspot] = 255;
	bufspot++;
	//neural net layer
	for (int i = 0; i < (NN_TOTAL_SIZE); i++)
	{
		//ensure no \0 chars and no 250+ chars
		buf[bufspot] = 1 + rand() % 250;
		bufspot++;
	}
	buf[bufspot] = '\0';
	std::string ret(buf);
	return ret;
}


//Organism* ParseCode(std::string code)
//{
//	std::string partsString;
//	std::string linksString;
//	std::string neuralNetString;
//
//	std::vector<std::string> compStrings(0);
//
//	DisassembleCode(code, compStrings, partsString, linksString, neuralNetString);
//	return ReproduceFromCodeLayers(compStrings, partsString, linksString, neuralNetString);
//}

//Organism* ReproduceFromCodeLayers(std::vector<std::string>& compStrings, std::string& partsString, std::string& linksString, std::string& neuralNetString)
//{
//	//seems odd to literally start by mutating the code, but it comes in handy later so lets get it out of the way
//	Organism* ret = new Organism();
//	//deep copy
//	ret->parts = partsString;
//	ret->links = linksString;
//	ret->neuralNetString = neuralNetString;
//	for (std::string str : compStrings)
//	{
//		ret->compounds.emplace_back(str);
//	}
//	//aaand apply radioactive spider bites
//	MutateCodeLayers(ret->compounds, ret->parts, ret->links, ret->neuralNetString);
//	//the new organism's code is now eXtrA fuNkY
//
//	//create brain, using UN-MUTATED STRING
//	ret->Brain.LoadNet(neuralNetString.c_str());
//
//	std::vector<int> pendingLinks(0);
//	std::vector<int> linkBranches(0);
//
//
//	unsigned char* parts;
//	parts = (unsigned char*)partsString.c_str();
//	unsigned char* links;
//	links = (unsigned char*)linksString.c_str();
//
//	ConstructLinks(parts, links, linksString.size(), 0, pendingLinks, linkBranches);
//	Organelle* start = ParsePart(compStrings, parts, links, 0, ret);
//	ret->AllOrganelles.emplace_back(start);
//	size_t processedidx = 0;
//	size_t createdidx = 1;
//	ret->center = ret->AllOrganelles[0];
//	for (; processedidx < pendingLinks.size(); processedidx++)
//	{
//		ret->CheckRep();
//		int branchBehaviour = linkBranches[processedidx];
//		if (branchBehaviour < 0)
//		{
//			//loop back, remove the 10 we put there earlier
//			int target = ((branchBehaviour * -1) - 10 + processedidx) % ret->AllOrganelles.size();
//			if (target != processedidx)
//			{
//				ret->AllOrganelles[processedidx]->ConnectTo(ret->AllOrganelles[target]);
//				ret->CheckRep();
//				ret->CheckRep();
//			}
//		}
//		else if (branchBehaviour > 0)
//		{
//			//branch or at least continue
//			for (int i = 0; i < branchBehaviour; i++)
//			{
//				Organelle* newOrg = ParsePart(compStrings, parts, links, pendingLinks[createdidx], ret);
//				ret->AllOrganelles.emplace_back(newOrg);
//				ret->AllOrganelles[processedidx]->ConnectTo(ret->AllOrganelles[createdidx]);
//				ret->CheckRep();
//				createdidx++;
//			}
//		}
//		ret->CheckRep();
//	}
//
//	return ret;
//}

void DisassembleCode(std::string code, std::vector<std::string>& compStrings, std::string& partsString, std::string& linksString, std::string& neuralNetString)
{
	//get the compound section
	std::string compoundString;
	std::stringstream code_input(code);
	std::getline(code_input, compoundString, (char)255);

	//parse out compounds from the larger string
	std::string singleCompound;
	std::stringstream compound_input(compoundString);
	while (std::getline(compound_input, singleCompound, (char)254))
	{
		compStrings.emplace_back(singleCompound);
	}

	//get the parts section (unstructured bytes)
	std::getline(code_input, partsString, (char)255);

	//get the links section (unstructured bytes)
	std::getline(code_input, linksString, (char)255);

	//get the neural net section (unstructured bytes)
	std::getline(code_input, neuralNetString, (char)255);
}
void FindConnectedThings(int startIdx, int totalThings, std::vector<bool>& presentThings, std::vector<int>& borderList, std::vector<std::pair<int, int>>& connectionList, unsigned char parts[]);
void GetAllGroups(unsigned char parts[], ConnectionGroup* out, std::vector<std::string>& usedCompStrings, std::vector<std::string>& compStrings)
{
	unsigned char startCode = parts[out->codeIdx];
	int numThings = (startCode % 3 + startCode % 4) + 2;//maximum of 7, average of 4 or 5 ish, min of 2. adjustable later, shoudn't break anything	
	for (int i = 0; i < numThings; i++)
	{
		out->presentThings.emplace_back(false);
	}
	std::vector<int> borderList(0);
	FindConnectedThings(out->codeIdx, numThings, out->presentThings, borderList, out->internalConnections, parts);
	int offset = 0;
	for (bool b : out->presentThings)
	{
		offset++;
		if (b)
		{
			char subThingIdx = parts[out->codeIdx + offset];
			if (parts[subThingIdx] % 10 == 0)//1 in 10 chance
			{
				ConnectionGroup* subGroup = new ConnectionGroup();
				out->subgroups.emplace_back(subGroup);
				GetAllGroups(parts, subGroup, usedCompStrings, compStrings);
				continue;
			}
			else
			{
				usedCompStrings.emplace_back(compStrings[parts[subThingIdx]]);
			}
		}
		out->subgroups.emplace_back(nullptr);//nullptr if is empty OR if is organelle
	}
}

void FindConnectedThings(int startIdx, int totalThings, std::vector<bool>& presentThings, std::vector<int>& borderList, std::vector<std::pair<int,int>>& connectionList, unsigned char parts[])
{
	while (borderList.size() != 0)
	{
		int thingToCheckIdx = borderList[borderList.size()];
		borderList.pop_back();
		if (presentThings[thingToCheckIdx])
		{
			continue;
		}
		presentThings[thingToCheckIdx] = true;
		int thingIdx = parts[startIdx + thingToCheckIdx];
		for (int i = 0; i < totalThings; i++)
		{
			if (!presentThings[i] && parts[thingIdx + i] % 3 == 0)
			{
				connectionList.emplace_back(std::pair<int, int>(thingToCheckIdx, i));
				borderList.emplace_back(i);
			}
		}
	}
}

void ConstructLinks(unsigned char parts[], unsigned char links[], int numLinks, int idx, std::vector<int>& linkList, std::vector<int>& linkBranches)
{
	linkList.emplace_back(idx);//first part
	linkList.emplace_back(idx+1);//second part
	linkBranches.emplace_back(1);//first part is just connected to second part at the beginning
	for (size_t idx = 1; idx < linkList.size(); idx++)
	{
		//idx in links where this link is
		size_t currentlinkidx = linkList[idx];
		//the link itself, which points into parts
		unsigned char currentlink = links[currentlinkidx];
		//an indicator of whether we should add more parts or not
		unsigned char operation = ((parts[currentlink] % 8) + 3) / 5;
		int target = 0;
		switch (operation)
		{
		case(0):
			//loop back
			target = parts[currentlink + 1] % 4 - parts[currentlink + 2] % 4 + 10;
			linkBranches.emplace_back(target * -1);//mark this as needing to go backward this many idxs (remove the 10 later)
			break;
		case(1):
			//branch out
			target = (parts[currentlink + 1] % 2 + 1) * parts[currentlink + 1] % 4 + 1;
			linkBranches.emplace_back(0);
			for (int i = 0; i < target; i++)
			{
				currentlinkidx += (parts[(currentlink + 2 + 2 * i) % 250] % 8) * (parts[(currentlink + 3 + 2 * i) % 250] % 8);
				if (currentlinkidx >= numLinks)
				{
					break;
				}
				linkBranches[linkBranches.size() - 1] += 1; //mark this as needing to branch another time
				linkList.emplace_back(currentlinkidx + 1);
			}
			break;
		case(2):
			//dont branch or loop, just end
			linkBranches.emplace_back(0);
			break;
		}
	}
}

//Organelle* ParsePart(std::vector<std::string>& compoundStrings, unsigned char parts[], unsigned char links[], int idx, Organism * parentPtr)
//{
//	//pointer to new organelle
//	Organelle* ret;
//	//where this part's details are located
//	unsigned char partIdx = links[idx];
//	//this is the type of organelle
//	switch (parts[partIdx + 2] % 5)
//	{
//	case 0:
//		ret = new Factory;
//		break;
//	case 1:
//		ret = new Membrane;
//		break;
//	case 2:
//		ret = new Generator;
//		break;
//	case 3:
//		ret = new Muscle;
//		break;
//	default:
//		ret = new Organelle;
//		break;
//	}
//	
//	ret->metaData1 = parts[partIdx + 3];
//	ret->metaData2 = parts[partIdx + 4];
//	ret->metaData3 = parts[partIdx + 5];
//	ret->metaData4 = parts[partIdx + 6];
//
//	Compound orgComp = Compound(compoundStrings[parts[partIdx] % compoundStrings.size()]);
//
//	//      structure  critical region
//	ret->init(orgComp, parts[partIdx + 1] % 3 + 2);
//	//init called last to allow the organelle to access its metadata before deciding how to initialize
//	return ret;
//}

std::string ConcatCodeLayers(std::vector<std::string>& compStrings, std::string& partsString, std::string& linksString, std::string& neuralNetString)
{
	std::string ret = "";
	for (std::string c : compStrings)
	{
		ret += c;
		ret += (char)254;
	}
	return ret + (char)255 + partsString + (char)255 + linksString + (char)255 + neuralNetString;
}

std::string MutateCode(std::string code)
{
	std::vector<std::string> compStrings(0);
	std::string partsString;
	std::string linksString;
	std::string neuralNetString;
	DisassembleCode(code, compStrings, partsString, linksString, neuralNetString);
	//MutateCodeLayers(compStrings, partsString, linksString, neuralNetString);
	return ConcatCodeLayers(compStrings, partsString, linksString, neuralNetString);
}

//void MutateCodeLayers(std::vector<std::string>& compStrings, std::string & partsString, std::string& linksString, std::string& neuralNetString)
//{
//	for (int i = rand() % 6 - 3; i > 0; i--)
//	{
//		partsString[rand() % 256] = 1 + rand() % 250;
//		neuralNetString[rand() % (NN_TOTAL_SIZE)] = 1 + rand() % 250;
//		linksString[rand() % linksString.size()] = 1 + rand() % 250;
//	}
//	if (rand() % ((linksString.size()/2) * (linksString.size()/3)) == 0)
//	{
//		linksString = linksString + (char)(1 + rand() % 250);
//	}
//	int compidx1 = rand() % (compStrings.size()-1);
//	Compound comp1 = Compound(compStrings[compidx1]);
//	int ielem1 = comp1.filledIndices[rand() % comp1.filledIndices.size()];
//	FastDelete(compStrings, compidx1);
//	int compidx2 = rand() % (compStrings.size()-1);
//	Compound comp2 = Compound(compStrings[compidx2]);
//	int ielem2 = comp2.filledIndices[rand() % comp2.filledIndices.size()];
//	FastDelete(compStrings, compidx2);
//	int stability = 0;
//	PerformReactionIfGoodEnough(&comp1, &comp2, rand() % 20 - 10, stability, ielem1, ielem2);
//	compStrings.emplace_back(compStrings[compidx2]);
//	compStrings[compidx2] = comp2.ChemicalString();
//	if (comp1.mass != 0)
//	{
//		int numPieces = 0;
//		Compound* pieces = comp1.SplitCompound(ielem1, numPieces);
//		if (pieces == nullptr)
//		{
//			compStrings.emplace_back(compStrings[compidx1]);
//			compStrings[compidx1] = comp1.ChemicalString();
//		}
//		else
//		{
//			for (int i = 0; i < numPieces; i++)
//			{
//				compStrings.emplace_back(pieces[i].ChemicalString());
//			}
//			delete[] pieces;
//		}
//	}
//}
