#include "Cuvant.hpp"
#include "Buton.hpp"
#include <fstream>
#include <random>

Cuvant::Cuvant()
    // Fontul marit initial la 40
    : font(), textHelper(font, "", 40), asteaptaSpawn(false)
{
    linieRosie.setSize({700.f, 10.f});
    linieRosie.setFillColor(sf::Color(200, 50, 50, 150));
    linieRosie.setPosition({0.f, 750.f});
}

void Cuvant::initializeaza(const sf::Font& fontIncarcat)
{
    font = fontIncarcat;
    textHelper.setFont(font);
    // MODIFICARE: Font mai mare
    textHelper.setCharacterSize(40);

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
        if (!fin.is_open()) return;
        std::string cuv;
        while (fin >> cuv) dest.push_back(cuv);
    };

    cuvinteScurte.clear(); cuvinteMedii.clear(); cuvinteLungi.clear();
    incarca("Date/cuvinte_scurte.txt", cuvinteScurte);
    incarca("Date/cuvinte_medii.txt", cuvinteMedii);
    incarca("Date/cuvinte_lungi.txt", cuvinteLungi);

    if (cuvinteScurte.empty()) cuvinteScurte.emplace_back("scurt");
    if (cuvinteMedii.empty()) cuvinteMedii.emplace_back("mediu");
    if (cuvinteLungi.empty()) cuvinteLungi.emplace_back("lungime");
}

std::string Cuvant::extrageCuvantAleatoriu(DificultateJoc dificultate)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> procent(1, 100);
    int p = procent(gen);

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
    std::uniform_int_distribution<> rand_index(0, static_cast<int>(sursa->size()) - 1);
    return (*sursa)[rand_index(gen)];
}

void Cuvant::spawneazaGrup(float latimeEcran, DificultateJoc dificultate)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    int numarCuvinte = (std::uniform_int_distribution<>(0, 1)(gen) == 0) ? 2 : 3;

    float spatiuDisponibil = latimeEcran - 100.f;
    float pas = spatiuDisponibil / static_cast<float>(numarCuvinte);

    for (int i = 0; i < numarCuvinte; ++i) {
        float startX = 50.f;
        std::string textAles = extrageCuvantAleatoriu(dificultate);

        float bazaViteza = 50.f;
        if (dificultate == DificultateJoc::Mediu) bazaViteza = 80.f;
        if (dificultate == DificultateJoc::Greu) bazaViteza = 120.f;
        float vitezaFinala = bazaViteza + static_cast<float>(std::uniform_int_distribution<>(0, 20)(gen));

        float xPos = startX + static_cast<float>(i) * pas + static_cast<float>(std::uniform_int_distribution<>(-20, 20)(gen));

        // MODIFICARE: Spawn intre 10% (80px) si 50% (400px) din ecran (presupunand h=800)
        // 10% din 800 = 80, 50% din 800 = 400.
        // Pentru siguranta, sa nu intre in textul scorului, incepem de la 100.
        float yPos = static_cast<float>(std::uniform_int_distribution<>(100, 350)(gen));

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

// MODIFICARE: Returneaza int (damage)
int Cuvant::actualizeaza(float dt, DificultateJoc dificultate)
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
            // A lovit linia rosie!
            damage += 1; // Contorizam cuvantul pierdut
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
        char caracterTastat = static_cast<char>(textEv->unicode);

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
                if (!cuv.text.empty() && cuv.text[0] == caracterTastat) {
                    if (cuv.y > maxY) {
                        maxY = cuv.y;
                        tinta = &cuv;
                    }
                }
            }
        }

        if (tinta) {
            if (tinta->text[tinta->indexTastat] == caracterTastat) {
                tinta->indexTastat++;

                if (tinta->indexTastat >= tinta->text.size()) {
                    tinta->finalizat = true;

                    // MODIFICARE: Calcul Scor = 10 * dificultate
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
    }
}