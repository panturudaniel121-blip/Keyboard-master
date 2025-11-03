#include "Joc.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

Joc::Joc()
    : window(sf::VideoMode({700, 800}), "Keyboard Master"),
      fundal(52, 235, 137),
      fontPrincipal(),
      cuvant(),
      scor(),
      scorBoard("../Date/score_board.txt"),
      ceasJoc(),
      timpLimita(sf::seconds(1.f)),
      textTimer(fontPrincipal, ""),
      stareCurenta(StareJoc::Jucand),
      textGameOver(fontPrincipal, ""),
      textScorFinal(fontPrincipal, ""),
      textIntroduNume(fontPrincipal, ""),
      textNumeJucator(fontPrincipal, ""),
      numeJucator(""),
      scorSalvat(false),
      butonIesire(),
      textButonIesire(fontPrincipal, ""),
      butonReset(),
      textButonReset(fontPrincipal, "")
{
    window.setFramerateLimit(60);

    const std::string fontPath = "../fonts/arial.ttf";
    if (!fontPrincipal.openFromFile(fontPath)) {
        std::cerr << "Eroare: nu pot incarca fontul din " << fontPath << "\n";
        window.close();
        return;
    }

    scor.initializareFont(fontPrincipal);
    cuvant.seteazaCuvant("../Date/cuvinte.txt", window, fontPrincipal);

    scorBoard.SBincarcare();

    initializeazaUIGameOver();

    ceasJoc.restart();
}

void Joc::initializeazaUIGameOver()
{
    textGameOver.setString("Timpul a expirat!");
    textGameOver.setCharacterSize(50);
    textGameOver.setFillColor(sf::Color::Black);
    textGameOver.setStyle(sf::Text::Bold);

    textScorFinal.setCharacterSize(40);
    textScorFinal.setFillColor(sf::Color::Black);

    textIntroduNume.setString("Introdu numele: (apasa Enter pt. a salva)");
    textIntroduNume.setCharacterSize(24);
    textIntroduNume.setFillColor(sf::Color::Black);
    textIntroduNume.setPosition({150.f, 700.f});

    textNumeJucator.setCharacterSize(30);
    textNumeJucator.setFillColor(sf::Color(50, 50, 50));
    textNumeJucator.setPosition({150.f, 740.f});

    butonIesire.setSize({120.f, 50.f});
    butonIesire.setFillColor(sf::Color(200, 50, 50));
    butonIesire.setPosition({570.f, 5.f}); // Mutat la dreapta

    textButonIesire.setString("Iesire");
    textButonIesire.setCharacterSize(24);
    textButonIesire.setFillColor(sf::Color::White);
    textButonIesire.setPosition({595.f, 15.f}); // Textul ajustat

    butonReset.setSize({140.f, 50.f});
    butonReset.setFillColor(sf::Color(50, 50, 200));
    butonReset.setPosition({20.f, 5.f});

    textButonReset.setString("Reset Scor");
    textButonReset.setCharacterSize(22);
    textButonReset.setFillColor(sf::Color::White);
    textButonReset.setPosition({30.f, 15.f});

    textTimer.setString("Timp: 30.0");
    textTimer.setCharacterSize(24);
    textTimer.setFillColor(sf::Color::Black);
    textTimer.setPosition({550.f, 30.f});
}

void Joc::tranzitieLaGameOver()
{
    stareCurenta = StareJoc::GameOver;

    std::string text = "Scorul tau final: " + std::to_string(scor.getValoare());
    textScorFinal.setString(text);

    sf::FloatRect b1 = textGameOver.getLocalBounds();
    textGameOver.setOrigin({
        b1.position.x + b1.size.x / 2.f,
        b1.position.y + b1.size.y / 2.f
    });
    textGameOver.setPosition({window.getSize().x / 2.f, 80.f});

    sf::FloatRect b2 = textScorFinal.getLocalBounds();
    textScorFinal.setOrigin({
        b2.position.x + b2.size.x / 2.f,
        b2.position.y + b2.size.y / 2.f
    });
    textScorFinal.setPosition({window.getSize().x / 2.f, 150.f});
}

void Joc::ruleaza()
{
    while (window.isOpen())
    {
        gestioneazaEvenimente();
        actualizeaza();
        afiseaza();
    }
}

void Joc::gestioneazaEvenimente()
{
    while (auto event = window.pollEvent())
    {
        if (event->getIf<sf::Event::Closed>())
            window.close();

        if (stareCurenta == StareJoc::Jucand) {
            gestioneazaEvenimenteJucand(*event);
        } else {
            gestioneazaEvenimenteGameOver(*event);
        }
    }
}

void Joc::gestioneazaEvenimenteJucand(const sf::Event& event)
{
    cuvant.gestioneazaEvenimente(event, window, scor);
}

void Joc::gestioneazaEvenimenteGameOver(const sf::Event& event)
{
    if (auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {

        if (mouseEv->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouseEv->position);

            if (butonIesire.getGlobalBounds().contains(mousePos)) {
                window.close();
                return;
            }

            if (butonReset.getGlobalBounds().contains(mousePos)) {
                scorBoard.SBresetare();
                scorSalvat = false;
                numeJucator = "";
                textNumeJucator.setString(numeJucator);
                textIntroduNume.setString("Scor resetat! Introdu numele:");
                return;
            }

        }
    }

    if (auto textEv = event.getIf<sf::Event::TextEntered>()) {
        if (scorSalvat) return;

        uint32_t unicode = textEv->unicode;

        if (unicode == 8 && !numeJucator.empty()) {
            numeJucator.pop_back();
        }
        else if (unicode == 13 && !numeJucator.empty()) {
            scorBoard.adaugaScor(numeJucator, scor.getValoare());
            scorSalvat = true;
            textIntroduNume.setString("Scor salvat! Felicitari!");
        }
        else if (unicode >= 32 && unicode < 128 && numeJucator.size() < 15) {
            numeJucator += static_cast<char>(unicode);
        }

        textNumeJucator.setString(numeJucator);
    }
}

void Joc::actualizeaza()
{
    if (stareCurenta == StareJoc::Jucand) {
        actualizeazaJucand();
    } else {
        actualizeazaGameOver();
    }
}

void Joc::actualizeazaJucand()
{
    float timpRamas = timpLimita.asSeconds() - ceasJoc.getElapsedTime().asSeconds();
    if (timpRamas < 0) {
        timpRamas = 0;
    }

    if (timpRamas == 0) {
        tranzitieLaGameOver();
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << timpRamas;
    textTimer.setString("Timp: " + ss.str());
}

void Joc::actualizeazaGameOver()
{
}

void Joc::afiseaza()
{
    window.clear(fundal);

    if (stareCurenta == StareJoc::Jucand) {
        afiseazaJucand();
    } else {
        afiseazaGameOver();
    }

    window.display();
}

void Joc::afiseazaJucand()
{
    cuvant.afiseaza(window);
    scor.afiseaza(window);
    window.draw(textTimer);
}

void Joc::afiseazaGameOver()
{
    window.draw(textGameOver);
    window.draw(textScorFinal);

    scorBoard.afiseaza(window, fontPrincipal);

    window.draw(textIntroduNume);
    window.draw(textNumeJucator);

    window.draw(butonIesire);
    window.draw(textButonIesire);

    window.draw(butonReset);
    window.draw(textButonReset);
}