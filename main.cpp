#include <SFML/Graphics.hpp>
#include "header/Cuvant.hpp"
#include "header/Scor.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({700, 800}), "Keyboard Master");
    window.setFramerateLimit(60);

    constexpr sf::Color fundal(52, 235, 137);
    Scor scor;
    Cuvant cuvant;
    cuvant.seteazaCuvant("../Date/cuvinte.txt", window);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->getIf<sf::Event::Closed>())
                window.close();

            cuvant.gestioneazaEvenimente(*event, window, scor); //
        }

        window.clear(fundal);
        cuvant.afiseaza(window);
        scor.afiseaza(window); //
        window.display();
    }


    return 0;
}
