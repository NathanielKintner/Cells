#pragma once

#include "Organelle.h"
#include "Synthase.h"
#include "Nucleotides.h"



class Chromosome : public Organelle
{
public:

	Nucleotides sequence;

	ConnectionNode<Chromosome*, Synthase*> synthaseReadingThis;

	void init(Nucleotides* n);
	void DoDeath();


	~Chromosome() {/* stuff */ }

};
