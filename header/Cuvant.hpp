#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Scor;

class Cuvant
{
    std::vector<std::string> listaCuvinte;
    std::string cuvantAleatoriu;
    std::string inputUtilizator;
    sf::Font font;
    sf::Text textCuvant;
    sf::Text textInput;
    bool tiparit = false;
    Scor* scor = nullptr;

public:
    Cuvant();

    void seteazaCuvant(const std::string& fisierCuvinte, const sf::RenderWindow& window);
    void seteazaScor(Scor* s) { scor = s; }
    void gestioneazaEvenimente(const sf::Event& event, const sf::RenderWindow& window, Scor& scor_ref);
    void afiseaza(sf::RenderWindow& window);
    const std::string& getCuvant() const { return cuvantAleatoriu; }

private:
    void incarcaCuvinteDinFisier(const std::string& fisier);
    void alegeAleatoriu();
    void actualizeazaTextPozitii(const sf::RenderWindow& window);
};
