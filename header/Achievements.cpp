#include "Achievements.hpp"
#include "Static.hpp"
#include "Buton.hpp"
#include <fstream>
#include <iostream>

sf::Color ManagerAchievements::getCuloareNivel(int nivel) {
    switch (nivel) {
        case 0: return sf::Color({100, 100, 100});
        case 1: return sf::Color({205, 127, 50});
        case 2: return sf::Color({192, 192, 192});
        case 3: return sf::Color({255, 215, 0});
        case 4: return sf::Color({0, 255, 255});
        case 5: return sf::Color({255, 0, 0});
        case 6: return sf::Color({148, 0, 211});
        default: return sf::Color({255, 255, 255});
    }
}

ManagerAchievements::ManagerAchievements()
    : fontRef(nullptr),
      notificareTitlu(nullptr),
      notificareNume(nullptr),
      titluMeniu(nullptr),
      timpAfisareNotificare(0.f),
      afiseazaNotificare(false),
      sunetUnlock(nullptr),
      butonMeniu(nullptr),
      butonReset(nullptr)
{
    notificareBg.setSize({400.f, 90.f});
    notificareBg.setFillColor(sf::Color(30, 30, 30, 230));
    notificareBg.setOutlineThickness(2.f);
    notificareBg.setOutlineColor(sf::Color::White);
    notificareBg.setPosition({(Config::LATIME_FEREASTRA - 400.f) / 2.f, -120.f});

    listaAchievements = {
        {"campion", "Campion", "Castiga jocul pe dificultatea:", 0, {0, 1, 2}},
        {"combo_master", "Combo Master", "Atinge Combo x", 0, {2, 3, 4}},
        {"sniper", "Sniper", "100% Acuratete la cuvinte:", 0, {5, 10, 15, 20}},
        {"survivor", "Supravietuitor", "Ajungi la Wave:", 0, {5, 10, 15, 20, 25}},
        {"invincibil", "Invincibil", "Atinge Scorul (cu HP Max):", 0, {250, 500, 750, 1000, 1250, 1500}}
    };

    incarcaProgres();
}

ManagerAchievements::~ManagerAchievements() {
    delete notificareTitlu;
    delete notificareNume;
    delete titluMeniu;
    delete sunetUnlock;
    delete butonMeniu;
    delete butonReset;
}

void ManagerAchievements::initializeaza(const sf::Font& font) {
    fontRef = &font;

    notificareTitlu = new sf::Text(font, "ACHIEVEMENT LEVEL UP!", 16);
    notificareTitlu->setFillColor(sf::Color::Yellow);
    notificareTitlu->setStyle(sf::Text::Bold);

    notificareNume = new sf::Text(font, "", 22);
    notificareNume->setFillColor(sf::Color::White);

    titluMeniu = new sf::Text(font, "REALIZARI & PROGRES", 40);
    titluMeniu->setFillColor(sf::Color::Black);

    sf::FloatRect b = titluMeniu->getLocalBounds();
    titluMeniu->setOrigin({b.size.x / 2.f, 0.f});
    titluMeniu->setPosition({Config::LATIME_FEREASTRA / 2.f, 30.f});

    if (bufferSunet.loadFromFile(Config::CALE_SUNET_ACHIEVEMENT)) {
        sunetUnlock = new sf::Sound(bufferSunet);
        sunetUnlock->setVolume(100.f);
        sunetUnlock->setPitch(1.5f);
    }

    butonMeniu = new ButonMeniu({Config::LATIME_FEREASTRA / 2.f - 100.f, 700.f}, font);
    butonReset = new ButonResetAchievements({Config::LATIME_FEREASTRA - 160.f, 20.f}, font);
}

