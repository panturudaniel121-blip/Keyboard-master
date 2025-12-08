#include "Buton.hpp"
#include "Joc.hpp"
#include <algorithm>
#include <iostream>

Buton::Buton(const sf::Vector2f& pozitie, const sf::Vector2f& dimensiune,
             const std::string& text, const sf::Font& font, sf::Color culoare)
: shape(dimensiune),textButton(font, text, 20),culoareNormala(culoare)

{
    shape.setPosition(pozitie);
    shape.setFillColor(culoare);

    culoareHover = culoare;
    culoareHover.r = std::min(255, culoare.r + 40);
    culoareHover.g = std::min(255, culoare.g + 40);
    culoareHover.b = std::min(255, culoare.b + 40);

    textButton.setFillColor(sf::Color::White);

    sf::FloatRect bounds = textButton.getLocalBounds();
    textButton.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });
    textButton.setPosition({
        pozitie.x + dimensiune.x / 2.0f,
        pozitie.y + dimensiune.y / 2.0f
    });
}

void Buton::afiseaza(sf::RenderWindow& window) const {
    doAfiseaza(window);
}

void Buton::doAfiseaza(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(textButton);
}

bool Buton::esteMouseDeasupra(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

void Buton::updateCuloare(const sf::Vector2f& mousePos) {
    if (esteMouseDeasupra(mousePos)) {
        shape.setFillColor(culoareHover);
    } else {
        shape.setFillColor(culoareNormala);
    }
}

std::ostream& operator<<(std::ostream& out, const Buton& b) {
    b.doPrint(out);
    return out;
}

void Buton::doPrint(std::ostream& out) const {
    out << "Buton Generic";
}

ButonIesire::ButonIesire(const sf::Vector2f& pos, const sf::Font& font)
    : Buton(pos, {120.f, 50.f}, "Iesire", font, sf::Color(200, 50, 50)) {}

Buton* ButonIesire::clone() const { return new ButonIesire(*this); }

void ButonIesire::executaActiune(Joc& joc) {
    joc.getWindow().close();
}

void ButonIesire::doPrint(std::ostream& out) const { out << "[Buton Iesire]"; }

ButonReset::ButonReset(const sf::Vector2f& pos, const sf::Font& font)
    : Buton(pos, {140.f, 50.f}, "Reset Top", font, sf::Color(50, 50, 200)) {}

Buton* ButonReset::clone() const { return new ButonReset(*this); }

void ButonReset::executaActiune(Joc& joc) {
    joc.reseteazaClasament();
}

void ButonReset::doPrint(std::ostream& out) const { out << "[Buton Reset]"; }

ButonRestart::ButonRestart(const sf::Vector2f& pos, const sf::Font& font)
    : Buton(pos, {140.f, 50.f}, "Joaca Iar", font, sf::Color(50, 200, 50)) {}

Buton* ButonRestart::clone() const { return new ButonRestart(*this); }

void ButonRestart::executaActiune(Joc& joc) {
    joc.restartJoc();
}

void ButonRestart::doPrint(std::ostream& out) const { out << "[Buton Restart]"; }

ButonDificultate::ButonDificultate(const sf::Vector2f& pos, const sf::Font& font,
                                   DificultateJoc dif, const std::string& text)
    : Buton(pos, {200.f, 60.f}, text, font, sf::Color(100, 100, 100)),
      dificultateAsociata(dif)
{
    if (dif == DificultateJoc::Usor) culoareNormala = sf::Color(50, 200, 50);
    else if (dif == DificultateJoc::Mediu) culoareNormala = sf::Color(200, 200, 50);
    else culoareNormala = sf::Color(200, 50, 50);

    shape.setFillColor(culoareNormala);

    culoareHover = culoareNormala;
    culoareHover.r = std::min(255, culoareNormala.r + 40);
    culoareHover.g = std::min(255, culoareNormala.g + 40);
    culoareHover.b = std::min(255, culoareNormala.b + 40);
}

Buton* ButonDificultate::clone() const { return new ButonDificultate(*this); }

void ButonDificultate::executaActiune(Joc& joc) {
    joc.setDificultate(dificultateAsociata);
    joc.incepeJoc();
}

void ButonDificultate::doPrint(std::ostream& out) const {
    out << "[Dificultate: " << static_cast<int>(dificultateAsociata) << "]";
}

ButonMeniu::ButonMeniu(const sf::Vector2f& pos, const sf::Font& font)
    : Buton(pos, {160.f, 50.f}, "Meniu", font, sf::Color(100, 100, 200))
{}

Buton* ButonMeniu::clone() const { return new ButonMeniu(*this); }

void ButonMeniu::executaActiune(Joc& joc) {
    joc.mergiLaMeniu();
}

void ButonMeniu::doPrint(std::ostream& out) const { out << "[Buton Meniu]"; }

Meniu::~Meniu() {
    for (const auto btn : butoane) delete btn;
    butoane.clear();
}

Meniu::Meniu(const Meniu& other) {
    for (const auto* btn : other.butoane) {
        butoane.push_back(btn->clone());
    }
}

Meniu& Meniu::operator=(Meniu other) {
    swap(*this, other);
    return *this;
}

void swap(Meniu& first, Meniu& second) noexcept {
    using std::swap;
    swap(first.butoane, second.butoane);
}

void Meniu::adaugaButon(Buton* btn) {
    butoane.push_back(btn);
}

void Meniu::gestioneazaClick(const sf::Vector2f& mousePos, Joc& joc) const {
    for (const auto btn : butoane) {
        if (btn->esteMouseDeasupra(mousePos)) {
            if (dynamic_cast<ButonIesire*>(btn)) {
                std::cout << "DEBUG: Exit pressed\n";
            }
            btn->executaActiune(joc);
            return;
        }
    }
}

void Meniu::actualizeazaHover(const sf::Vector2f& mousePos) const {
    for (auto btn : butoane) {
        btn->updateCuloare(mousePos);
    }
}

void Meniu::deseneaza(sf::RenderWindow& window) const {
    for (const auto btn : butoane) {
        btn->afiseaza(window);
    }
}
