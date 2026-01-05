#include "Cuvant.hpp"
#include "Buton.hpp"
#include "Static.hpp"
#include "Exceptii.hpp"
#include <fstream>
#include "Efecte.hpp"


Cuvant::Cuvant()
    : font(), textHelper(font, "", 30), asteaptaSpawn(false),valCurentValid(false)
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
    valCurentValid = false;
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

void Cuvant::spawneazaGrup(float latimeEcran, DificultateJoc dificultate, int waveIndex)
{
    const int numarCuvinte = (Random::getInt(0, 1) == 0) ? 2 : 3;
    const float spatiuDisponibil = latimeEcran - 100.f;
    const float pas = spatiuDisponibil / static_cast<float>(numarCuvinte);

    const bool areSpecial = (Random::getInt(0, 1) == 1);
    const int indexSpecial = areSpecial ? Random::getInt(0, numarCuvinte - 1) : -1;

    for (int i = 0; i < numarCuvinte; ++i) {
        constexpr float startX = 50.f;
        const std::string textAles = extrageCuvantAleatoriu(dificultate);

        TipCuvant tipCurent = TipCuvant::Normal;

        if (Random::getInt(1, 100) <= 10) {
            tipCurent = TipCuvant::Capcana;
        }
        else if (i == indexSpecial) {
            tipCurent = CuvantSpecial::genereazaTipAleatoriu();
            if (dificultate == DificultateJoc::Endless && tipCurent == TipCuvant::BonusTimp) {
                tipCurent = TipCuvant::Normal;
            }
        }

        float vitezaFinala = 0.f;

        if (tipCurent == TipCuvant::Capcana) {
            vitezaFinala = 200.f;
        }
        else {
            float bazaViteza = 50.f;
            if (dificultate == DificultateJoc::Mediu) bazaViteza = 80.f;
            if (dificultate == DificultateJoc::Greu) bazaViteza = 120.f;

            if (dificultate == DificultateJoc::Endless) {
                bazaViteza = 60.f;
                const int treaptaViteza = waveIndex / 5;
                const float bonusViteza = static_cast<float>(treaptaViteza) * 20.f;
                bazaViteza += bonusViteza;
            }

            vitezaFinala = bazaViteza + static_cast<float>(Random::getInt(0, 20));
        }

        const float xPos = startX + static_cast<float>(i) * pas + static_cast<float>(Random::getInt(-20, 20));
        const auto yPos = static_cast<float>(Random::getInt(50, 300));

        CuvantActiv nou;
        nou.text = textAles;
        nou.indexTastat = 0;
        nou.x = xPos;
        nou.y = yPos;
        nou.viteza = vitezaFinala;
        nou.finalizat = false;
        nou.tip = tipCurent;
        nou.timpRamaneOverlay = 0.0f;

        cuvinteActive.push_back(nou);
    }
}

int Cuvant::actualizeaza(const float dt, const DificultateJoc dificultate, Scor& scor, int waveIndex)
{
    int damage = 0;
    float yLimitaRosie = static_cast<float>(Config::INALTIME_FEREASTRA) - 50.f;

    if (cuvinteActive.empty()) {
        if (!asteaptaSpawn) {
            if (valCurentValid) {
                scor.cresteCombo();
            }
            valCurentValid = true;
            ceasSpawn.restart();
            asteaptaSpawn = true;
        }
        else {
            if (ceasSpawn.getElapsedTime().asSeconds() > 1.0f) {
                spawneazaGrup(static_cast<float>(Config::LATIME_FEREASTRA), dificultate, waveIndex);
                asteaptaSpawn = false;
                grupNouGenerat = true;
            }
        }
    }

    auto it = cuvinteActive.begin();
    while (it != cuvinteActive.end()) {

        if (it->finalizat) {
            it->timpRamaneOverlay -= dt;
            if (it->timpRamaneOverlay <= 0.0f) {
                it = cuvinteActive.erase(it);
                continue;
            }
            ++it;
            continue;
        }

        it->y += it->viteza * dt;

        if (it->y > yLimitaRosie) {

            if (it->tip != TipCuvant::Capcana) {
                damage++;
                valCurentValid = false;
                scor.resetCombo();
            }

            it = cuvinteActive.erase(it);
        }
        else {
            ++it;
        }
    }
    return damage;
}

TipCuvant Cuvant::gestioneazaEvenimente(const sf::Event& event, Scor& scor_ref, DificultateJoc dificultate, int& tasteCorecteRef, ManagerEfecte& efecteRef)
{
    auto textEv = event.getIf<sf::Event::TextEntered>();
    if (!textEv) return TipCuvant::Niciunul;

    char caracterTastat = static_cast<char>(std::tolower(static_cast<int>(textEv->unicode)));
    if (caracterTastat < 32) return TipCuvant::Niciunul;

    CuvantActiv* tinta = gasesteTinta(caracterTastat);
    if (!tinta) {
        valCurentValid = false;
        scor_ref.resetCombo();
        return TipCuvant::Niciunul;
    }

    return proceseazaTinta(tinta, caracterTastat, scor_ref, dificultate, tasteCorecteRef, efecteRef);
}

