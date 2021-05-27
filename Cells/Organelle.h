#pragma once
#include <vector>
#include "Compound.h"
#include "Sector.h"
#include "Universe.h"
#include "ReactionManager.h"
#include "Element.h"
#include "ReactionSpace.h"

class Organism;
class Membrane;

class Organelle
{
public:
	int xpos;
	int ypos;
	int xposRequest;
	int yposRequest;
	int totalRequestForce;
	Organism* parent;
	Membrane* outerMembrane;
	unsigned char outerMembraneConnectionMetadata;
	Compound structure;
	int criticalRegion;
	char UtilityMarker = 0;
	unsigned char isded = 0;
	unsigned char metaData1;
	unsigned char metaData2;
	unsigned char metaData3;
	unsigned char metaData4;
	virtual void ConnectTo(Organelle* o);
	virtual void SeverAllConnections();
	virtual void GetImmediateFamily(std::list<Organelle*>& retList);
	virtual void SendRepositionRequests();
	virtual void ReceiveRepositionRequest(int deltax, int deltay, int force);
	virtual void Reposition();
	virtual void Sense();
	virtual void Activate();
	virtual void Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset);
	virtual int size();
	virtual bool IsAlive();
	virtual int AlivenessPercentageGuess();
	virtual Element GetCriticalCharge();
	virtual void init(Compound struc, int crit, Organism* parentPtr);
	virtual int mass();

	virtual void DoChemistry(std::vector<Compound*>& reactants);
	virtual void CheckConnectionDeath(std::list<Organelle*>& border);

	//ideally, this would be protected, but im running into implementation issues so we'll do it later
	void UpdateConnectionMetaData(unsigned char MetadataToFindEntry, unsigned char MetadataToChangeEntryTo);
	std::vector<Organelle*> connections;
	virtual bool CheckRep();
	virtual ~Organelle() {/* stuff */ }
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


