#include "CuvantSpecial.hpp"
#include "Static.hpp"

sf::Color CuvantSpecial::getCuloare(TipCuvant tip) {
    switch (tip) {
        case TipCuvant::BonusHP:      return {0, 150, 0};
        case TipCuvant::BonusTimp:    return sf::Color::Blue;
        case TipCuvant::BonusInstant: return sf::Color::Black;
        default:                      return sf::Color::Red;
    }
}

TipCuvant CuvantSpecial::genereazaTipAleatoriu() {
    const int r = Random::getInt(0, 2);

    if (r == 0) return TipCuvant::BonusHP;
    if (r == 1) return TipCuvant::BonusTimp;
    return TipCuvant::BonusInstant;
}