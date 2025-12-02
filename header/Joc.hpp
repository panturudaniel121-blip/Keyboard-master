#pragma once
#include <SFML/Graphics.hpp>
#include "Cuvant.hpp"
#include "Scor.hpp"
#include "Buton.hpp" // Contine enum class DificultateJoc


enum class StareJoc {
    SelectieDificultate,
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
    DificultateJoc nivelDificultate;

    sf::Text textGameOver;
    sf::Text textScorFinal;
    sf::Text textIntroduNume;
    sf::Text textNumeJucator;
    std::string numeJucator;
    bool scorSalvat;
    Meniu meniuGameOver;

    Meniu meniuStart;
    sf::Text textTitluStart;

public:
    Joc();
    void ruleaza();
    friend std::ostream& operator<<(std::ostream& out, const Joc& j);

    sf::RenderWindow& getWindow() { return window   ; }
    void reseteazaClasament();
    void restartJoc();

    void setDificultate(DificultateJoc dif);
    void incepeJoc();

private:
    void initializeazaUIGameOver();
    void initializeazaUIStart();
    void tranzitieLaGameOver();

    void gestioneazaEvenimente();
    void gestioneazaEvenimenteJucand(const sf::Event& event);
    void gestioneazaEvenimenteGameOver(const sf::Event& event);
    void gestioneazaEvenimenteStart(const sf::Event& event);

    void actualizeaza();
    void actualizeazaJucand();
    static void actualizeazaGameOver();

    void afiseaza();
    void afiseazaJucand();
    void afiseazaGameOver();
    void afiseazaStart();
};