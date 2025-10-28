#pragma once
#include <SFML/Graphics.hpp>

class Scor
{
    int valoare;
    sf::Font font;
    sf::Text text;

public:
    Scor();

    void increment();
    void afiseaza(sf::RenderWindow& window);
};
