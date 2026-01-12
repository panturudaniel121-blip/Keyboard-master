#include "Shop.hpp"
#include "Static.hpp"
#include <fstream>
#include <iostream>

ManagerMagazin::ManagerMagazin()
    : monede(0),
      texCoin(),
      spriteCoin(texCoin),
      fontRef(nullptr),
      textTitlu(nullptr),
      textBani(nullptr),
      butonIesire(nullptr)
{
    if (!tex1.loadFromFile(Config::CALE_TEXTURA_BG_1)) std::cerr << "Eroare BG1\n";
    if (!tex2.loadFromFile(Config::CALE_TEXTURA_BG_2)) std::cerr << "Eroare BG2\n";
    if (!tex3.loadFromFile(Config::CALE_TEXTURA_BG_3)) std::cerr << "Eroare BG3\n";

    if (!texCoin.loadFromFile(Config::CALE_ICON_COIN)) {
        std::cerr << "!!! EROARE: Nu gasesc " << Config::CALE_ICON_COIN << " !!!\n";
        sf::Image img;
        img.resize({32, 32}, sf::Color::Red);
        if (!texCoin.loadFromImage(img)) {
             std::cerr << "Eroare critica fallback.\n";
        }
    }

    spriteCoin.setTexture(texCoin, true);

    spriteCoin.setScale({0.3f, 0.3f});

    produse.push_back({0, "Clasic", 0, TipProdus::Culoare, sf::Color(52, 235, 137), nullptr, true, true});
    produse.push_back({1, "Ocean", 100, TipProdus::Culoare, sf::Color(0, 100, 200), nullptr, false, false});
    produse.push_back({2, "Dark", 200, TipProdus::Culoare, sf::Color(30, 30, 30), nullptr, false, false});
    produse.push_back({3, "Hartie", 500, TipProdus::Textura, sf::Color::White, &tex1, false, false});
    produse.push_back({4, "superGalben", 750, TipProdus::Textura, sf::Color::White, &tex2, false, false});
    produse.push_back({5, "Vant", 1000, TipProdus::Textura, sf::Color::White, &tex3, false, false});

    incarcaDate();
}

ManagerMagazin::~ManagerMagazin() {
    delete butonIesire;
    delete textTitlu;
    delete textBani;
}

void ManagerMagazin::initializeaza(sf::Font& font) {
    fontRef = &font;

    textTitlu = new sf::Text(font, "MAGAZIN", 40);
    textTitlu->setFillColor(sf::Color::Black);
    textTitlu->setPosition({Config::LATIME_FEREASTRA / 2.f - 80.f, 20.f});

    textBani = new sf::Text(font, "", 30);
    textBani->setFillColor(sf::Color::Black);
    textBani->setStyle(sf::Text::Bold);
    textBani->setPosition({Config::LATIME_FEREASTRA - 150.f, 30.f});

    butonIesire = new ButonMeniu({50.f, 700.f}, font);

    cardShape.setSize({200.f, 120.f});
    cardShape.setOutlineThickness(2.f);
}

void ManagerMagazin::actualizeaza(sf::Vector2f mousePos) {
    if (textBani) {
        textBani->setString(std::to_string(monede));
        sf::Vector2f pos = textBani->getPosition();

        spriteCoin.setScale({0.3f, 0.3f});

        spriteCoin.setPosition({pos.x - 65.f, pos.y - 10.f});
    }
    if (butonIesire) butonIesire->updateCuloare(mousePos);
}

