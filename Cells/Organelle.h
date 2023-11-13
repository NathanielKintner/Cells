#pragma once

class Organelle;
class Membrane;

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
#include "Membrane.h"


class Organelle// : public ReactionSpace
{
public:
	//Chromosome geneticCode;

	int xpos;
	int ypos;
	int xposRequest;
	int yposRequest;
	int totalRequestForce;

	int age = 0;

	Organelle* right;
	Organelle* left;


	ConnectionNode<Organelle*, Sector*> localArea;

	ConnectionNode<Organelle*, Membrane*> outerMembrane;

	Compound idealArrangement;

	NeuralNet Brain;
	#define NUM_COMMUNICATION_CHANNELS 4
	unsigned char communication_inputs[NUM_COMMUNICATION_CHANNELS];
	unsigned char communication_outputs[NUM_COMMUNICATION_CHANNELS];
	#define NUM_ACTIVATION_OPTIONS 5
	unsigned char activation_channels[NUM_ACTIVATION_OPTIONS];
	unsigned char activation_locations[NUM_ACTIVATION_OPTIONS];

	Compound structure;
	Compound criticalRegion;
	Compound membraneMask;
	Compound productInProgress = Compound();
	//std::list<Compound> complete;
	char UtilityMarker = 0;
	unsigned char isded = 0;
	unsigned char metaData1;
	unsigned char metaData2;
	unsigned char metaData3;
	unsigned char metaData4;
	//void ConnectTo(Organelle* o);
	//void SeverAllConnections();
	void GetImmediateFamily(std::list<Organelle*>& retList);
	void MakePresenceKnown();
	virtual void SendRepositionRequests();
	void ReceiveRepositionRequest(int deltax, int deltay, int force);
	void Reposition();
	void Sense();
	void Activate();
	virtual void TakeAction(int NNOutput, unsigned char location);
	void Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset);
	virtual int size();
	bool IsAlive();
	int AlivenessPercentageGuess();
	virtual void DoDeath();
	virtual void DoDiffusion();
	void SetPosition(int x, int y);
	void RotateLeft();
	void RotateRight();

	//Element GetCriticalCharge();
	//void init(Compound struc, int crit);
	virtual void init(Nucleotides* n);

	//void tempinit();
	int mass();

	void DoChemistry(std::vector<Compound*>& reactants);
	//void CheckConnectionDeath(std::list<Organelle*>& border);

	ReactionSpace* GetReactionSpace();

	//ideally, this would be protected, but im running into implementation issues so we'll do it later
	//void UpdateConnectionMetaData(unsigned char MetadataToFindEntry, unsigned char MetadataToChangeEntryTo);
	//std::vector<Organelle*> connections;
	ConnectionNode<Organelle*,Organelle*> connections;
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
	//virtual char ConnectOneWay(Organelle* o, unsigned char& Metadata);	
	//virtual void DisconnectOneWay(unsigned char Metadata);
	std::vector<unsigned char> connectionMetaData;
	Element CriticalIdentity;
};


