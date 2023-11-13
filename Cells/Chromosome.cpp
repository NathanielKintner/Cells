#include "Chromosome.h"


#include "Synthase.h"

void Chromosome::init(Nucleotides* n)
{
	sequence = *n;
	Organelle::init(n);
}

void Chromosome::DoDeath()
{
	//disconnect from the synthases that are reading us
	while (synthaseReadingThis.vec.size() != 0)
	{
		FastDisconnect(&synthaseReadingThis, 0);
	}

	Organelle::DoDeath();
}