#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <deque>
#include "Scor.hpp"
#include "CuvantSpecial.hpp"

enum class DificultateJoc;

struct CuvantActiv {
    std::string text;
    size_t indexTastat;
    float x;
    float y;
    float viteza;
    bool finalizat;
    TipCuvant tip;
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

    sf::RectangleShape cursor;

    bool valCurentValid;
public:
    Cuvant();
    void initializeaza(const sf::Font& fontIncarcat);
    void reseteaza();

    int actualizeaza(float dt, DificultateJoc dificultate, Scor& scor);

    TipCuvant gestioneazaEvenimente(const sf::Event& event, Scor& scor_ref, DificultateJoc dificultate,int& tasteCorecteRef);

    void afiseaza(sf::RenderWindow& window);

private:
    void incarcaDictionare();
    void spawneazaGrup(float latimeEcran, DificultateJoc dificultate);
    std::string extrageCuvantAleatoriu(DificultateJoc dificultate);
};