#pragma once
#include <SFML/Graphics.hpp>

enum class TipCuvant {
    Niciunul,
    Normal,
    BonusHP,
    BonusTimp,
    BonusInstant,
    BonusTripluScor,
    BonusComboMax
};

class CuvantSpecial {
public:
    static sf::Color getCuloare(TipCuvant tip);
    static TipCuvant genereazaTipAleatoriu();
};