#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "Buton.hpp"

struct Achievement {
    std::string id;
    std::string nume;
    std::string descriereBase;

    int nivelCurent;
    std::vector<int> praguri;

    bool esteMaxat() const {
        return nivelCurent >= static_cast<int>(praguri.size());
    }

    int getUrmatorulPrag() const {
        if (esteMaxat()) return 0;
        return praguri[nivelCurent];
    }
};

class ManagerAchievements {
    std::vector<Achievement> listaAchievements;
    const sf::Font* fontRef;

    sf::Text* notificareTitlu;
    sf::Text* notificareNume;
    sf::Text* titluMeniu;

    sf::RectangleShape notificareBg;
    float timpAfisareNotificare;
    bool afiseazaNotificare;

    sf::SoundBuffer bufferSunet;
    sf::Sound* sunetUnlock;

    ButonMeniu* butonMeniu;
    ButonResetAchievements* butonReset;

    void incarcaProgres();
    void salveazaProgres() const;

    static sf::Color getCuloareNivel(int nivel) ;

public:
    ManagerAchievements();
    ~ManagerAchievements();

    void initializeaza(const sf::Font& font);

    void verificaConditii(int scor, int combo, int wave, int cuvinteTotale, float acuratete, int hp);

    void deblocheaza(int index);

    void actualizeaza(float dt, sf::Vector2f mousePos);
    void deseneazaMeniu(sf::RenderWindow& window) const;
    void deseneazaNotificare(sf::RenderWindow& window) const;

    bool aDatClickMeniu(sf::Vector2f mousePos) const;
    void reseteazaProgres();
    bool aDatClickReset(sf::Vector2f mousePos) const;

    void verificaConditii(int scor, int combo, int wave, int cuvinteTotale, float acuratete, int hp, bool victorie, int dificultateEnum);
};