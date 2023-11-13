#pragma once
#include "Organelle.h"
//#include "Organism.h"
#include "GeneticsManager.h"
#include "ReactionManager.h"
#include "ReactionSpace.h"
#include <vector>
#include <list>

class Organelle;

class Membrane : public Organelle, public ReactionSpace
{
public:

	ConnectionNode<Membrane*, Organelle*> innerOrganelles;

	std::vector<Compound*> innerSolution;

	Organelle associationRingNodeOne;
	Organelle associationRingNodeTwo;

	void AddOrganelleToRing(Organelle* o);

	//ReactionSpace Interface
	bool ContainsReactant();
	int GetReactantKey();
	Compound* GetReactantWithKey(int key);
	void ResolveSituation(int key);
	void AddCompoundToRandomLocationInSolution(Compound* c);

	//inherited method adjustments
	void init(Nucleotides* n);
	void SendRepositionRequests();
	void DoDeath();
	int size();
	void DoDiffusion();

	//std::list<Compound> complete;
	//std::vector<Organelle*> innerOrganelles;
	//std::vector<Compound*> innerSolution;
	//Compound idealArrangement;

	//void Activate();
	//void Sense();
	//int size();
	//void init(Compound struc, int crit);
	//virtual int mass();
	//virtual void SeverAllConnections();
	//void ConnectTo(Organelle* o);
	//char ConnectOneWay(Organelle* o, unsigned char& Metadata);
	//void DisconnectOneWay(unsigned char Metadata);
	//void UpdateConnectionMetaData(unsigned char MetadataToFindEntry, unsigned char MetadataToChangeEntryTo);
	//void SendRepositionRequests();
	//void CheckConnectionDeath(std::list<Organelle*>& border);
	//bool CheckRep();
	//void Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset);

	////ReactionSpace Interface
	//bool ContainsReactant();
	//int GetReactantKey();
	//Compound* GetReactantWithKey(int key);
	//void ResolveSituation(Compound pieces[], int numpieces, int key);
	//void AddCompoundToRandomLocationInSolution(Compound* c);

	virtual ~Membrane() {/* stuff */ }

protected:
private:
	//bool firstConnectionMade;
};