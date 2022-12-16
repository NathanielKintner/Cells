//#pragma once
//
//#include <vector>
//#include "Compound.h"
//#include "Organelle.h"
//#include "Membrane.h"
//#include "NeuralNet.h"
//
//class Organism
//{
//public:
//	std::vector<Organelle*> AllOrganelles;
//	NeuralNet Brain;
//	Organelle* center;
//	std::vector<std::string> compounds;
//	std::string links;
//	std::string parts;
//	std::string neuralNetString;
//
//
//	int age = 0;
//	int energy = 50;
//	bool hasBeenPlaced = 0;
//
//	void SetPosition(int xpos, int ypos);
//	void TranslatePosition(int xpos, int ypos);
//
//	void Reposition();
//	void DoChemistry(std::vector<Compound*>& reactants);
//	void DoDeaths();
//	void DoActivations();
//	bool CheckRep();
//
//	~Organism()
//	{
//		for (Organelle* o : AllOrganelles)
//		{
//			delete o;
//		}
//	}
//};
//
