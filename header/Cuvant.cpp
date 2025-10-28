#include "Cuvant.hpp"
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

Cuvant::Cuvant()
: font(), text(font, "", 30)
{
}

void Cuvant::seteazaCuvant(const std::string& fisierCuvinte, const sf::RenderWindow& window)
{
    static bool seedInit = false;
    if (!seedInit) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seedInit = true;
    }

    incarcaCuvinteDinFisier(fisierCuvinte);

    const std::string fontPath = "../fonts/arial.ttf";
    if (!font.openFromFile(fontPath)) {
        std::cerr << "Eroare: nu pot incarca fontul implicit din " << fontPath << "\n";
        cuvantAleatoriu = "Eroare font!";
        text.setFont(font);
        text.setString(cuvantAleatoriu);
        return;
    }

    alegeAleatoriu();

    text.setFont(font);
    text.setString(cuvantAleatoriu);
    text.setCharacterSize(64);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);

    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f origin{
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    };
    text.setOrigin(origin);

    sf::Vector2u windowSize = window.getSize();
    text.setPosition({
        windowSize.x / 2.f,
        windowSize.y / 2.f
    });
}

void Cuvant::incarcaCuvinteDinFisier(const std::string& fisier)
{
    std::ifstream fin(fisier);
    if (!fin.is_open()) {
        std::cerr << "Eroare: nu pot deschide fisierul " << fisier << "\n";
        return;
    }

    listaCuvinte.clear();
    std::string cuv;
    while (fin >> cuv)
        listaCuvinte.push_back(cuv);

    fin.close();
}

void Cuvant::alegeAleatoriu()
{
    if (listaCuvinte.empty()) {
        cuvantAleatoriu = "Fisier gol!";
        return;
    }

    cuvantAleatoriu = listaCuvinte[std::rand() % listaCuvinte.size()];
}

void Cuvant::afiseaza(sf::RenderWindow& window)
{
    window.draw(text);

    if (!tiparit) {
        std::cout << "Cuvant ales: " << cuvantAleatoriu << "\n";
        tiparit = true;
    }
}
