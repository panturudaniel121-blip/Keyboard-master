#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Buton.hpp"

enum class TipProdus {
    Culoare,
    Textura
};

struct Produs {
    int id;
    std::string nume;
    int pret;
    TipProdus tip;
    sf::Color culoare;
    sf::Texture* texturaRef;
    bool cumparat;
    bool echipat;
};

class ManagerMagazin {
    int monede;
    std::vector<Produs> produse;

    sf::Texture tex1, tex2, tex3;

    sf::Texture texCoin;
    sf::Sprite spriteCoin;

    sf::Font* fontRef;
    sf::Text* textTitlu;
    sf::Text* textBani;

    sf::RectangleShape cardShape;

    ButonMeniu* butonIesire;

    void incarcaDate();
    void salveazaDate() const;

public:
    ManagerMagazin();
    ~ManagerMagazin();

    void initializeaza(sf::Font& font);
    void actualizeaza(sf::Vector2f mousePos);
    void deseneaza(sf::RenderWindow& window);

    void gestioneazaClick(sf::Vector2f mousePos);
    bool aDatClickIesire(sf::Vector2f mousePos) const;

    void adaugaMonede(int suma);
    int getMonede() const { return monede; }

    sf::Color getCuloareFundal() const;
    const sf::Texture* getTexturaFundal() const;
};