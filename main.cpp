#include <SFML/Graphics.hpp>
#include "header/Cuvant.hpp"
int main()
{
    sf::RenderWindow window(sf::VideoMode({700, 800}), "Keyboard Master");
    window.setFramerateLimit(60); // limitează FPS la 60

    sf::Color fundal(52, 235, 137);
    Cuvant cuvant;
    cuvant.seteazaCuvant("../Date/cuvinte.txt", window);
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (auto closed = event->getIf<sf::Event::Closed>())
                window.close();
        }

        window.clear(fundal); // fundal constant
        cuvant.afiseaza(window); // placeholder vizual
        window.display();
    }
    return 0;
}