void ManagerAchievements::verificaConditii(int scor, int combo, int wave, int cuvinteTotale, float acuratete, int hp, bool victorie, int dificultateEnum) {
    if (victorie && !listaAchievements[0].esteMaxat()) {
        int dificultateCeruta = listaAchievements[0].getUrmatorulPrag();
        if (dificultateEnum == dificultateCeruta) {
            deblocheaza(0);
        }
    }

    if (!listaAchievements[1].esteMaxat() && combo >= listaAchievements[1].getUrmatorulPrag()) {
        deblocheaza(1);
    }

    if (!listaAchievements[2].esteMaxat()) {
        int prag = listaAchievements[2].getUrmatorulPrag();
        if (acuratete >= 99.9f && cuvinteTotale >= prag) {
            deblocheaza(2);
        }
    }

    if (!listaAchievements[3].esteMaxat() && wave >= listaAchievements[3].getUrmatorulPrag()) {
        deblocheaza(3);
    }

    if (!listaAchievements[4].esteMaxat()) {
        int prag = listaAchievements[4].getUrmatorulPrag();
        if (scor >= prag && hp == 100) {
            deblocheaza(4);
        }
    }
}

void ManagerAchievements::deblocheaza(int index) {
    if (index < 0 || static_cast<size_t>(index) >= listaAchievements.size()) return;

    Achievement& ach = listaAchievements[index];
    ach.nivelCurent++;
    salveazaProgres();

    afiseazaNotificare = true;
    timpAfisareNotificare = 3.5f;

    if (notificareNume && notificareTitlu) {
        notificareNume->setString(ach.nume + " (Lvl " + std::to_string(ach.nivelCurent) + ")");

        sf::FloatRect bTitle = notificareTitlu->getLocalBounds();
        notificareTitlu->setPosition({(Config::LATIME_FEREASTRA - bTitle.size.x) / 2.f, 25.f});

        sf::FloatRect bName = notificareNume->getLocalBounds();
        notificareNume->setPosition({(Config::LATIME_FEREASTRA - bName.size.x) / 2.f, 50.f});
    }

    notificareBg.setPosition({(Config::LATIME_FEREASTRA - 400.f) / 2.f, 10.f});
    notificareBg.setOutlineColor(getCuloareNivel(ach.nivelCurent));

    if (sunetUnlock) {
        sunetUnlock->setPitch(1.0f + (0.1f * static_cast<float>(ach.nivelCurent)));
        sunetUnlock->play();
    }
}

void ManagerAchievements::actualizeaza(float dt, sf::Vector2f mousePos) {
    if (afiseazaNotificare) {
        timpAfisareNotificare -= dt;
        if (timpAfisareNotificare <= 0.f) {
            afiseazaNotificare = false;
        }
    }

    if (butonMeniu) butonMeniu->updateCuloare(mousePos);
    if (butonReset) butonReset->updateCuloare(mousePos);
}

void ManagerAchievements::deseneazaNotificare(sf::RenderWindow& window) const {
    if (!afiseazaNotificare || !notificareTitlu || !notificareNume) return;

    window.draw(notificareBg);
    window.draw(*notificareTitlu);
    window.draw(*notificareNume);
}

