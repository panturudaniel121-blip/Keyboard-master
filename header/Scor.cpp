#include "Scor.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

// --- Clasa Scor ---

Scor::Scor()
    : valoare(0), font(), text(font, "", 30)
{
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setPosition({20.f, 20.f});
    text.setString("Scor: 0");
}

void Scor::initializareFont(const sf::Font& fontIncarcat) {
    font = fontIncarcat;
    text.setFont(font);
}

void Scor::increment()
{
    valoare++;
    text.setString("Scor: " + std::to_string(valoare));
}

void Scor::afiseaza(sf::RenderWindow& window) const {
    window.draw(text);
}


// --- Clasa Scor_board ---

std::istream& operator>>(std::istream& in, IntrareScor& intrare) {
    in >> intrare.nume >> intrare.scor;
    return in;
}

std::ostream& operator<<(std::ostream& out, const IntrareScor& intrare) {
    out << intrare.nume << " " << intrare.scor << '\n';
    return out;
}


Scor_board::Scor_board(const std::string& fisier) : fisierScoruri(fisier) {
    board.reserve(10);
}

void Scor_board::SBincarcare()
{
    std::ifstream fin(fisierScoruri);
    if (!fin.is_open()) {
        std::cerr << "Eroare: nu s-a putut deschide fisierul '" << fisierScoruri << "' pentru citire.\n";
        SBactualizare();
        return;
    }

    board.clear();
    IntrareScor intrare;
    while (fin >> intrare) {
        board.push_back(intrare);
    }
    fin.close();
}

void Scor_board::SBresetare()
{
    board.clear();
    SBactualizare();
}

void Scor_board::adaugaScor(const std::string& nume, int scor)
{
    board.push_back({nume, scor});
    //range-sort facut de Clion
    std::ranges::sort(board, std::greater<IntrareScor>());

    if (board.size() > 10) {
        board.resize(10);
    }

    SBactualizare();
}

void Scor_board::SBactualizare() const {
    std::ofstream fout(fisierScoruri);
    if (!fout.is_open()) {
        std::cerr << "Eroare: nu s-a putut deschide fisierul '" << fisierScoruri << "' pentru scriere.\n";
        return;
    }

    for (const auto& intrare : board) {
        fout << intrare;
    }
    fout.close();
}

void Scor_board::afiseaza(sf::RenderWindow& window, const sf::Font& font) const
{
    sf::Text textAfisare(font, "");
    textAfisare.setFont(font);
    textAfisare.setCharacterSize(28);
    textAfisare.setFillColor(sf::Color::Black);

    float yPos = 250.f;

    textAfisare.setString("Top 10 Scoruri:");
    textAfisare.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textAfisare.setPosition({250.f, yPos});
    window.draw(textAfisare);
    yPos += 50.f;

    textAfisare.setStyle(sf::Text::Regular);
    int rank = 1;
    for (const auto& intrare : board) {
        std::stringstream ss;
        ss << std::setw(2) << rank << ". " << std::setw(15) << std::left
           << intrare.nume << " : " << intrare.scor;

        textAfisare.setString(ss.str());
        textAfisare.setPosition({200.f, yPos});
        window.draw(textAfisare);

        yPos += 35.f;
        rank++;
    }
}