#pragma once
#include <SFML/Graphics.hpp>
#include "Cuvant.hpp"
#include "Scor.hpp"

enum class StareJoc {
    Jucand,
    GameOver
};

class Joc
{
    sf::RenderWindow window;
    sf::Color fundal;
    sf::Font fontPrincipal;

    Cuvant cuvant;
    Scor scor;
    Scor_board scorBoard;

    sf::Clock ceasJoc;
    sf::Time timpLimita;
    sf::Text textTimer;

    StareJoc stareCurenta;

    sf::Text textGameOver;
    sf::Text textScorFinal;
    sf::Text textIntroduNume;
    sf::Text textNumeJucator;
    std::string numeJucator;
    bool scorSalvat;

    sf::RectangleShape butonIesire;
    sf::Text textButonIesire;

    sf::RectangleShape butonReset;
    sf::Text textButonReset;

public:
    Joc();
    void ruleaza();

private:
    void initializeazaUIGameOver();
    void tranzitieLaGameOver();

    void gestioneazaEvenimente();
    void gestioneazaEvenimenteJucand(const sf::Event& event);
    void gestioneazaEvenimenteGameOver(const sf::Event& event);

    void actualizeaza();
    void actualizeazaJucand();

    static void actualizeazaGameOver();

    void afiseaza();
    void afiseazaJucand();
    void afiseazaGameOver();
};