CuvantActiv* Cuvant::gasesteTinta(char caracterTastat) {

    for (auto& cuv : cuvinteActive) {
        if (cuv.indexTastat > 0 && !cuv.finalizat) {
            return &cuv;
        }
    }

    CuvantActiv* candidatSafe = nullptr;
    CuvantActiv* candidatCapcana = nullptr;

    float maxY_Safe = -1000.f;
    float maxY_Capcana = -1000.f;

    for (auto& cuv : cuvinteActive) {
        if (cuv.finalizat) continue;

        if (!cuv.text.empty() && static_cast<char>(std::tolower(static_cast<int>(cuv.text[0]))) == caracterTastat) {
            if (cuv.tip == TipCuvant::Capcana) {
                if (cuv.y > maxY_Capcana) {
                    maxY_Capcana = cuv.y;
                    candidatCapcana = &cuv;
                }
            } else {
                if (cuv.y > maxY_Safe) {
                    maxY_Safe = cuv.y;
                    candidatSafe = &cuv;
                }
            }
        }
    }

    if (candidatSafe) return candidatSafe;
    if (candidatCapcana) return candidatCapcana;

    return nullptr;
}

TipCuvant Cuvant::proceseazaTinta(CuvantActiv* tinta, char caracterTastat, Scor& scor_ref, DificultateJoc dificultate, int& tasteCorecteRef, ManagerEfecte& efecteRef) {

    char asteptat = static_cast<char>(std::tolower(static_cast<int>(tinta->text[tinta->indexTastat])));
    if (caracterTastat == asteptat) {
        if (tinta->tip == TipCuvant::Capcana) {
            sf::FloatRect bounds = textHelper.getGlobalBounds();
            sf::Vector2f centruCuvant = {tinta->x + bounds.size.x/2.f, tinta->y + bounds.size.y/2.f};

            efecteRef.spawnExplozieCuvant(centruCuvant, sf::Color(50, 50, 50));
            efecteRef.playBoom();
            scor_ref.resetCombo();

            valCurentValid = false;
            tinta->finalizat = true;
            tinta->timpRamaneOverlay = 0.0f;
            return TipCuvant::Capcana;
        }
        tasteCorecteRef++;

        textHelper.setString(tinta->text);
        textHelper.setPosition({tinta->x, tinta->y});
        sf::Vector2f pozLitera = textHelper.findCharacterPos(tinta->indexTastat);
        pozLitera.x += 10.f;
        pozLitera.y += 20.f;
        efecteRef.spawnLiteraCorecta(pozLitera);
        efecteRef.playTasta();

        if (tinta->tip == TipCuvant::BonusInstant) {
            tinta->finalizat = true;
        } else {
            tinta->indexTastat++;
        }

        if (tinta->finalizat || tinta->indexTastat >= tinta->text.size()) {
            tinta->finalizat = true;
            tinta->timpRamaneOverlay = 0.5f;

            const sf::FloatRect bounds = textHelper.getGlobalBounds();
            const sf::Vector2f centruCuvant = {
                tinta->x + (bounds.size.x / 2.f),
                tinta->y + (bounds.size.y / 2.f)
            };

            sf::Color culoareExplozie = CuvantSpecial::getCuloare(tinta->tip);
            if (tinta->tip == TipCuvant::Normal) culoareExplozie = sf::Color(255, 100, 50);

            efecteRef.playBoom();
            efecteRef.spawnExplozieCuvant(centruCuvant, culoareExplozie);

            constexpr int puncteBaza = 10;
            const int multiplicatorDificultate = (dificultate == DificultateJoc::Greu) ? 3 : (dificultate == DificultateJoc::Mediu ? 2 : 1);
            const int puncteTotale = puncteBaza * multiplicatorDificultate;
            if (tinta->tip == TipCuvant::BonusTripluScor) {
                scor_ref.adauga(puncteTotale * 3);
            }
            else if (tinta->tip == TipCuvant::BonusComboMax) {
                scor_ref.setMultiplicator(3);
                scor_ref.adauga(puncteTotale);
            }
            else {
                scor_ref.adauga(puncteTotale);
            }
            return tinta->tip;
        }
    } else {
        valCurentValid = false;
        scor_ref.resetCombo();
    }

    return TipCuvant::Niciunul;
}

void Cuvant::afiseaza(sf::RenderWindow& window)
{
    window.draw(linieRosie);

    for (const auto& cuv : cuvinteActive) {


        if (cuv.finalizat) {
            sf::Color culoareFade = sf::Color(255, 215, 0);

            float alphaRatio = cuv.timpRamaneOverlay / 0.5f;
            culoareFade.a = static_cast<unsigned char>(255.0f * alphaRatio);
            textHelper.setFillColor(culoareFade);

            float ridicare = (0.5f - cuv.timpRamaneOverlay) * 50.0f;

            textHelper.setString(cuv.text);
            textHelper.setPosition({ cuv.x, cuv.y - ridicare });

        }
        else {
            textHelper.setString(cuv.text);
            textHelper.setPosition({ cuv.x, cuv.y });

            if (cuv.indexTastat > 0) {
                textHelper.setFillColor(sf::Color(255, 165, 0));
            } else {
                textHelper.setFillColor(CuvantSpecial::getCuloare(cuv.tip));
            }
        }

        window.draw(textHelper);

        if (!cuv.finalizat && cuv.indexTastat < cuv.text.size()) {
            sf::Vector2f pozitieLitera = textHelper.findCharacterPos(cuv.indexTastat);
            cursor.setPosition({pozitieLitera.x, pozitieLitera.y + 35.f});
            window.draw(cursor);
        }
    }
}

bool Cuvant::aGeneratGrupNou() {
    if (grupNouGenerat) {
        grupNouGenerat = false;
        return true;
    }
    return false;
}