#include "Display.h"

int WINDOWX = 1000;
int WINDOWY = 900;

int zoom = 3;
int xpan = 0;
int ypan = 0;


int refresh = 10;


void DisplayAll(sf::RenderWindow &window) 
{
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text txt;
    txt.setFont(font);

    sf::Vector2i position = sf::Mouse::getPosition();
    position = position - window.getPosition();
    sf::Event event;
    int polls = 0;
    while (window.pollEvent(event))
    {
        polls++;
        if (polls > 5)
        {
            break;
        }
        if (event.type == sf::Event::Closed)
            window.close();
        double percentageXAcross = (xpan - position.x) * 1.0 / (Universe::numxsectors * 320 / zoom);
        double percentageYAcross = (ypan - position.y) * 1.0 / (Universe::numysectors * 320 / zoom);
        std::cout << percentageXAcross << ", " << percentageYAcross << std::endl;
        if (event.type == sf::Event::MouseWheelMoved)
        {
            zoom -= event.mouseWheel.delta;
            if (zoom > 10)
            {
                zoom = 10;
                if (refresh < 10)
                {
                    refresh++;
                }
                else if (refresh < 1000)
                {
                    refresh += 100;
                }
                std::cout << "refresh set to " << refresh << std::endl;
            }
            if (zoom < 3)
            {
                zoom = 3;
                if (refresh > 19)
                {
                    refresh -= 100;
                }
                else if (refresh > 1)
                {
                    refresh--;
                }
                std::cout << "refresh set to " << refresh << std::endl;
            }
            xpan = (int)(percentageXAcross * (Universe::numxsectors * 320 / zoom)) + position.x;
            ypan = (int)(percentageYAcross * (Universe::numysectors * 320 / zoom)) + position.y;
        }
    }


    window.clear();


    if (position.x > -25 && position.y > -25 && position.x < window.getSize().x + 25 && position.y < window.getSize().y + 25)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
        {
        }
        if (position.x < WINDOWX / 4)
        {
            xpan += 25;
        }
        if (position.x > WINDOWX - WINDOWX / 4)
        {
            xpan -= 25;
        }
        if (position.y < WINDOWY / 4)
        {
            ypan += 25;
        }
        if (position.y > WINDOWY - WINDOWY / 4)
        {
            ypan -= 25;
        }
        if (xpan < WINDOWX - 25 - Universe::numxsectors * 320 / zoom)
        {
            xpan = WINDOWX - 25 - Universe::numxsectors * 320 / zoom;
        }
        if (ypan < WINDOWY - 25 - Universe::numysectors * 320 / zoom)
        {
            ypan = WINDOWY - 25 - Universe::numysectors * 320 / zoom;
        }
        if (xpan > 25)
        {
            xpan = 25;
        }
        if (ypan > 25)
        {
            ypan = 25;
        }
    }

    int staticXOffset = xpan;
    int staticYOffset = ypan;
    for (int i = 0; i < Universe::numxsectors * Universe::numysectors; i++)
    {
        int xpos = i % Universe::numxsectors * 320/zoom;
        int ypos = i / Universe::numxsectors * 320/zoom;
        sf::RectangleShape sect(sf::Vector2f(300.f/zoom, 300.f/zoom));
        sect.setPosition(staticXOffset + xpos, staticYOffset + ypos);
        window.draw(sect);
        //for (int j = 0; j < 25; j++)
        //{
        //    if (Universe::worldHexes[i].solution[j].mass < 1)
        //    {
        //        continue;
        //    }
        //    sf::RectangleShape comp(sf::Vector2f(40.f / zoom, 40.f / zoom));
        //    comp.setFillColor(Universe::worldHexes[i].solution[j].GetColor());
        //    comp.setPosition(10/zoom + staticXOffset + xpos + (j % 5) * 60/zoom, 10/zoom + staticYOffset + ypos + (j / 5) * 60/zoom);
        //    window.draw(comp);
        //}
        int j = 0;
        for (Compound * c : Universe::worldHexes[i].solution)
        {
            if (c != nullptr)
            {
                c->CalculateProduct();
                sf::RectangleShape comp(sf::Vector2f(40.f / zoom, 40.f / zoom));
                comp.setFillColor(c->GetColor());

                /*sf::Color ret = sf::Color::Black;
                ret.r += c->internalEnergy/10;
                ret.b += c->internalEnergy/10;
                comp.setFillColor(ret);*/

                comp.setPosition(10 / zoom + staticXOffset + xpos + (j % 5) * 60 / zoom, 10 / zoom + staticYOffset + ypos + (j / 5) * 60 / zoom);
                window.draw(comp);

                txt.setString(std::to_string(c->product));
                txt.setCharacterSize(45 / zoom);
                txt.setPosition(10 / zoom + staticXOffset + xpos + (j % 5) * 60 / zoom, staticYOffset + ypos + (j / 5) * 60 / zoom);
                //txt.setOrigin(size() * 0.9 / zoom, size() * 0.9 / zoom);
                txt.setOrigin(0.9 / zoom, 0.9 / zoom);
                txt.setFillColor(sf::Color::Blue);
                window.draw(txt);
            }
            j++;
        }
    }
    for (Organelle* o : Universe::allLife)
    {
        o->Display(window, zoom, staticXOffset, staticYOffset);
    }
    window.display();
}