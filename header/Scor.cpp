#include "Scor.hpp"
// ... include-uri existente ...
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <utility>

// ... Constructor si initializareFont raman la fel ...

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

// MODIFICARE AICI
void Scor::adauga(int puncte)
{
    valoare += puncte;
    text.setString("Scor: " + std::to_string(valoare));
}

void Scor::afiseaza(sf::RenderWindow& window) const {
    window.draw(text);
}

// ... Restul fisierului (Scor_board) ramane EXACT la fel ...
// (Te rog sa pastrezi codul pentru Scor_board din raspunsul anterior, e corect)
std::istream& operator>>(std::istream& in, IntrareScor& intrare) {
    in >> intrare.nume >> intrare.scor;
    return in;
}
std::ostream& operator<<(std::ostream& out, const IntrareScor& intrare) {
    out << intrare.nume << " " << intrare.scor << '\n';
    return out;
}
Scor_board::Scor_board(std::string  fisier) : fisierScoruri(std::move(fisier)) {
    board.reserve(10);
}
void Scor_board::SBincarcare() {
    std::ifstream fin(fisierScoruri);
    if (!fin.is_open()) throw std::runtime_error("Eroare");
    board.clear();
    IntrareScor intrare;
    while (fin >> intrare) board.push_back(intrare);
    fin.close();
}
void Scor_board::SBresetare() {
    board.clear();
    SBactualizare();
}
void Scor_board::adaugaScor(const std::string& nume, int scor) {
    board.push_back({nume, scor});
    std::ranges::sort(board, std::greater<>());
    if (board.size() > 10) board.resize(10);
    SBactualizare();
}
void Scor_board::SBactualizare() const {
    std::ofstream fout(fisierScoruri);
    for (const auto& intrare : board) fout << intrare;
    fout.close();
}
void Scor_board::afiseaza(sf::RenderWindow& window, const sf::Font& font) const {
    sf::Text textAfisare(font, "", 28);
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
        ss << std::setw(2) << rank << ". " << std::setw(15) << std::left << intrare.nume << " : " << intrare.scor;
        textAfisare.setString(ss.str());
        textAfisare.setPosition({200.f, yPos});
        window.draw(textAfisare);
        yPos += 35.f;
        rank++;
    }
}
std::ostream& operator<<(std::ostream& out, const Scor& s) {
    out << "Scor curent  " << s.getValoare() << "\n";
    return out;
}