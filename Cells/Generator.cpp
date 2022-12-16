//#include "Generator.h"
//
//
//void Generator::Activate()
//{
//	Sector& s = Universe::getSectorAtLocation(xpos, ypos);
//	if (!s.ContainsReactant())
//	{
//		return;
//	}
//	int chosenComp = s.GetReactantKey();
//	Compound* compEnv = s.GetReactantWithKey(chosenComp);
//	Element elemEnv = compEnv->GetColorCharges();
//	Element elemGen = GetCriticalCharge();
//	int bluediff = abs(elemEnv.blue - elemGen.blue);
//	int greendiff = abs(elemEnv.green - elemGen.green);
//	if (bluediff < 3 && greendiff < 3)
//	{
//		int efficiency = 1 + elemGen.red - (greendiff + bluediff);
//		if (efficiency < 1)
//		{
//			return;
//		}
//		int energyHarvested = (compEnv->internalEnergy * efficiency) / (efficiency + 2);
//		compEnv->internalEnergy -= energyHarvested;
//		parent->energy += energyHarvested;
//	}
//}
//
//
//
//
