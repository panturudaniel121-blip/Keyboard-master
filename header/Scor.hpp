#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// --- Clasa Scor ---

class Scor
{
    int valoare;
    sf::Font font;
    sf::Text text;

public:
    Scor();
    void initializareFont(const sf::Font& fontIncarcat);
    void increment();
    void afiseaza(sf::RenderWindow& window) const;
    int getValoare() const { return valoare; }
};

struct IntrareScor {
    std::string nume;
    int scor;

    bool operator>(const IntrareScor& other) const {
        return scor > other.scor;
    }
};

// --- Clasa Scor_board ---

class Scor_board {
    std::vector<IntrareScor> board;
    std::string fisierScoruri;

public:
    explicit Scor_board(const std::string& fisier);
    void SBincarcare();
    void SBresetare();
    void SBactualizare() const;

    void adaugaScor(const std::string& nume, int scor);

    void afiseaza(sf::RenderWindow& window, const sf::Font& font) const;

    friend std::istream& operator>>(std::istream& in, IntrareScor& intrare);
    friend std::ostream& operator<<(std::ostream& out, const IntrareScor& intrare);
};