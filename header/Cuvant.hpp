#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <deque>
#include "Scor.hpp"

enum class DificultateJoc;

struct CuvantActiv {
    std::string text;
    size_t indexTastat;
    float x;
    float y;
    float viteza;
    bool finalizat;
};

class Cuvant
{
    std::vector<std::string> cuvinteScurte;
    std::vector<std::string> cuvinteMedii;
    std::vector<std::string> cuvinteLungi;

    std::deque<CuvantActiv> cuvinteActive;

    sf::Font font;
    sf::RectangleShape linieRosie;
    sf::Text textHelper;

    sf::Clock ceasSpawn;
    bool asteaptaSpawn;

public:
    Cuvant();
    void initializeaza(const sf::Font& fontIncarcat);
    void reseteaza();

    // MODIFICARE: Returneaza int (cate cuvinte au lovit linia rosie in acest frame)
    int actualizeaza(float dt, DificultateJoc dificultate);

    // MODIFICARE: Primeste dificultatea pentru a calcula scorul
    void gestioneazaEvenimente(const sf::Event& event, Scor& scor_ref, DificultateJoc dificultate);

    void afiseaza(sf::RenderWindow& window);

private:
    void incarcaDictionare();
    void spawneazaGrup(float latimeEcran, DificultateJoc dificultate);
    std::string extrageCuvantAleatoriu(DificultateJoc dificultate);
};