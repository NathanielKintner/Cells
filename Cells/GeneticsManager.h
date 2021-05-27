#pragma once

#include <string>
#include <sstream>
#include "Organism.h"
#include <vector>
#include "Compound.h"
#include "Universe.h"
#include "Organelle.h"
#include "Factory.h"
#include "Membrane.h"
#include "Generator.h"
#include "Muscle.h"
#include "ReactionManager.h"
#include "NeuralNet.h"

Organelle* ParsePart(std::vector<std::string>& compoundStrings, unsigned char parts[], unsigned char links[], int idx, Organism* parentPtr);
void DisassembleCode(std::string code, std::vector<std::string>& compStrings, std::string& partsString, std::string& linksString, std::string& neuralNetString);
void ConstructLinks(unsigned char parts[], unsigned char links[], int numLinks, int idx, std::vector<int>& linkList, std::vector<int>& linkBranches);
std::string CreateRandomCode(std::vector<std::string> universeComps);
Organism* ReproduceFromCodeLayers(std::vector<std::string>& compStrings, std::string& partsString, std::string& linksString, std::string& neuralNetString);
Organism* ParseCode(std::string code);
std::string ConcatCodeLayers(std::vector<std::string>& compStrings, std::string& partsString, std::string& linksString);
std::string MutateCode(std::string code);
void MutateCodeLayers(std::vector<std::string>& compStrings, std::string& partsString, std::string& linksString, std::string& neuralNetString);