#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Cuvant
{
    std::vector<std::string> listaCuvinte;
    std::string cuvantAleatoriu;
    sf::Font font;
    sf::Text text;
    bool tiparit = false;

public:
    Cuvant(); // ✅ avem nevoie de implementare
    void seteazaCuvant(const std::string& fisierCuvinte, const sf::RenderWindow& window);
    const std::string& getCuvant() const { return cuvantAleatoriu; }
    void afiseaza(sf::RenderWindow& window);

    void incarcaCuvinteDinFisier(const std::string& fisier);
    void alegeAleatoriu();
};
