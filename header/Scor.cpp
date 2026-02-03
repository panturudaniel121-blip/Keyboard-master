#include "Scor.hpp"
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <utility>

#include "Static.hpp"

Scor::Scor()
    : valoare(0), font(), text(font, "", 30), multiplicator(1), textCombo(font, "", 24),multiplicatorDificultate(1)
{
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setPosition({20.f, 20.f});
    text.setString("Scor: 0");

    textCombo.setCharacterSize(28);
    textCombo.setFillColor(sf::Color(255, 215, 0));
    textCombo.setOutlineColor(sf::Color::Black);
    textCombo.setOutlineThickness(2.f);
    textCombo.setStyle(sf::Text::Bold);
    textCombo.setPosition({300.f, 25.f});
    textCombo.setString("");
}

void Scor::initializareFont(const sf::Font& fontIncarcat) {
    font = fontIncarcat;
    text.setFont(font);
    textCombo.setFont(font);
}

void Scor::adauga(int puncte)
{
    valoare += puncte * multiplicator * multiplicatorDificultate;
    text.setString("Scor: " + std::to_string(valoare));
}

void Scor::cresteCombo() {
    if (multiplicator < 4) {
        multiplicator++;
        textCombo.setString("COMBO x" + std::to_string(multiplicator));
    }
}

void Scor::resetCombo() {
    if (multiplicator > 1) {
        multiplicator = 1;
        textCombo.setString("");
    }
}

void Scor::reset() {
    valoare = 0;
    resetCombo();
    multiplicatorDificultate = 1;
    text.setString("Scor: 0");
}

void Scor::afiseaza(sf::RenderWindow& window) const {
    window.draw(text);
    if (multiplicator > 1) {
        window.draw(textCombo);
    }
}

std::istream& operator>>(std::istream& in, IntrareScor& intrare) {
    in >> intrare.nume >> intrare.scor;
    if (!(in >> intrare.timpDisplay)) {
        intrare.timpDisplay = "-";
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const IntrareScor& intrare) {
    out << intrare.nume << " " << intrare.scor << " " << intrare.timpDisplay << '\n';
    return out;
}

Scor_board::Scor_board(std::string  fisier) : fisierScoruri(std::move(fisier)) {
    board.reserve(10);
}
void Scor_board::SBincarcare() {
    std::ifstream fin(fisierScoruri);
    if (!fin.is_open()) {
        return;
    }
    board.clear();
    IntrareScor intrare;
    while (fin >> intrare) board.push_back(intrare);
    fin.close();
}
void Scor_board::SBresetare() {
    board.clear();
    SBactualizare();
}
void Scor_board::adaugaScor(const std::string& nume, int scor, const std::string& timp) {
    board.push_back({nume, scor, timp});
    std::ranges::sort(board, std::greater<>());
    if (board.size() > 10) board.resize(10);
    SBactualizare();
}
void Scor_board::SBactualizare() const {
    std::ofstream fout(fisierScoruri);
    for (const auto& intrare : board) fout << intrare;
    fout.close();
}
void Scor_board::afiseaza(sf::RenderWindow& window, const sf::Font& font,bool arataTimp) const {
    sf::Text textAfisare(font, "", 28);
    textAfisare.setFillColor(sf::Color::Black);

    float yPos = 250.f;

    if (arataTimp) {
        textAfisare.setString("Top 10 Scoruri | Timp");
    } else {
        textAfisare.setString("Top 10 Scoruri");
    }
    textAfisare.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::FloatRect b = textAfisare.getLocalBounds();
    textAfisare.setOrigin({b.size.x / 2.f, 0.f});
    textAfisare.setPosition({static_cast<float>(Config::LATIME_FEREASTRA) / 2.f, yPos});

    window.draw(textAfisare);
    yPos += 50.f;

    textAfisare.setStyle(sf::Text::Regular);
    textAfisare.setOrigin({0.f, 0.f});

    int rank = 1;
    for (const auto& intrare : board) {
        std::stringstream ss;

        ss << std::setw(2) << rank << ". "
           << std::setw(12) << std::left << intrare.nume
           << " : " << std::setw(6) << intrare.scor;

        if (arataTimp) {
            ss << " | " << intrare.timpDisplay;
        }

        textAfisare.setString(ss.str());

        sf::FloatRect r = textAfisare.getLocalBounds();
        float xPos = (static_cast<float>(Config::LATIME_FEREASTRA) - r.size.x) / 2.f;

        textAfisare.setPosition({xPos, yPos});

        window.draw(textAfisare);
        yPos += 35.f;
        rank++;
    }
}
std::ostream& operator<<(std::ostream& out, const Scor& s) {
    out << "Scor curent  " << s.getValoare() << "\n";
    return out;
}
void Scor::setMultiplicator(const int m) {
    if (m > 0 && m <= 4) {
        multiplicator = m;
        textCombo.setString("COMBO x" + std::to_string(multiplicator));
    }
}

ScorBuilder::ScorBuilder() {
    scorTemporar.valoare = 0;
    scorTemporar.multiplicator = 1;
    scorTemporar.multiplicatorDificultate = 1;
}

ScorBuilder& ScorBuilder::cuValoareStart(int val) {
    scorTemporar.valoare = val;
    scorTemporar.text.setString("Scor: " + std::to_string(val));
    return *this;
}

ScorBuilder& ScorBuilder::cuComboStart(int combo) {
    scorTemporar.multiplicator = combo;
    if (combo > 1) scorTemporar.textCombo.setString("COMBO x" + std::to_string(combo));
    return *this;
}

ScorBuilder& ScorBuilder::cuDificultate(int dif) {
    scorTemporar.multiplicatorDificultate = dif;
    return *this;
}

Scor ScorBuilder::build() {
    return scorTemporar;
}