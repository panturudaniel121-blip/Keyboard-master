#include "Scor.hpp"
#include <iostream>

Scor::Scor()
    : valoare(0), font(), text(font, "", 30) // inițializare directă ca la Cuvant
{
    if (!font.openFromFile("../fonts/arial.ttf")) {
        std::cerr << "Eroare: nu pot incarca fontul "<< "\n";
    }

    text.setFont(font);
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setPosition({20.f, 20.f});
    text.setString("Scor: 0");
}

void Scor::increment()
{
    valoare++;
    text.setString("Scor: " + std::to_string(valoare));
}

void Scor::afiseaza(sf::RenderWindow& window)
{
    window.draw(text);
}
