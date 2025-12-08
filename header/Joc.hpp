#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Cuvant.hpp"
#include "Scor.hpp"
#include "Buton.hpp"


enum class StareJoc {
    SelectieDificultate,
    Jucand,
    GameOver,
    Pierdut
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

    int hpCurent;
    int hpMaxim;
    sf::Text textHP;

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

    Meniu meniuPierdut;
    sf::Text textMesajPierdut;

    sf::Music muzicaMeniu;
    sf::Music muzicaJoc;
    sf::Music muzicaPierdut;

    bool sunetOprit;

public:
    Joc();
    void ruleaza();

    sf::RenderWindow& getWindow() { return window; }

    void reseteazaClasament();
    void restartJoc();
    void incepeJoc();

    void setDificultate(DificultateJoc dif);
    void mergiLaMeniu();

    void toggleMute();

    friend std::ostream& operator<<(std::ostream& out, const Joc& j);

private:
    void initializeazaUIGameOver();
    void initializeazaUIStart();
    void initializeazaUIPierdut();

    void tranzitieLaGameOver();
    void tranzitieLaPierdut();

    void gestioneazaEvenimente();
    void gestioneazaEvenimenteJucand(const sf::Event& event);
    void gestioneazaEvenimenteGameOver(const sf::Event& event);
    void gestioneazaEvenimenteStart(const sf::Event& event);
    void gestioneazaEvenimentePierdut(const sf::Event& event);

    void actualizeaza();
    void actualizeazaJucand();

    static void actualizeazaGameOver();

    void incarcaMuzica();

    void afiseaza();
    void afiseazaJucand();
    void afiseazaGameOver();
    void afiseazaStart();
    void afiseazaPierdut();
};