#include "Cuvant.hpp"
#include "Scor.hpp" //
#include <fstream>
#include <iostream>
#include <ctime>


Cuvant::Cuvant()
: font(), textCuvant(font, "", 64), textInput(font, "", 40)
{
}

void Cuvant::seteazaCuvant(const std::string& fisierCuvinte, const sf::RenderWindow& window)
{
    static bool seedInit = false;
    if (!seedInit) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seedInit = true;
    }

    incarcaCuvinteDinFisier(fisierCuvinte);

    const std::string fontPath = "../fonts/arial.ttf";
    if (!font.openFromFile(fontPath)) {
        std::cerr << "Eroare: nu pot incarca fontul implicit din " << fontPath << "\n";
        cuvantAleatoriu = "Eroare font!";
        textCuvant.setFont(font);
        textCuvant.setString(cuvantAleatoriu);
        return;
    }

    textCuvant.setFont(font);
    textInput.setFont(font);

    alegeAleatoriu();

    textCuvant.setString(cuvantAleatoriu);
    textCuvant.setCharacterSize(64);
    textCuvant.setFillColor(sf::Color::Black);
    textCuvant.setStyle(sf::Text::Bold);

    textInput.setString(inputUtilizator);
    textInput.setCharacterSize(40);
    textInput.setFillColor(sf::Color(60, 60, 60));
    textInput.setStyle(sf::Text::Regular);

    actualizeazaTextPozitii(window);
}

void Cuvant::incarcaCuvinteDinFisier(const std::string& fisier)
{
    std::ifstream fin(fisier);
    if (!fin.is_open()) {
        std::cerr << "Eroare: nu pot deschide fisierul " << fisier << "\n";
        return;
    }

    listaCuvinte.clear();
    std::string cuv;
    while (fin >> cuv)
        listaCuvinte.push_back(cuv);
    fin.close();
}

void Cuvant::alegeAleatoriu()
{
    if (listaCuvinte.empty()) {
        cuvantAleatoriu = "Fisier gol!";
        return;
    }
    cuvantAleatoriu = listaCuvinte[std::rand() % listaCuvinte.size()];
}

void Cuvant::actualizeazaTextPozitii(const sf::RenderWindow& window)
{
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect b1 = textCuvant.getLocalBounds();
    sf::Vector2f origin1{
        b1.position.x + b1.size.x / 2.f,
        b1.position.y + b1.size.y / 2.f
    };
    textCuvant.setOrigin(origin1);

    textCuvant.setPosition({
    static_cast<float>(windowSize.x) / 2.f,
    static_cast<float>(windowSize.y) / 2.f - 80.f
    });

    textInput.setPosition({
        static_cast<float>(windowSize.x) / 2.f,
        static_cast<float>(windowSize.y) / 2.f + 60.f
    });

    sf::FloatRect b2 = textInput.getLocalBounds();
    sf::Vector2f origin2{
        b2.position.x + b2.size.x / 2.f,
        b2.position.y + b2.size.y / 2.f
    };
    textInput.setOrigin(origin2);

    textCuvant.setPosition({
    static_cast<float>(windowSize.x) / 2.f,
    static_cast<float>(windowSize.y) / 2.f - 80.f
});

    textInput.setPosition({
        static_cast<float>(windowSize.x) / 2.f,
        static_cast<float>(windowSize.y) / 2.f + 60.f
    });
}


void Cuvant::gestioneazaEvenimente(const sf::Event& event, const sf::RenderWindow& windowRef, Scor& scor_ref)

{
    if (event.is<sf::Event::TextEntered>()) {
        auto tePtr = event.getIf<sf::Event::TextEntered>();
        if (!tePtr) return;
        uint32_t unicode = tePtr->unicode;

        if (unicode == 8) {
            if (!inputUtilizator.empty()) inputUtilizator.pop_back();
        }
        else if (unicode >= 32 && unicode < 128) { // caractere vizibile ASCII
            inputUtilizator += static_cast<char>(unicode);
        }

        textInput.setString(inputUtilizator);
        actualizeazaTextPozitii(windowRef);

        if (!cuvantAleatoriu.empty() && inputUtilizator == cuvantAleatoriu) {
            scor_ref.increment(); // ✅ creștem scorul
            alegeAleatoriu();
            textCuvant.setString(cuvantAleatoriu);
            inputUtilizator.clear();
            textInput.setString("");
            actualizeazaTextPozitii(windowRef);
        }
    }
}

void Cuvant::afiseaza(sf::RenderWindow& window)
{
    window.draw(textCuvant);
    window.draw(textInput);

    if (!tiparit) {
        std::cout << "Cuvant ales: " << cuvantAleatoriu << "\n";
        tiparit = true;
    }
}
