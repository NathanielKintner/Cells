// Cells.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Cells.h"
int runSim();

int main()
{
    try
    {
        return runSim();
    }
    catch (std::exception msg)
    {
        std::cerr << msg.what() << std::endl;
    }
}

int runSim()
{
    //srand(46);
    //srand(42);
    srand(40);
    //srand(time(NULL));
    //Grids::InitGrids(65, 65);
    //Grids::InitGrids(25, 25);
    Universe::init(25, 25);
    
    Element e;
    e.red = 1;
    e.blue = 1;
    e.green = 1;
    e.name = 'a';
    PeriodicTable::ptable.emplace_back(e);
    e.red = 2;
    e.blue = 2;
    e.green = 2;
    e.name = 'b';
    PeriodicTable::ptable.emplace_back(e);
    e.red = 2;
    e.blue = -2;
    e.green = 2;
    e.name = 'c';
    PeriodicTable::ptable.emplace_back(e);
    e.red = 2;
    e.blue = -1;
    e.green = 3;
    e.name = 'd';
    PeriodicTable::ptable.emplace_back(e);
    e.red = 2;
    e.blue = 1;
    e.green = 3;
    e.name = 'e';
    PeriodicTable::ptable.emplace_back(e);
    for (int i = 1; i <= 4; i++)
    {
        e.red = rand() % 3 + 1;
        e.blue = rand() % 8 - 4;
        e.green = rand() % 6;
        e.name = 'e' + i;
        PeriodicTable::ptable.emplace_back(e);
    }

    for (Element e : PeriodicTable::ptable)
    {
        PeriodicTable::ptableHash[e.name] = e;
    }

    Organelle env;

    for (int i = 0; i < 100; i++)
    {
        Compound * c = new Compound();
        for (int j = 0; j < 4; j++)
        {
            //c->AddElementAtIndex(PeriodicTable::ptable[rand() % PeriodicTable::ptable.size()], j, 0);
            c->elements[0] = rand() % 5;
            c->elements[1] = rand() % 5;
            c->elements[2] = rand() % 5;
            c->elements[3] = rand() % 5;
        }
        c->CalculateSum();
        if (c->sum == 0)
        {
            i--;
        }
        else
        {
            c->id = i;
            env.innerSolution.emplace_back(c);
        }
    }
    int stability = 0;

    for (Compound * c : env.innerSolution)
    {
        c->CalculateSum();
        stability += c->GetTotalInstability();
    }
    std::cout << stability << std::endl;


    std::clock_t start;
    double duration;

    start = std::clock();


    //for (int i = 0; i < 20000000; i++) //6.661 secs benchmark, debugging seems to take 140X longer or so (oof) //more recently, 9.928
    for (int i = 0; i < 20000; i++)
    {
        //std::cout << i << "\n";
        std::vector<Compound*> asdf;
        //AttemptReaction(env, 10, asdf);
        //insist that every move get at least slightly better
        //RandomlyReactInSolution(env, -1);
        /*if (i == 34539)
        {
            RandomlyReactInSolution(env, rand() % 20 - 25);
        }*/
        RandomlyReactInSolution(env, rand() % 20 - 25);
        
        //int bigsum = 0;
        //int bigcount = 0;
        //int bigenergy = 0;
        //int biginstability = 0;
        //for (Compound* c : env.innerSolution)
        //{
        //    //c->ReCenter();
        //    c->CalculateSum();
        //    //bigsum += c->sum;
        //    //bigcount += 1;
        //    //bigenergy += c->internalEnergy;q
        //    //biginstability += c->GetTotalInstability();
        //    if (c->sum <= 0 || c->elements[0] < 0 || c->elements[1] < 0 || c->elements[2] < 0 || c->elements[3] < 0 || c->internalEnergy < 0)
        //    {
        //        std::cout << i << "FUCK" << std::endl;
        //    }
        //}
        //std::cout << i << std::endl;
        //std::cout << "Element sum: " << bigsum << std::endl;
        //std::cout << "Count of compounds: " << bigcount << std::endl;
        //std::cout << "Total internal energy: " << bigenergy << std::endl;
        //std::cout << "Total instability: " << biginstability << std::endl;
        //std::cout << "Thermodynamic sum: " << bigenergy + biginstability << std::endl;

    }

    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    std::cout << "duration: " << duration << '\n';
    stability = 0;

    for (Compound * c : env.innerSolution)
    {
        stability += c->GetTotalInstability();
    }
    std::cout << stability << std::endl;

    std::vector<std::string> compStrings(0);

    /*for (int i = 0; i < 50; i++)
    {
        std::string compstr = env.innerSolution[i]->ChemicalString();
        Compound test1 = Compound(compstr);
        std::string compstr2 = test1.ChemicalString();
        Compound test2 = Compound(compstr2);
        std::string compstr3 = test2.ChemicalString();
        Compound test3 = Compound(compstr3);
        std::string compstr4 = test3.ChemicalString();
        Compound test4 = Compound(compstr4);
        if (compstr2 != compstr3)
        {
            int asdf = 1;
        }
    }

    for (Compound * c : env.innerSolution)
    {
        compStrings.emplace_back(c->ChemicalString());
    }*/

    /*for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            Organism* o;
            while(true)
            {
                std::string newcell = CreateRandomCode(compStrings);
                o = ParseCode(newcell);
                std::cout << o->AllOrganelles.size() << std::endl;
                o->CheckRep();
                o->CheckRep();

                o->CheckRep();
                o->DoDeaths();
                if (o->center == nullptr || o->AllOrganelles.size() < 3 || o->AllOrganelles.size() > 14)
                {
                    delete o;
                }
                else
                {
                    break;
                }
            }
            o->CheckRep();
            o->hasBeenPlaced = 1;
            Universe::allLife.emplace_back(o);


            for (Organelle* oo : o->AllOrganelles)
            {
                oo->xpos = 500*i + rand() % 800;
                oo->ypos = 500*j + rand() % 800;
            }
        }
    }*/

    for (Sector &s : Universe::worldHexes)
    {
        for (int i = 0; i < 5; i++)
        {
            //fairly simple, probably temporary
            Compound* newComp = new Compound();
            *newComp = *env.innerSolution[rand() % env.innerSolution.size()];
            s.AddCompoundToRandomLocationInSolution(newComp);
        }
    }
    
    
    sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY), "Squishy dudes, one day");

    std::vector<int> sectorsToVisit(0);
    start = std::clock();
    std::vector<Compound*> reactants;
    int step = 0;//-9000;
    while (window.isOpen())
    {
        step = (step + 1) % 100000;
        //while (std::clock() - start < 30)//keep a consistent minimum of 30 millis between steps
        //{
        //    Sleep(1);
        //}
        start = std::clock();

        /*for (Organism* o : Universe::allLife)
        {
            for (Organelle* oo : o->AllOrganelles)
            {
            }
        }*/

        if (step % 99 == 0)
        {
            int bigsum = 0;
            int bigcount = 0;
            int bigenergy = 0;
            int biginstability = 0;
            for (Sector s : Universe::worldHexes)
            {
                for (int idx : s.filledIdxs)
                {
                    Compound* c = s.solution[idx];
                    do
                    {
                        //c->ReCenter();
                        c->CalculateSum();
                        bigsum += c->sum;
                        bigcount += 1;
                        bigenergy += c->internalEnergy;
                        biginstability += c->GetTotalInstability();
                        if (c->sum <= 0 || c->elements[0] < 0 || c->elements[1] < 0 || c->elements[2] < 0 || c->elements[3] < 0 || c->internalEnergy < 0)
                        {
                            std::cout << "compound malformed" << std::endl;
                        }
                        if (c->elementCount > 75)
                        {
                            std::cout << "large compound" << std::endl;
                        }
                        c = c->StackedCompound;
                    } while (c != nullptr);
                }
            }
            for (Organelle* o : Universe::allLife)
            {
                o->structure.CalculateSum();
                bigsum += o->structure.sum;
                bigcount += 1;
                bigenergy += o->structure.internalEnergy;
                biginstability += o->structure.GetTotalInstability();
                for (Compound* c : o->innerSolution)
                {
                    c->CalculateSum();
                    bigsum += c->sum;
                    bigcount += 1;
                    bigenergy += c->internalEnergy;
                    biginstability += c->GetTotalInstability();
                }
            }
            std::cout << "Element sum: " << bigsum << std::endl;
            std::cout << "Count of compounds: " << bigcount << std::endl;
            std::cout << "Total internal energy: " << bigenergy << std::endl;
            std::cout << "Total instability: " << biginstability << std::endl;
            std::cout << "Thermodynamic sum: " << bigenergy + biginstability << std::endl;
        }



        for (int i = 0; i < Universe::allLife.size();)
        {
            if (Universe::allLife[i]->isded)
            {
                delete Universe::allLife[i];
                FastDelete(Universe::allLife, i);
            }
            else
            {
                Universe::allLife[i]->CheckRep();
                ++i;
            }
        }

        int pointsFilled = 0;
        for (size_t i = 0; i < Universe::worldHexes.size(); i++)
        {            
            sectorsToVisit.emplace_back(i);
            pointsFilled += Universe::worldHexes[i].filledIdxs.size();
        }
        int pointsToRemove = pointsFilled - Universe::worldHexes.size() * 8;
        //std::cout << pointsToRemove << std::endl;
        while (sectorsToVisit.size() != 0)
        {
            size_t randpick = rand() % sectorsToVisit.size();
            int sectorWeAreVisitingNow = sectorsToVisit[randpick];
            FastDelete(sectorsToVisit, randpick);

            //for (size_t j = 0; j < Universe::worldHexes[sectorWeAreVisitingNow].filledIdxs.size(); j++);

            //while (Universe::worldHexes[sectorWeAreVisitingNow].filledIdxs.size() > 15)
            //{
            //    if (pointsToRemove > 0)
            //    {
            //        delete Universe::worldHexes[sectorWeAreVisitingNow].RemoveCompoundByIdxInList(0);
            //        pointsToRemove--;
            //    }
            //    DoEntropy(sectorWeAreVisitingNow);
            //    int energy = AttemptReaction(Universe::worldHexes[sectorWeAreVisitingNow], rand() % 20 - 5, reactants);
            //    reactants.clear();
            //}
            for (size_t j = rand()%2; j < Universe::worldHexes[sectorWeAreVisitingNow].filledIdxs.size(); j++)
            {
                DoEntropy(sectorWeAreVisitingNow);
                //int energy = AttemptReaction(Universe::worldHexes[sectorWeAreVisitingNow], rand() % 20 - 5, reactants);
                //reactants.clear();
                /*if (sectorWeAreVisitingNow == 64 && step == 3)
                {
                    RandomlyReactInSolution(Universe::worldHexes[sectorWeAreVisitingNow], rand() % 20 - 25);
                }*/
                RandomlyReactInSolution(Universe::worldHexes[sectorWeAreVisitingNow], rand() % 20 - 25);
                //start of nonsense
                for (int i = 0; i < Universe::allLife.size();)
                {
                    if (Universe::allLife[i]->isded)
                    {
                        delete Universe::allLife[i];
                        FastDelete(Universe::allLife, i);
                    }
                    else
                    {
                        Universe::allLife[i]->CheckRep();
                        ++i;
                    }
                }
                //end of nonsense
            }
            for (size_t i = Universe::worldHexes[sectorWeAreVisitingNow].localPopulation.vec.size(); i > 0; i--)
            {
                FastDisconnect(&(Universe::worldHexes[sectorWeAreVisitingNow].localPopulation), i-1);
            }
            
        }

        

        /*if (step < 1 && false)
            continue;*/
        //if(step%refresh == 0)
        if (step % 3 == 0)
            DisplayAll(window);
        
        for (Organelle* o : Universe::allLife)
        {
            o->MakePresenceKnown();
        }

        for (Organelle* o : Universe::allLife)
        {
            o->SendRepositionRequests();
        }

        for (Organelle* o : Universe::allLife)
        {
            o->age++;
            //o->CheckRep();
            o->Reposition();
            o->DoDiffusion();
            //o->CheckRep();
            //o->DoChemistry(reactants);
            //o->CheckRep();
            //if (o->center != nullptr)
            //{
            //    for (Organelle* oo : o->AllOrganelles)
            //    {
            //        if (oo->structure.filledIndices.size() == 0)
            //        {
            //            int asdf = 123;
            //        }
            //    }
            //    o->DoActivations();
            //    //o->CheckRep();
            //}
            o->Activate();
        }
        for (int i = 0; i < Universe::allLife.size();)
        {
            if (Universe::allLife[i]->isded)
            {
                delete Universe::allLife[i];
                FastDelete(Universe::allLife, i);
            }
            else
            {
                Universe::allLife[i]->CheckRep();
                ++i;
            }
        }
        for (Organelle* newO : Universe::newLife)
        {
            //newO->hasBeenPlaced = 1;
            Universe::allLife.push_back(newO);
        }
        Universe::newLife.clear();
        
        do 
        {
            if (Universe::allLife.size() > 100)
            {
                break;
            }
            //Organism* o;
            Organelle* o;

            //while (true)
            //{
            //    std::string newcell = CreateRandomCode(compStrings);
            //    o = ParseCode(newcell);
            //    //std::cout << o->AllOrganelles.size() << std::endl;
            //    o->DoDeaths();
            //    if (o->center == nullptr)// || o->AllOrganelles.size() < 3 || o->AllOrganelles.size() > 24)
            //    {
            //        delete o;
            //    }
            //    else
            //    {
            //        break;
            //    }
            //}

            //o->hasBeenPlaced = 1;

            Chromosome c;
            RandomizeChromosome(&c);
            o = CreateOrganelle(&c, 0);

            int xpos = (rand() % Universe::numxsectors) * 320;
            int ypos = (rand() % Universe::numysectors) * 320;

            o->xpos = xpos + rand() % 800 - 400;
            o->ypos = ypos + rand() % 800 - 400;
            o->structure.CalculateSum();
            o->CheckRep();
            o->structure.internalEnergy = 50;
            Universe::allLife.emplace_back(o);
            /*for (Organelle* oo : o->AllOrganelles)
            {
                oo->xpos = xpos + rand() % 800 - 400;
                oo->ypos = ypos + rand() % 800 - 400;
            }*/
        } while (Universe::allLife.size() < 50);
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
