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
    srand(46);
    //srand(42);
    //srand(time(NULL));
    Grids::InitGrids(65, 65);
    
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

    Membrane env;

    for (int i = 0; i < 100; i++)
    {
        Compound * c = new Compound();
        for (int j = 0; j < 4; j++)
        {
            c->AddElementAtIndex(PeriodicTable::ptable[rand() % PeriodicTable::ptable.size()], j, 0);
        }
        c->id = i;
        env.innerSolution.emplace_back(c);
    }
    int stability = 0;

    for (Compound * c : env.innerSolution)
    {
        stability += c->GetTotalStability();
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
        AttemptReaction(env, 10, asdf);

    }

    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    std::cout << "duration: " << duration << '\n';
    stability = 0;

    for (Compound * c : env.innerSolution)
    {
        stability += c->GetTotalStability();
    }
    std::cout << stability << std::endl;

    std::vector<std::string> compStrings(0);

    for (int i = 0; i < 50; i++)
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
    }

    for (int i = 0; i < 7; i++)
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
    }

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
        /*while (std::clock() - start < 30)//keep a consistent minimum of 30 millis between steps
        {
            Sleep(1);
        }*/
        start = std::clock();
        for (Organism* o : Universe::allLife)
        {
            for (Organelle* oo : o->AllOrganelles)
            {
            }
        }

        if (step % 99 == 0)
        {
            for (Sector s : Universe::worldHexes)
            {
                for (int idx : s.filledIdxs)
                {
                    Compound* c = s.solution[idx];
                    do
                    {
                        c->ReCenter();
                        c = c->StackedCompound;
                    } while (c != nullptr);
                }
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
            while (Universe::worldHexes[sectorWeAreVisitingNow].filledIdxs.size() > 15)
            {
                if (pointsToRemove > 0)
                {
                    delete Universe::worldHexes[sectorWeAreVisitingNow].RemoveCompoundByIdxInList(0);
                    pointsToRemove--;
                }
                DoEntropy(sectorWeAreVisitingNow);
                int energy = AttemptReaction(Universe::worldHexes[sectorWeAreVisitingNow], rand() % 20 - 5, reactants);
                reactants.clear();
            }
            for (size_t j = rand()%2; j < Universe::worldHexes[sectorWeAreVisitingNow].filledIdxs.size(); j++)
            {
                DoEntropy(sectorWeAreVisitingNow);
                int energy = AttemptReaction(Universe::worldHexes[sectorWeAreVisitingNow], rand() % 20 - 5, reactants);
                reactants.clear();
            }
        }
        if (step < 1 && false)
            continue;
        if(step%refresh == 0)
            DisplayAll(window);

        for (Organism* o : Universe::allLife)
        {
            o->age++;
            //o->CheckRep();
            o->Reposition();
            //o->CheckRep();
            o->DoChemistry(reactants);
            //o->CheckRep();
            if (o->center != nullptr)
            {
                for (Organelle* oo : o->AllOrganelles)
                {
                    if (oo->structure.filledIndices.size() == 0)
                    {
                        int asdf = 123;
                    }
                }
                o->DoActivations();
                //o->CheckRep();
            }
        }
        for (Organism* o : Universe::allLife)
        {
            o->DoDeaths();
        }
        for (int i = 0; i < Universe::allLife.size();)
        {
            if (Universe::allLife[i]->center == nullptr)
            {
                for (Organelle* o : Universe::allLife[i]->AllOrganelles)
                {
                    //dump guts
                    if (!o->isded)
                    {
                        o->SeverAllConnections();
                    }
                }
                delete Universe::allLife[i];
                FastDelete(Universe::allLife, i);
            }
            else
            {
                ++i;
            }
        }
        for (Organism* newO : Universe::newLife)
        {
            newO->hasBeenPlaced = 1;
            Universe::allLife.push_back(newO);
        }
        Universe::newLife.clear();
        
        do 
        {
            if (Universe::allLife.size() > 100)
            {
                break;
            }
            Organism* o;
            while (true)
            {
                std::string newcell = CreateRandomCode(compStrings);
                o = ParseCode(newcell);
                //std::cout << o->AllOrganelles.size() << std::endl;
                o->DoDeaths();
                if (o->center == nullptr)// || o->AllOrganelles.size() < 3 || o->AllOrganelles.size() > 24)
                {
                    delete o;
                }
                else
                {
                    break;
                }
            }

            o->hasBeenPlaced = 1;
            Universe::allLife.emplace_back(o);

            int xpos = (rand() % Universe::numxsectors) * 320;
            int ypos = (rand() % Universe::numysectors) * 320;
            for (Organelle* oo : o->AllOrganelles)
            {
                oo->xpos = xpos + rand() % 800 - 400;
                oo->ypos = ypos + rand() % 800 - 400;
            }
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
