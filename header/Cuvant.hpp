#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Scor.hpp"
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

public:
    Cuvant();
    ~Cuvant();
    Cuvant(const Cuvant& other);
    Cuvant& operator=(const Cuvant& other);

    void seteazaCuvant(const std::string& fisierCuvinte, const sf::RenderWindow& window, const sf::Font& fontIncarcat);
    void gestioneazaEvenimente(const sf::Event& event, const sf::RenderWindow& window, Scor& scor_ref);
    void afiseaza(sf::RenderWindow& window);

    friend std::ostream& operator<<(std::ostream& out, const Cuvant& c);

private:
    void incarcaCuvinteDinFisier(const std::string& fisier);
    void alegeAleatoriu();
    void actualizeazaTextPozitii(const sf::RenderWindow& window);
};