#include "Cuvant.hpp"
#include "Buton.hpp"
#include "Static.hpp"
#include "Exceptii.hpp"
#include <fstream>

Cuvant::Cuvant()
    : font(), textHelper(font, "", 30), asteaptaSpawn(false)
{
    linieRosie.setSize({static_cast<float>(Config::LATIME_FEREASTRA), 60.f});
    linieRosie.setFillColor(sf::Color(255, 50, 50, 200));
    linieRosie.setPosition({0.f, static_cast<float>(Config::INALTIME_FEREASTRA) - 50.f});
    cursor.setSize({15.f, 3.f});
    cursor.setFillColor(sf::Color::Black);
}

void Cuvant::initializeaza(const sf::Font& fontIncarcat)
{
    font = fontIncarcat;
    textHelper.setFont(font);
    textHelper.setCharacterSize(30);

    incarcaDictionare();
}

void Cuvant::reseteaza() {
    cuvinteActive.clear();
    ceasSpawn.restart();
    asteaptaSpawn = false;
}

void Cuvant::incarcaDictionare()
{
    auto incarca = [](const std::string& path, std::vector<std::string>& dest) {
        std::ifstream fin(path);

        if (!fin.is_open()) {
            throw EroareResursaCritica(path);
        }

        std::string cuv;
        while (fin >> cuv) dest.push_back(cuv);

        if (dest.empty()) {
            throw EroareDateInvalide("Fisierul " + path + " este gol!");
        }
    };

    cuvinteScurte.clear(); cuvinteMedii.clear(); cuvinteLungi.clear();

    incarca("Date/cuvinte_scurte.txt", cuvinteScurte);
    incarca("Date/cuvinte_medii.txt", cuvinteMedii);
    incarca("Date/cuvinte_lungi.txt", cuvinteLungi);
}

std::string Cuvant::extrageCuvantAleatoriu(DificultateJoc dificultate)
{
    int p = Random::getInt(1, 100);

    std::vector<std::string>* sursa = &cuvinteScurte;

    if (dificultate == DificultateJoc::Usor) {
        if (p > 60 && p <= 90) sursa = &cuvinteMedii;
        else if (p > 90) sursa = &cuvinteLungi;
    }
    else if (dificultate == DificultateJoc::Mediu) {
        if (p > 33 && p <= 66) sursa = &cuvinteMedii;
        else if (p > 66) sursa = &cuvinteLungi;
    }
    else {
        if (p <= 30) sursa = &cuvinteMedii;
        else sursa = &cuvinteLungi;
    }

    if (sursa->empty()) return "gol";

    const int index = Random::getInt(0, static_cast<int>(sursa->size()) - 1);
    return (*sursa)[index];
}

void Cuvant::spawneazaGrup(float latimeEcran, DificultateJoc dificultate)
{
    const int numarCuvinte = (Random::getInt(0, 1) == 0) ? 2 : 3;

    const float spatiuDisponibil = latimeEcran - 100.f;
    const float pas = spatiuDisponibil / static_cast<float>(numarCuvinte);

    for (int i = 0; i < numarCuvinte; ++i) {
        constexpr float startX = 50.f;
        const std::string textAles = extrageCuvantAleatoriu(dificultate);

        float bazaViteza = 50.f;
        if (dificultate == DificultateJoc::Mediu) bazaViteza = 80.f;
        if (dificultate == DificultateJoc::Greu) bazaViteza = 120.f;

        const float vitezaFinala = bazaViteza + static_cast<float>(Random::getInt(0, 20));

        const float xPos = startX + static_cast<float>(i) * pas + static_cast<float>(Random::getInt(-20, 20));

        const auto yPos = static_cast<float>(Random::getInt(100, 300));

        CuvantActiv nou;
        nou.text = textAles;
        nou.indexTastat = 0;
        nou.x = xPos;
        nou.y = yPos;
        nou.viteza = vitezaFinala;
        nou.finalizat = false;

        cuvinteActive.push_back(nou);
    }
}

int Cuvant::actualizeaza(const float dt, const DificultateJoc dificultate)
{
    int damage = 0;

    if (cuvinteActive.empty()) {
        if (!asteaptaSpawn) {
            ceasSpawn.restart();
            asteaptaSpawn = true;
        }
        else {
            if (ceasSpawn.getElapsedTime().asSeconds() > 1.0f) {
                spawneazaGrup(700.f, dificultate);
                asteaptaSpawn = false;
            }
        }
    }

    float yLimitaRosie = 750.f;

    auto it = cuvinteActive.begin();
    while (it != cuvinteActive.end()) {
        if (it->finalizat) {
            it = cuvinteActive.erase(it);
            continue;
        }

        it->y += it->viteza * dt;

        if (it->y > yLimitaRosie) {
            damage += 1;
            it = cuvinteActive.erase(it);
        }
        else {
            ++it;
        }
    }

    return damage;
}

void Cuvant::gestioneazaEvenimente(const sf::Event& event, Scor& scor_ref, DificultateJoc dificultate)
{
    if (auto textEv = event.getIf<sf::Event::TextEntered>()) {
        char caracterTastat = static_cast<char>(std::tolower(static_cast<int>(textEv->unicode)));

        if (caracterTastat < 32) return;

        CuvantActiv* tinta = nullptr;

        for (auto& cuv : cuvinteActive) {
            if (cuv.indexTastat > 0 && !cuv.finalizat) {
                tinta = &cuv;
                break;
            }
        }

        if (!tinta) {
            float maxY = -1000.f;
            for (auto& cuv : cuvinteActive) {
                if (cuv.finalizat) continue;
                if (!cuv.text.empty()) {
                    char primaLiteraCuvant = static_cast<char>(std::tolower(static_cast<int>(cuv.text[0])));

                    if (primaLiteraCuvant == caracterTastat) {
                        if (cuv.y > maxY) {
                            maxY = cuv.y;
                            tinta = &cuv;
                        }
                    }
                }
            }
        }

        if (tinta) {
            char literaAsteptata = static_cast<char>(std::tolower(static_cast<int>(tinta->text[tinta->indexTastat])));

            if (literaAsteptata == caracterTastat) {
                tinta->indexTastat++;

                if (tinta->indexTastat >= tinta->text.size()) {
                    tinta->finalizat = true;

                    int multiplicator = 1;
                    if (dificultate == DificultateJoc::Mediu) multiplicator = 2;
                    if (dificultate == DificultateJoc::Greu) multiplicator = 3;

                    scor_ref.adauga(10 * multiplicator);
                }
            }
        }
    }
}

void Cuvant::afiseaza(sf::RenderWindow& window)
{
    window.draw(linieRosie);

    for (const auto& cuv : cuvinteActive) {

        textHelper.setString(cuv.text);
        textHelper.setPosition({ cuv.x, cuv.y });

        if (cuv.indexTastat > 0) {
            textHelper.setFillColor(sf::Color(255, 165, 0));
        }
        else {
            textHelper.setFillColor(sf::Color::Red);
        }

        window.draw(textHelper);

        if (!cuv.finalizat && cuv.indexTastat < cuv.text.size()) {
            sf::Vector2f pozitieLitera = textHelper.findCharacterPos(cuv.indexTastat);

            cursor.setPosition({pozitieLitera.x, pozitieLitera.y + 35.f});
            window.draw(cursor);
        }
    }
}