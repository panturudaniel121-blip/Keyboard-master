#include "Efecte.hpp"
#include "Static.hpp"
#include <cmath>
#include <iostream>

ManagerEfecte::ManagerEfecte()
    : bufferTasta(), bufferBoom(), bufferWin(), sunetBoom(bufferBoom), sunetWin(bufferWin)
{
    shapeParticula.setSize({5.f, 5.f});
    shapeParticula.setOrigin({2.5f, 2.5f});

    indexSunetCurent = 0;
}

void ManagerEfecte::incarcaSunete() {
    if (!bufferTasta.loadFromFile(Config::CALE_SUNET_TASTA)) {
        std::cerr << "[Eroare] Sunet tasta lipsa: " << Config::CALE_SUNET_TASTA << "\n";
    } else {
        poolSuneteTasta.clear();
        for (int i = 0; i < 10; ++i) {
            sf::Sound s(bufferTasta);
            s.setVolume(50.f);
            poolSuneteTasta.push_back(s);
        }
    }
    if (!bufferBoom.loadFromFile(Config::CALE_SUNET_BOOM)) {
        std::cerr << "[Eroare] Sunet boom lipsa: " << Config::CALE_SUNET_BOOM << "\n";
    } else {
        sunetBoom.setBuffer(bufferBoom);
        sunetBoom.setVolume(70.f);
    }

    if (!bufferWin.loadFromFile(Config::CALE_SUNET_WIN)) {
        std::cerr << "[Eroare] Sunet win lipsa: " << Config::CALE_SUNET_WIN << "\n";
    } else {
        sunetWin.setBuffer(bufferWin);
        sunetWin.setVolume(80.f);
    }
}
void ManagerEfecte::actualizeaza(float dt) {
    auto it = particule.begin();
    while (it != particule.end()) {
        it->durataViata -= dt;
        if (it->durataViata <= 0) {
            it = particule.erase(it);
        } else {
            it->pozitie += it->viteza * dt;
            it->viteza.y += 200.f * dt;
            ++it;
        }
    }
}

void ManagerEfecte::deseneaza(sf::RenderWindow& window) {
    for (const auto& p : particule) {
        shapeParticula.setPosition(p.pozitie);
        float alphaRatio = p.durataViata / p.durataMaxima;
        sf::Color col = p.culoareStart;

        col.a = static_cast<unsigned char>(255.0f * (0.2f + 0.8f * alphaRatio));

        shapeParticula.setFillColor(col);
        shapeParticula.setScale({p.marime, p.marime});
        window.draw(shapeParticula);
    }
}

void ManagerEfecte::spawnLiteraCorecta(sf::Vector2f pos) {
    for (int i = 0; i < 5; ++i) {
        Particula p;
        p.pozitie = pos;
        float unghi = Random::getFloat(0.f, 360.f) * 3.14159f / 180.f;
        float viteza = Random::getFloat(50.f, 150.f);
        p.viteza = {static_cast<float>(std::cos(unghi)) * viteza, static_cast<float>(std::sin(unghi)) * viteza - 100.f};
        p.durataViata = p.durataMaxima = Random::getFloat(0.3f, 0.6f);
        p.culoareStart = sf::Color(255, 215, 0);
        p.marime = 1.0f;
        particule.push_back(p);
    }
}

void ManagerEfecte::spawnExplozieCuvant(sf::Vector2f pos, sf::Color culoareBaza) {
    for (int i = 0; i < 30; ++i) {
        Particula p;
        p.pozitie = pos;
        float unghi = Random::getFloat(0.f, 360.f) * 3.14159f / 180.f;
        float viteza = Random::getFloat(150.f, 400.f);
        p.viteza = {static_cast<float>(std::cos(unghi)) * viteza, static_cast<float>(std::sin(unghi)) * viteza};
        p.durataViata = p.durataMaxima = Random::getFloat(0.5f, 1.0f);
        p.culoareStart = culoareBaza;
        p.marime = Random::getFloat(1.5f, 3.0f);
        particule.push_back(p);
    }
}

void ManagerEfecte::spawnConfetti() {
    for (int i = 0; i < 300; ++i) {
        Particula p;
        float startX = Random::getFloat(0.f, static_cast<float>(Config::LATIME_FEREASTRA));
        float startY = Random::getFloat(-400.f, -50.f);
        p.pozitie = {startX, startY};


        float vx = Random::getFloat(-50.f, 50.f);
        float vy = Random::getFloat(100.f, 300.f);
        p.viteza = {vx, vy};

        p.durataViata = p.durataMaxima = Random::getFloat(3.0f, 6.0f);

        int r = Random::getInt(0, 5);
        switch(r) {
            case 0: p.culoareStart = sf::Color::Red; break;
            case 1: p.culoareStart = sf::Color::Green; break;
            case 2: p.culoareStart = sf::Color::Blue; break;
            case 3: p.culoareStart = sf::Color::Yellow; break;
            case 4: p.culoareStart = sf::Color::Magenta; break;
            case 5: p.culoareStart = sf::Color::Cyan; break;
        }

        p.marime = Random::getFloat(1.5f, 2.5f);
        particule.push_back(p);
    }
}

void ManagerEfecte::playTasta() {
    if (poolSuneteTasta.empty()) return;
    sf::Sound& sunetAles = poolSuneteTasta[indexSunetCurent];
    sunetAles.setPitch(Random::getFloat(0.9f, 1.1f));
    sunetAles.play();
    indexSunetCurent = (indexSunetCurent + 1) % poolSuneteTasta.size();
}

void ManagerEfecte::playBoom() {
    sunetBoom.setPitch(Random::getFloat(0.8f, 1.2f));
    sunetBoom.play();
}

void ManagerEfecte::playWin() {
    sunetWin.play();
}