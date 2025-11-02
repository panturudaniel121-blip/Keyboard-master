#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Scor
{
    int valoare;
    sf::Font font;
    sf::Text text;

public:
    Scor();

    void increment();
    void afiseaza(sf::RenderWindow& window) const;
};
class Scor_board {

    std::pmr::vector<int> board;

public:
    Scor_board();
    void SBincarcare();
    void SBresetare();
    void SBactualizare() const;
    friend std::istream& operator>>(std::istream& in, Scor_board& sb);
    friend std::ostream& operator<<(std::ostream& out, const Scor_board& sb);
};
