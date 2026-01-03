#include "Achievements.hpp"
#include "Static.hpp"
#include "Buton.hpp"
#include <fstream>

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
    notificareBg.setSize({300.f, 80.f});
    notificareBg.setFillColor(sf::Color(30, 30, 30, 230));
    notificareBg.setOutlineThickness(2.f);
    notificareBg.setOutlineColor(sf::Color::White);
    notificareBg.setPosition({(Config::LATIME_FEREASTRA - 300.f) / 2.f, -100.f});

    listaAchievements = {
        {"first_blood", "Incepator", "Distruge primul tau cuvant.", false, sf::Color(205, 127, 50)},
        {"combo_master", "Combo Master", "Atinge combo maxim (4x).", false, sf::Color(192, 192, 192)},
        {"sniper", "Sniper", "Ai 100% acuratete (min. 20 taste).", false, sf::Color(255, 215, 0)},
        {"survivor", "Supravietuitor", "Ajungi la Wave 5 in Endless.", false, sf::Color(255, 69, 0)},
        {"invincibil", "Invincibil", "Fa 500 scor fara sa pierzi HP.", false, sf::Color(0, 255, 255)}
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

    notificareTitlu = new sf::Text(font, "ACHIEVEMENT UNLOCKED!", 14);
    notificareTitlu->setFillColor(sf::Color::Yellow);

    notificareNume = new sf::Text(font, "", 20);
    notificareNume->setFillColor(sf::Color::White);
    notificareNume->setStyle(sf::Text::Bold);

    titluMeniu = new sf::Text(font, "REALIZARI", 40);
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

void ManagerAchievements::verificaConditii(int scor, int combo, int wave, int cuvinteTotale, float acuratete, int hp) {
    if (cuvinteTotale >= 1 && !listaAchievements[0].deblocat) deblocheaza(0);
    if (combo >= 4 && !listaAchievements[1].deblocat) deblocheaza(1);
    if (cuvinteTotale >= 5 && acuratete >= 99.9f && !listaAchievements[2].deblocat) deblocheaza(2);
    if (wave >= 5 && !listaAchievements[3].deblocat) deblocheaza(3);
    if (scor >= 500 && hp == 100 && !listaAchievements[4].deblocat) deblocheaza(4);
}

void ManagerAchievements::deblocheaza(int index) {
    if (index < 0 || static_cast<size_t>(index) >= listaAchievements.size()) return;

    listaAchievements[index].deblocat = true;
    salveazaProgres();

    afiseazaNotificare = true;
    timpAfisareNotificare = 3.0f;

    if (notificareNume && notificareTitlu) {
        notificareNume->setString(listaAchievements[index].nume);

        sf::FloatRect bTitle = notificareTitlu->getLocalBounds();
        notificareTitlu->setPosition({(Config::LATIME_FEREASTRA - bTitle.size.x) / 2.f, 25.f});

        sf::FloatRect bName = notificareNume->getLocalBounds();
        notificareNume->setPosition({(Config::LATIME_FEREASTRA - bName.size.x) / 2.f, 45.f});
    }

    notificareBg.setPosition({(Config::LATIME_FEREASTRA - 300.f) / 2.f, 10.f});

    if (sunetUnlock) sunetUnlock->play();
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
        sf::RectangleShape cutie({500.f, 80.f});
        cutie.setOrigin({250.f, 0.f});
        cutie.setPosition({Config::LATIME_FEREASTRA / 2.f, startY});

        if (ach.deblocat) {
            cutie.setFillColor(sf::Color(240, 240, 240));
            cutie.setOutlineColor(ach.culoare);
            cutie.setOutlineThickness(3.f);
        } else {
            cutie.setFillColor(sf::Color(200, 200, 200));
            cutie.setOutlineColor(sf::Color(100, 100, 100));
            cutie.setOutlineThickness(1.f);
        }

        window.draw(cutie);

        if (fontRef) {
            sf::Text txtNume(*fontRef, ach.nume + (ach.deblocat ? " [DEBLOCAT]" : " [LOCKED]"), 22);
            txtNume.setFillColor(ach.deblocat ? ach.culoare : sf::Color(100, 100, 100));
            txtNume.setPosition({cutie.getPosition().x - 230.f, startY + 10.f});
            window.draw(txtNume);

            sf::Text txtDesc(*fontRef, ach.descriere, 16);
            txtDesc.setFillColor(sf::Color::Black);
            txtDesc.setPosition({cutie.getPosition().x - 230.f, startY + 45.f});
            window.draw(txtDesc);
        }

        sf::CircleShape icon(20.f);
        icon.setPosition({cutie.getPosition().x + 200.f, startY + 20.f});
        icon.setFillColor(ach.deblocat ? ach.culoare : sf::Color(128, 128, 128));
        window.draw(icon);

        startY += 95.f;
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
            fout << (ach.deblocat ? 1 : 0) << "\n";
        }
    }
}

void ManagerAchievements::incarcaProgres() {
    std::ifstream fin("achievements.data");
    if (!fin.is_open()) return;

    int status;
    for (auto& ach : listaAchievements) {
        if (fin >> status) {
            ach.deblocat = (status == 1);
        }
    }
}

void ManagerAchievements::reseteazaProgres() {
    for (auto& ach : listaAchievements) {
        ach.deblocat = false;
    }
    salveazaProgres();
    std::cout << "[Achievements] Progres resetat!" << std::endl;
}