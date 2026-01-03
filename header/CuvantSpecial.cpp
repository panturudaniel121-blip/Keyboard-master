#include "CuvantSpecial.hpp"
#include "Static.hpp"

sf::Color CuvantSpecial::getCuloare(const TipCuvant tip) {
    switch (tip) {
        case TipCuvant::BonusHP:          return {0, 150, 0};
        case TipCuvant::BonusTimp:        return sf::Color::Blue;
        case TipCuvant::BonusInstant:     return sf::Color::Black;
        case TipCuvant::BonusTripluScor:  return {255, 215, 0};
        case TipCuvant::BonusComboMax:    return {255, 0, 255};
        case TipCuvant::Capcana:          return {80, 80, 80};
        default:                          return sf::Color::Red;
    }
}

TipCuvant CuvantSpecial::genereazaTipAleatoriu() {
    const int r = Random::getInt(0, 4);

    if (r == 0) return TipCuvant::BonusHP;
    if (r == 1) return TipCuvant::BonusTimp;
    if (r == 2) return TipCuvant::BonusInstant;
    if (r == 3) return TipCuvant::BonusTripluScor;
    return TipCuvant::BonusComboMax;
}