void ManagerAchievements::deseneazaMeniu(sf::RenderWindow& window) const {
    if (titluMeniu) window.draw(*titluMeniu);

    float startY = 100.f;
    for (const auto& ach : listaAchievements) {
        sf::RectangleShape cutie({520.f, 85.f});
        cutie.setOrigin({260.f, 0.f});
        cutie.setPosition({Config::LATIME_FEREASTRA / 2.f, startY});

        sf::Color culoareNivel = getCuloareNivel(ach.nivelCurent);
        if (ach.nivelCurent == 0) culoareNivel = sf::Color(100, 100, 100);

        if (ach.nivelCurent > 0) {
            cutie.setFillColor(sf::Color(245, 245, 245));
            cutie.setOutlineColor(culoareNivel);
            cutie.setOutlineThickness(3.f);
        } else {
            cutie.setFillColor(sf::Color(200, 200, 200));
            cutie.setOutlineColor(sf::Color(80, 80, 80));
            cutie.setOutlineThickness(1.f);
        }

        window.draw(cutie);

        if (fontRef) {
            std::string statusText = ach.nume;
            if (ach.nivelCurent > 0) {
                statusText += " [Lv. " + std::to_string(ach.nivelCurent) + "/" + std::to_string(ach.praguri.size()) + "]";
            } else {
                statusText += " [LOCKED]";
            }

            sf::Text txtNume(*fontRef, statusText, 22);
            txtNume.setFillColor(ach.nivelCurent > 0 ? sf::Color::Black : sf::Color(80, 80, 80));
            txtNume.setStyle(sf::Text::Bold);
            txtNume.setPosition({cutie.getPosition().x - 240.f, startY + 10.f});
            window.draw(txtNume);

            std::string descText = ach.descriereBase;
            if (ach.esteMaxat()) {
                descText = "COMPLETAT!";
                if (ach.id == "campion") {
                    descText += " (GREU)";
                } else {
                    descText += " (" + ach.descriereBase + " " + std::to_string(ach.praguri.back()) + ")";
                }
            } else {
                if (ach.id == "campion") {
                    int dif = ach.getUrmatorulPrag();
                    std::string numeDif = (dif == 0) ? "USOR" : ((dif == 1) ? "MEDIU" : "GREU");
                    descText += " " + numeDif;
                } else {
                    descText += " " + std::to_string(ach.getUrmatorulPrag());
                }

                if (ach.nivelCurent > 0) descText += " (Urmatorul)";
            }

            sf::Text txtDesc(*fontRef, descText, 18);
            txtDesc.setFillColor(sf::Color(60, 60, 60));
            txtDesc.setPosition({cutie.getPosition().x - 240.f, startY + 45.f});
            window.draw(txtDesc);
        }

        sf::CircleShape icon(25.f);
        icon.setPosition({cutie.getPosition().x + 200.f, startY + 18.f});
        icon.setFillColor(culoareNivel);
        icon.setOutlineThickness(2.f);
        icon.setOutlineColor(sf::Color::Black);
        window.draw(icon);

        if (fontRef && ach.nivelCurent > 0) {
            sf::Text txtLvl(*fontRef, std::to_string(ach.nivelCurent), 20);
            txtLvl.setFillColor(sf::Color::White);
            txtLvl.setOutlineColor(sf::Color::Black);
            txtLvl.setOutlineThickness(1.f);
            sf::FloatRect b = txtLvl.getLocalBounds();
            txtLvl.setOrigin({b.size.x/2.f + b.position.x, b.size.y/2.f + b.position.y});
            txtLvl.setPosition({icon.getPosition().x + 25.f, icon.getPosition().y + 25.f});
            window.draw(txtLvl);
        }

        startY += 100.f;
    }

    if (butonMeniu) butonMeniu->afiseaza(window);
    if (butonReset) butonReset->afiseaza(window);
}

bool ManagerAchievements::aDatClickMeniu(sf::Vector2f mousePos) const {
    if (butonMeniu) return butonMeniu->esteMouseDeasupra(mousePos);
    return false;
}

bool ManagerAchievements::aDatClickReset(sf::Vector2f mousePos) const {
    if (butonReset) return butonReset->esteMouseDeasupra(mousePos);
    return false;
}

void ManagerAchievements::salveazaProgres() const {
    std::ofstream fout("achievements.data");
    if (fout.is_open()) {
        for (const auto& ach : listaAchievements) {
            fout << ach.nivelCurent << "\n";
        }
    }
}

void ManagerAchievements::incarcaProgres() {
    std::ifstream fin("achievements.data");
    if (!fin.is_open()) return;

    int lvl;
    for (auto& ach : listaAchievements) {
        if (fin >> lvl) {
            ach.nivelCurent = lvl;
            if (ach.nivelCurent > static_cast<int>(ach.praguri.size())) {
                ach.nivelCurent = static_cast<int>(ach.praguri.size());
            }
        }
    }
}

void ManagerAchievements::reseteazaProgres() {
    for (auto& ach : listaAchievements) {
        ach.nivelCurent = 0;
    }
    salveazaProgres();
    std::cout << "[Achievements] Progres resetat la 0!" << std::endl;
}