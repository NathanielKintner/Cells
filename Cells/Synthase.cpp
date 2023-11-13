#include "Synthase.h"


void Synthase::DoDeath()
{
	bool attachedToChromosome = chromosomeBeingRead.vec.size() == 1;
	//disconnect from the chromosome we are reading, if any
	if (attachedToChromosome)
	{
		FastDisconnect(&chromosomeBeingRead, 0);
	}

	bool outerMembraneExists = outerMembrane.vec.size() == 1;

	//dump productinprogress
	if (outerMembraneExists)
	{
		if (productInProgress.elementCount != 0)
		{
			Compound* c = new Compound();
			//hard copy the compound from the stack onto the heap
			*c = productInProgress;
			outerMembrane.vec[0]->innerSolution.emplace_back(c);
		}
	}
	else
	{
		Sector& s = Universe::getSectorAtLocation(xpos, ypos);
		if (productInProgress.elementCount != 0)
		{
			Compound* c = new Compound();
			//hard copy the compound from the stack onto the heap
			*c = productInProgress;
			s.AddCompoundToRandomLocationInSolution(c);
		}
	}
	Organelle::DoDeath();
}

void Synthase::TakeAction(int NNOutput, unsigned char location)
{
	//char decision = geneticCode.dna[location]; //this is what we were doing just post-return
	char decision = location; //this is some random bullshit we are doing right now. Not well-reasoned.

	Nucleotides newNuc;

	switch (decision % 5)
	{
	case 0:
		//eat
		//structure.internalEnergy += 1;
		break;
	case 1:
	{
		//reproduce?
		bool attachedToChromosome = chromosomeBeingRead.vec.size() == 1;
		Nucleotides* geneticCode = &(chromosomeBeingRead.vec[0]->sequence);


		Compound target = ParseCompoundFromGeneticCode(geneticCode, location + 1);

		ReactionSpace* rs = GetReactionSpace();

		if (rs->ContainsReactant())
		{
			int reactionCompIdx = rs->GetReactantKey();
			Compound* reactionComp = rs->GetReactantWithKey(reactionCompIdx);
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
			}
			productInProgress.CalculateProduct();
			target.CalculateProduct();
			if (productInProgress.product == target.product)
			{
				CopyChromosome(geneticCode, &newNuc);
				MutateChromosome(&newNuc);
				Organelle* o = CreateOrganelle(&newNuc, location + 1);
				o->structure = productInProgress;
				//delete the progress; make a new empty compound
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