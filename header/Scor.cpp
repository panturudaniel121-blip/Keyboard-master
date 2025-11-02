#include "Scor.hpp"
#include <fstream>
#include <iostream>

Scor::Scor()
    : valoare(0), font(), text(font, "", 30)
{


    text.setFont(font);
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setPosition({20.f, 20.f});
    text.setString("Scor: 0");
}

void Scor::increment()
{
    valoare++;
    text.setString("Scor: " + std::to_string(valoare));
}

void Scor::afiseaza(sf::RenderWindow& window) const {
    window.draw(text);
}

Scor_board::Scor_board()= default;

std::istream& operator>>(std::istream& in, Scor_board& sb) {
    for (int i = 0; i < 10; ++i) {
        if (!(in >> sb.board[i])) {
            sb.board[i] = 0;
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Scor_board& sb) {
    for (int i = 0; i < 10; ++i) {
        out << sb.board[i] << '\n';
    }
    return out;
}

void Scor_board::SBincarcare()
{
    std::ifstream fin("date/scoare_board");
    if (!fin.is_open()) {
        std::cerr << "Eroare: nu s-a putut deschide fisierul 'date/scoare_board' pentru citire.\n";
        return;
    }
    fin>>*this;
    fin.close();
}

void Scor_board::SBresetare()
{
    std::ifstream fin("date/scoare_board");
    if (!fin.is_open()) {
        std::cerr << "Eroare: nu s-a putut deschide fisierul 'date/scoare_board' pentru citire.\n";
        return;
    }
    for (int i = 0; i < 10; ++i) board[i]=0;
    fin.close();
}

void  Scor_board::SBactualizare() const {
    std::ofstream fout("date/scoare_board");
    if (!fout.is_open()) {
        std::cerr << "Eroare: nu s-a putut deschide fisierul 'date/scoare_board' pentru citire.\n";
        return;
    }
    fout.open("scor_board.txt", std::ofstream::out | std::ofstream::trunc);
    fout<<*this;
    fout.close();
}