void ManagerMagazin::deseneaza(sf::RenderWindow& window) {
    if (textTitlu) window.draw(*textTitlu);

    if (textBani) {
        spriteCoin.setScale({0.3f, 0.3f});
        window.draw(spriteCoin);
        window.draw(*textBani);
    }

    for (size_t i = 0; i < produse.size(); ++i) {
        constexpr float startX = 50.f;
        constexpr float startY = 100.f;
        const auto& p = produse[i];

        const int col = static_cast<int>(i % 2);
        const int row = static_cast<int>(i / 2);

        float x = startX + static_cast<float>(col) * 320.f;
        float y = startY + static_cast<float>(row) * 160.f;

        cardShape.setPosition({x, y});

        if (p.echipat) {
             cardShape.setFillColor(sf::Color(200, 255, 200));
             cardShape.setOutlineColor(sf::Color::Green);
        } else if (p.cumparat) {
             cardShape.setFillColor(sf::Color(240, 240, 240));
             cardShape.setOutlineColor(sf::Color::Black);
        } else {
             if (monede >= p.pret) {
                 cardShape.setFillColor(sf::Color::White);
                 cardShape.setOutlineColor(sf::Color::Blue);
             } else {
                 cardShape.setFillColor(sf::Color(220, 220, 220));
                 cardShape.setOutlineColor(sf::Color::Red);
             }
        }

        window.draw(cardShape);

        if (fontRef) {
            sf::Text tNume(*fontRef, p.nume, 24);
            tNume.setFillColor(sf::Color::Black);
            tNume.setPosition({x + 10.f, y + 10.f});
            window.draw(tNume);

            std::string sPret = p.echipat ? "ECHIPAT" : (p.cumparat ? "DETINUT" : std::to_string(p.pret));
            sf::Text tPret(*fontRef, sPret, 20);
            tPret.setFillColor((!p.cumparat && monede < p.pret) ? sf::Color::Red : sf::Color(50, 50, 50));
            tPret.setPosition({x + 10.f, y + 80.f});
            window.draw(tPret);

            if (!p.cumparat && !p.echipat) {
                sf::Sprite tempCoin = spriteCoin;
                tempCoin.setScale({0.22f, 0.22f});

                const sf::FloatRect bounds = tPret.getGlobalBounds();
                tempCoin.setPosition({bounds.position.x + bounds.size.x + 10.f, bounds.position.y - 8.f });
                window.draw(tempCoin);
            }
        }

        sf::RectangleShape preview({40.f, 40.f});
        preview.setPosition({x + 150.f, y + 10.f});
        preview.setOutlineThickness(1.f);
        preview.setOutlineColor(sf::Color::Black);

        if (p.tip == TipProdus::Culoare) {
            preview.setFillColor(p.culoare);
        } else {
            preview.setFillColor(sf::Color::White);
            if (p.texturaRef) preview.setTexture(p.texturaRef);
        }
        window.draw(preview);
    }

    if (butonIesire) butonIesire->afiseaza(window);
}

void ManagerMagazin::gestioneazaClick(sf::Vector2f mousePos) {
    for (size_t i = 0; i < produse.size(); ++i) {
        constexpr float startX = 50.f;
        constexpr float startY = 100.f;

        const int col = static_cast<int>(i % 2);
        const int row = static_cast<int>(i / 2);

        float x = startX + static_cast<float>(col) * 320.f;
        float y = startY + static_cast<float>(row) * 160.f;

        if (sf::FloatRect({x, y}, {200.f, 120.f}).contains(mousePos)) {
            auto& p = produse[i];

            if (p.cumparat) {
                for (auto& other : produse) other.echipat = false;
                p.echipat = true;
                salveazaDate();
            } else {
                if (monede >= p.pret) {
                    monede -= p.pret;
                    p.cumparat = true;
                    for (auto& other : produse) other.echipat = false;
                    p.echipat = true;
                    salveazaDate();
                }
            }
        }
    }
}

bool ManagerMagazin::aDatClickIesire(sf::Vector2f mousePos) const {
    if (butonIesire) return butonIesire->esteMouseDeasupra(mousePos);
    return false;
}

void ManagerMagazin::adaugaMonede(int suma) {
    monede += suma;
    salveazaDate();
}

sf::Color ManagerMagazin::getCuloareFundal() const {
    for (const auto& p : produse) {
        if (p.echipat && p.tip == TipProdus::Culoare) return p.culoare;
    }
    return sf::Color::White;
}

const sf::Texture* ManagerMagazin::getTexturaFundal() const {
    for (const auto& p : produse) {
        if (p.echipat && p.tip == TipProdus::Textura) return p.texturaRef;
    }
    return nullptr;
}

void ManagerMagazin::salveazaDate() const {
    std::ofstream fout(Config::CALE_SHOP_DATA);
    if (fout.is_open()) {
        fout << monede << "\n";
        for (const auto& p : produse) {
            fout << p.id << " " << p.cumparat << " " << p.echipat << "\n";
        }
    }
}

void ManagerMagazin::incarcaDate() {
    std::ifstream fin(Config::CALE_SHOP_DATA);
    if (fin.is_open()) {
        fin >> monede;
        int id; bool c, e;
        while (fin >> id >> c >> e) {
            if (id >= 0 && static_cast<size_t>(id) < produse.size()) {
                produse[id].cumparat = c;
                produse[id].echipat = e;
            }
        }
    }
}