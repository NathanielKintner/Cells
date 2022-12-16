#pragma once

class Organelle;

#include <vector>
#include "Compound.h"
#include "Sector.h"
#include "Universe.h"
#include "ReactionManager.h"
#include "Element.h"
#include "ReactionSpace.h"
#include "NeuralNet.h"
#include "Chromosome.h"
#include "GeneticsManager.h"


class Organelle : public ReactionSpace
{
public:
	Chromosome geneticCode;

	int xpos;
	int ypos;
	int xposRequest;
	int yposRequest;
	int totalRequestForce;

	int age = 0;

	int energy = 50;

	Organelle* outerMembrane;
	unsigned char outerMembraneConnectionMetadata;
	std::vector<Organelle*> innerOrganelles;
	std::vector<Compound*> innerSolution;
	Compound idealArrangement;

	NeuralNet Brain;
	#define NUM_COMMUNICATION_CHANNELS 4
	unsigned char communication_channels[NUM_COMMUNICATION_CHANNELS];
	#define NUM_ACTIVATION_OPTIONS 5
	unsigned char activation_channels[NUM_ACTIVATION_OPTIONS];
	unsigned char activation_locations[NUM_ACTIVATION_OPTIONS];

	Compound structure;
	std::list<Compound> complete;
	int criticalRegion;
	char UtilityMarker = 0;
	unsigned char isded = 0;
	unsigned char metaData1;
	unsigned char metaData2;
	unsigned char metaData3;
	unsigned char metaData4;
	void ConnectTo(Organelle* o);
	void SeverAllConnections();
	void GetImmediateFamily(std::list<Organelle*>& retList);
	void MakePresenceKnown();
	void SendRepositionRequests();
	void ReceiveRepositionRequest(int deltax, int deltay, int force);
	void Reposition();
	void Sense();
	void Activate();
	void TakeAction(int NNOutput, unsigned char location);
	void Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset);
	int size();
	bool IsAlive();
	int AlivenessPercentageGuess();
	//Element GetCriticalCharge();
	void init(Compound struc, int crit);

	void tempinit();
	int mass();

	void DoChemistry(std::vector<Compound*>& reactants);
	void CheckConnectionDeath(std::list<Organelle*>& border);


	//ReactionSpace Interface
	bool ContainsReactant();
	int GetReactantKey();
	Compound* GetReactantWithKey(int key);
	void ResolveSituation(int key);
	void AddCompoundToRandomLocationInSolution(Compound* c);

	//ideally, this would be protected, but im running into implementation issues so we'll do it later
	void UpdateConnectionMetaData(unsigned char MetadataToFindEntry, unsigned char MetadataToChangeEntryTo);
	std::vector<Organelle*> connections;
	bool CheckRep();
	~Organelle() {/* stuff */ }
protected:
	/// <summary>
	/// connect to o and exchange connection metadata
	/// </summary>
	/// <param name="o"></param>
	/// the organelle to connect to
	/// <param name="Metadata"></param>
	/// the metadata this organelle should send o when it wants to disconnect
	/// <returns></returns>
	/// an error code or 0, do not connect if ret != 0
	virtual char ConnectOneWay(Organelle* o, unsigned char& Metadata);	
	virtual void DisconnectOneWay(unsigned char Metadata);
	std::vector<unsigned char> connectionMetaData;
	Element CriticalIdentity;
};


