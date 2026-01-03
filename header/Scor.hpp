#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Scor
{
    int valoare;
    sf::Font font;
    sf::Text text;
    int multiplicator;
    sf::Text textCombo;

public:
    Scor();
    void initializareFont(const sf::Font& fontIncarcat);

    void adauga(int puncte);

    void cresteCombo();
    void resetCombo();
    void reset();
    void setMultiplicator(int m);
    void afiseaza(sf::RenderWindow& window) const;
    int getValoare() const { return valoare; }
    int getCombo() const { return multiplicator; }
    friend std::ostream& operator<<(std::ostream& out, const Scor& s);
};

struct IntrareScor {
    std::string nume;
    int scor;
    std::string timpDisplay;
    bool operator>(const IntrareScor& other) const { return scor > other.scor; }
};

class Scor_board {
    std::vector<IntrareScor> board;
    std::string fisierScoruri;
public:
    explicit Scor_board(std::string fisier);
    void SBincarcare();
    void SBresetare();
    void SBactualizare() const;
    void adaugaScor(const std::string& nume, int scor, const std::string& timp = "-");
    void afiseaza(sf::RenderWindow& window, const sf::Font& font,bool arataTimp) const;
    friend std::istream& operator>>(std::istream& in, IntrareScor& intrare);
    friend std::ostream& operator<<(std::ostream& out, const IntrareScor& intrare);
};