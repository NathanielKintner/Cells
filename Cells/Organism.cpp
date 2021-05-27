#include "Organism.h"

void Organism::Reposition()
{
	for (Organelle* o : AllOrganelles)
	{
		o->SendRepositionRequests();
		int jostle = (o->totalRequestForce + 1) * 2;
		o->ReceiveRepositionRequest(rand() % jostle*2 - jostle + 1, rand() % jostle *2 - jostle + 1, 1);
	}
	for (Organelle* o : AllOrganelles)
	{
		o->Reposition();
	}
	int x = 0;
	int y = 0;
	for (Organelle* o : AllOrganelles)
	{
		x += o->xpos;
		y += o->ypos;
	}
	x = x / (int)AllOrganelles.size();
	y = y / (int)AllOrganelles.size();
	if (Universe::numxsectors * 320 < x || Universe::numysectors * 320 < y || x < 0 || y < 0)
	{
		SetPosition((x + Universe::numxsectors * 320) % (Universe::numxsectors * 320), (y + Universe::numysectors * 320) % (Universe::numysectors * 320));
	}
}
void Organism::DoChemistry(std::vector<Compound*>& reactants)
{
	for (Organelle* o : AllOrganelles)
	{
		o->DoChemistry(reactants);
	}
}

void Organism::DoDeaths()
{
	if (age == 2000 || energy < -500)
	{
		center->structure.internalEnergy += energy;
		center = nullptr;//center is dead, kill the whole organism in a later method
		return;
	}
	bool doDeath = false;
	for (Organelle* o : AllOrganelles)
	{
		if (!o->IsAlive())
		{
			doDeath = true;
			o->UtilityMarker = 1;			
		}
	}
	if (doDeath)//at least one organelle has failed
	{
		//cut out all dead organelles
		for (Organelle* o : AllOrganelles)
		{
			if (o->UtilityMarker == 1)
			{
				o->SeverAllConnections();				
			}
		}
		//now that we've cut our losses, we need to determine if any organelles are stranded and
		//therefore must also be cut
		if (center->UtilityMarker == 1)
		{
			center->structure.internalEnergy += energy;
			center = nullptr;//center is dead, kill the whole organism in a later method
			return;
		}
		std::list<Organelle*> border;
		center->UtilityMarker = 2;
		border.push_back(center);
		while (border.size() != 0)
		{
			Organelle* o = border.front();
			border.pop_front();
			//will add to border if there are more connections to check
			o->CheckConnectionDeath(border);
		}
		//now, all living and connected organelles are marked with a 2
		for (int i = 0; i < AllOrganelles.size();i++)
		{
			if (AllOrganelles[i]->UtilityMarker != 2 and AllOrganelles[i]->UtilityMarker != 1)
			{
				//sever and dump guts
				AllOrganelles[i]->SeverAllConnections();
			}
		}
		for (int i = 0; i < AllOrganelles.size();)
		{
			if (AllOrganelles[i]->UtilityMarker != 2)
			{
				//free memory and delete from the the list				
				delete AllOrganelles[i];
				FastDelete(AllOrganelles, i);
			}
			else
			{
				++i;
			}
		}
		for (Organelle* o : AllOrganelles)
		{
			o->UtilityMarker = 0;
		}
	}
}

void Organism::DoActivations()
{
	energy -= 1;
	//energy += 5;
	Brain.SetNNInputForChar(1, (energy/10)-9);
	Brain.SetNNInputForChar(36, AllOrganelles.size());
	for (Organelle* o : AllOrganelles)
	{
		o->Sense();
	}
	Brain.RunNet();
	for (Organelle* o : AllOrganelles)
	{
		o->Activate();
	}
}

void Organism::SetPosition(int xpos, int ypos)
{
	for (Organelle* o : AllOrganelles)
	{
		o->xpos = xpos + rand() % 200 - 100;
		o->ypos = ypos + rand() % 200 - 100;
	}
}

void Organism::TranslatePosition(int xpos, int ypos)
{
	for (Organelle* o : AllOrganelles)
	{
		o->xpos += xpos;
		o->ypos += ypos;
	}
}

bool Organism::CheckRep()
{
	for (Organelle* o : AllOrganelles)
	{
		if (o->outerMembrane != nullptr)
		{
			if (o->outerMembrane->innerOrganelles[(char)(o->outerMembraneConnectionMetadata * -1) - 1] != o)
			{
				return false;
			}
			if (o->outerMembrane == o)
			{
				return false;
			}
		}
		if (!o->CheckRep())
		{
			return false;
		}
		for (Organelle* oo : o->connections)
		{
			if (oo == o)
			{
				return false;
			}
			if (oo == o->outerMembrane)
			{
				return false;
			}
			bool connectback = false;
			for (Organelle* ooo : oo->connections)
			{
				if (ooo == o)
				{
					connectback = true;
				}
			}
			if (!connectback)
			{
				return false;
			}
		}
	}
	return true;
}

