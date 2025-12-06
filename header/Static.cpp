#include "Static.hpp"

const std::string Config::TITLU_FEREASTRA = "Keyboard Master";
const std::string Config::CALE_FONT = "fonts/ARIAL.TTF";
const std::string Config::CALE_SCORURI = "Date/score_board.txt";

std::mt19937 Random::generator(std::random_device{}());

int Random::getInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator);
}

void Random::init() {
}