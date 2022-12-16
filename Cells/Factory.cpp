//#include "Factory.h"
//
//void Factory::init(Compound struc, int crit, Organism* parentPtr)
//{
//	Organelle::init(struc, crit, parentPtr);
//
//	std::vector<int> pendingLinks(0);
//	std::vector<int> linkBranches(0);
//	unsigned char* parts = (unsigned char*)parent->parts.c_str();
//	unsigned char* links = (unsigned char*)parent->links.c_str();
//
//	ConstructLinks(parts, links, parent->links.size(), 0, pendingLinks, linkBranches);
//	for (int idx : pendingLinks)
//	{
//		//where this part's details are located
//		unsigned char partIdx = links[idx];
//		toDo.push_back(parent->compounds[parts[partIdx] % parent->compounds.size()]);
//	}
//	progress = Compound();
//	current = Compound(toDo[0]);
//	
//	progress.AddElementAtIndex(Element(), current.minX + 1, current.minY + 1);
//	if(current.minX+1 != current.maxX-1 || current.minY + 1 != current.maxY -1)
//		progress.AddElementAtIndex(Element(), current.maxX - 1, current.maxY - 1);
//	
//	progress.RemoveElementAtIndex(current.minX + 1, current.minY + 1);
//	if (current.minX + 1 != current.maxX - 1 || current.minY + 1 != current.maxY - 1)
//		progress.RemoveElementAtIndex(current.maxX - 1, current.maxY - 1);
//	
//	progress.filledIndices.clear();
//	openborder.emplace_back(current.filledIndices[0]);
//	AdvanceGoalCursor();
//}
//
//int Factory::mass()
//{
//	int mass = progress.mass;
//	for (Compound c : complete)
//	{
//		mass += c.mass;
//	}
//	return Organelle::mass() + mass;
//}
//
//void Factory::SeverAllConnections()
//{
//	ReactionSpace* rs;
//	if (outerMembrane != nullptr)
//	{
//		rs = outerMembrane;
//	}
//	else
//	{
//		rs = &Universe::getSectorAtLocation(xpos, ypos);
//	}
//
//	if (progress.mass != 0)
//	{
//		for (Element& e : progress.composition)
//		{
//			if (e.red == -1)
//			{
//				e.red = 0;
//			}
//		}
//		Compound* progComp = new Compound();
//		*progComp = progress;
//		rs->AddCompoundToRandomLocationInSolution(progComp);
//
//	}
//	for (Compound c : complete)
//	{
//		Compound* compComp = new Compound();
//		*compComp = c;
//		rs->AddCompoundToRandomLocationInSolution(compComp);
//	}
//	Organelle::SeverAllConnections();
//}
//
//void Factory::Sense()
//{
//	parent->Brain.SetNNInputForChar(metaData3, (10*toDo.size()/(complete.size()+1)) - 5);
//}
//
//void Factory::Activate()
//{
//	if (parent->energy < 0)
//	{
//		return;
//	}
//	Sector& s = Universe::getSectorAtLocation(xpos, ypos);
//	if (!s.ContainsReactant())
//	{
//		return;
//	}
//	//there is a degree of redundancy here with the reactionmanager functions. However, nowhere else
//	//will we want to force a reaction and then simply report on how costly it was.(this may change later, tis
//	//easy enough to refactor)
//	int chosenComp = s.GetReactantKey();
//	Compound * compEnv = s.GetReactantWithKey(chosenComp);
//	for (int idxidx1 = 0; idxidx1 < compEnv->filledIndices.size(); idxidx1++)
//	{
//		int ielem1 = compEnv->filledIndices[idxidx1];
//		Element e = compEnv->composition[ielem1];
//		if (e.red == 0)
//		{
//			continue;
//		}
//		if (e.name == current.composition[openborder[0]].name)
//		{
//			int stability = 0;
//			std::vector<Compound*> reactants(0);
//			CallReactionAndCleanUp(compEnv, &progress, stability, ielem1, idxidx1, openborder[0], false, 0, s, reactants, false, chosenComp, chosenComp);
//			/*
//			int xenv, yenv, xprog, yprog;
//			compEnv.TranslateFromIdxToXY(idxidx, xenv, yenv);
//			
//			//progress.AddElementAtIndex(Element(), xprog, yprog);//expand progress to right place
//			//progress.RemoveElementAtIndex(xprog, yprog);//expand progress to right place
//			//FastDelete(progress.filledIndices, progress.filledIndices.size()-1);
//			int stability = compEnv.GetStabilityAtPoint(xenv, yenv) + progress.GetStabilityAtPoint(xprog, yprog);
//			DoReaction(compEnv, progress, xenv, yenv, xprog, yprog, i);
//			stability = compEnv.GetStabilityAtPoint(xenv, yenv) + progress.GetStabilityAtPoint(xprog, yprog) - stability;
//
//			std::vector<Compound*> reactants;
//			reactants.emplace_back(&progress);
//
//			if (compEnv.mass == 0)
//			{
//				FastDelete(s.sol, chosenComp);
//			}
//			else
//			{
//				//manage breaking up the compound from the environment
//				int numPieces = 0;
//				Compound* pieces = compEnv.SplitCompound(idxidx, numPieces);
//				if (pieces == nullptr)
//				{
//					reactants.emplace_back(&compEnv);
//				}
//				else
//				{
//					for (int j = 0; j < numPieces; j++)
//					{
//						s.sol.emplace_back(pieces[j]);
//					}
//					FastDelete(s.sol, chosenComp);
//					int target = s.sol.size() - 1;
//					for (int j = 0; j < numPieces; j++)
//					{
//						reactants.emplace_back(&s.sol[target - j]);
//					}
//					delete[] pieces;
//				}
//			}*/
//			//drain energy if this was a disadvantageous reaction
//			//AdjustEnergyValues(stability, reactants);
//			if (progress.internalEnergy < 0)
//			{
//				parent->energy += progress.internalEnergy;
//				progress.internalEnergy = 0;
//			}
//			int xprog, yprog;
//			current.TranslateFromIdxToXY(openborder[0], xprog, yprog);
//			std::vector<int> neighbors(0);
//			current.GetPopulatedNeighbors(openborder[0], neighbors);			
//			for (int n : neighbors)
//			{
//				current.TranslateFromIdxToXY(n, xprog, yprog);
//				if (progress.composition[progress.TranslateFromXYToIdx(xprog, yprog)].red == 0)
//				{
//					progress.composition[progress.TranslateFromXYToIdx(xprog, yprog)].red = -1;
//					openborder.emplace_back(n);
//					if (openborder.size() + progress.filledIndices.size() > current.filledIndices.size() + 1)
//					{
//						int asdf = 0;
//					}
//				}
//			}
//			FastDelete(openborder, 0);
//			AdvanceGoalCursor();
//			break;
//		}
//	}
//}
//
//void Factory::AdvanceGoalCursor()
//{
//	if (openborder.size() == 0)
//	{
//		complete.push_back(progress);
//		progress = Compound();
//		if (complete.size() == toDo.size())
//		{
//			int energyGiven = 0;
//			for (Compound c : complete)
//			{
//				energyGiven += c.internalEnergy;
//			}
//			complete.clear();
//			//Organism* newOrg = ReproduceFromCodeLayers(parent->compounds, parent->parts, parent->links, parent->neuralNetString);
//			//newOrg->SetPosition(xpos + rand() % 300 - 150, ypos + rand() % 300 - 150);
//			//Universe::newLife.emplace_back(newOrg);
//			//parent->energy -= (50 - energyGiven);//can potentially generate energy for parent if the factory is really efficient
//		}
//		current = Compound(toDo[complete.size()]);
//
//		progress.AddElementAtIndex(Element(), current.minX + 1, current.minY + 1);
//		if (current.minX + 1 != current.maxX - 1 || current.minY + 1 != current.maxY - 1)
//			progress.AddElementAtIndex(Element(), current.maxX - 1, current.maxY - 1);
//
//		progress.RemoveElementAtIndex(current.minX + 1, current.minY + 1);
//		if (current.minX + 1 != current.maxX - 1 || current.minY + 1 != current.maxY - 1)
//			progress.RemoveElementAtIndex(current.maxX - 1, current.maxY - 1);
//
//		progress.filledIndices.clear();
//		openborder.emplace_back(current.filledIndices[0]);
//	}
//}