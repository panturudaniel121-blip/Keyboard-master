#include "Static.hpp"

const std::string Config::TITLU_FEREASTRA = "Keyboard Master";
const std::string Config::CALE_FONT = "fonts/ARIAL.TTF";
const std::string Config::CALE_SCORURI = "Date/score_board.txt";
const std::string Config::CALE_SCORURI_ENDLESS = "Date/scoruri_endless.txt";
const std::string Config::CALE_MUZICA_MENIU = "Sunet/meniu.mp3";
const std::string Config::CALE_MUZICA_USOR = "Sunet/normala.mp3";
const std::string Config::CALE_MUZICA_MEDIU = "Sunet/normala.mp3";
const std::string Config::CALE_MUZICA_GREU = "Sunet/greu.mp3";
const std::string Config::CALE_MUZICA_PIERDUT = "Sunet/meniu.mp3";
const std::string Config::CALE_MUZICA_ENDLESS = "Sunet/endless.mp3";
const std::string Config::CALE_SUNET_TASTA =    "Sunet/tasta.wav";
const std::string Config::CALE_SUNET_BOOM = "Sunet/boom.wav";

std::mt19937 Random::generator(std::random_device{}());
int Random::getInt(const int min, const int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator);
}

float Random::getFloat(const float min, const float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(generator);
}