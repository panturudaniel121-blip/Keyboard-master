#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

struct Particula {
    sf::Vector2f pozitie;
    sf::Vector2f viteza;

    float durataViata = 0.f;
    float durataMaxima = 0.f;

    sf::Color culoareStart;
    float marime = 1.0f;
};

class ManagerEfecte {
    std::vector<Particula> particule;
    sf::RectangleShape shapeParticula;

    sf::SoundBuffer bufferTasta;
    sf::SoundBuffer bufferBoom;
    sf::SoundBuffer bufferWin;

    std::vector<sf::Sound> poolSuneteTasta;
    int indexSunetCurent = 0;

    sf::Sound sunetBoom;
    sf::Sound sunetWin;
public:
    ManagerEfecte();

    void incarcaSunete();
    void actualizeaza(float dt);
    void deseneaza(sf::RenderWindow& window);

    void spawnLiteraCorecta(sf::Vector2f pos);
    void spawnExplozieCuvant(sf::Vector2f pos, sf::Color culoareBaza);

    void playTasta();
    void playBoom();

    void spawnConfetti();
    void playWin();
};