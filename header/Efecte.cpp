#include "Efecte.hpp"
#include "Static.hpp"
#include <cmath>
#include <iostream>

ManagerEfecte::ManagerEfecte()
    : bufferTasta(),
      bufferBoom(),
      sunetTasta(bufferTasta),
      sunetBoom(bufferBoom)
{
    shapeParticula.setSize({5.f, 5.f});
    shapeParticula.setOrigin({2.5f, 2.5f});
}

void ManagerEfecte::incarcaSunete() {
    // Incarcam sunetul pentru tasta
    if (!bufferTasta.loadFromFile(Config::CALE_SUNET_TASTA)) {
        std::cerr << "[Eroare] Nu s-a putut incarca sunetul de tasta: " << Config::CALE_SUNET_TASTA << "\n";
    } else {
        sunetTasta.setBuffer(bufferTasta);
        sunetTasta.setVolume(50.f);
    }

    // Incarcam sunetul pentru explozie
    if (!bufferBoom.loadFromFile(Config::CALE_SUNET_BOOM)) {
        std::cerr << "[Eroare] Nu s-a putut incarca sunetul de boom: " << Config::CALE_SUNET_BOOM << "\n";
    } else {
        sunetBoom.setBuffer(bufferBoom);
        sunetBoom.setVolume(70.f);
    }
}

// ... restul functiilor raman neschimbate ...
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
        col.a = static_cast<unsigned char>(255.0f * alphaRatio);
        shapeParticula.setFillColor(col);
        shapeParticula.setScale({alphaRatio * p.marime, alphaRatio * p.marime});
        window.draw(shapeParticula);
    }
}

void ManagerEfecte::spawnLiteraCorecta(sf::Vector2f pos) {
    for (int i = 0; i < 5; ++i) {
        Particula p;
        p.pozitie = pos;
        float unghi = Random::getFloat(0.f, 360.f) * 3.14159f / 180.f;
        float viteza = Random::getFloat(50.f, 150.f);
        float vx = static_cast<float>(std::cos(unghi)) * viteza;
        float vy = static_cast<float>(std::sin(unghi)) * viteza - 100.f;
        p.viteza = {vx, vy};
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
        float vx = static_cast<float>(std::cos(unghi)) * viteza;
        float vy = static_cast<float>(std::sin(unghi)) * viteza;
        p.viteza = {vx, vy};
        p.durataViata = p.durataMaxima = Random::getFloat(0.5f, 1.0f);
        p.culoareStart = culoareBaza;
        p.marime = Random::getFloat(1.5f, 3.0f);
        particule.push_back(p);
    }
}

void ManagerEfecte::playTasta() {
    float pitch = Random::getFloat(0.9f, 1.1f);
    sunetTasta.setPitch(pitch);
    sunetTasta.play();
}

void ManagerEfecte::playBoom() {
    float pitch = Random::getFloat(0.8f, 1.2f);
    sunetBoom.setPitch(pitch);
    sunetBoom.play();
}