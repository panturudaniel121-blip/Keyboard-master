#pragma once
#include <string>
#include <random>
struct Config {
    static constexpr int LATIME_FEREASTRA = 700;
    static constexpr int INALTIME_FEREASTRA = 800;
    static const std::string TITLU_FEREASTRA;
    static const std::string CALE_FONT;
    static const std::string CALE_SCORURI;
    static const std::string CALE_MUZICA_MENIU;
    static const std::string CALE_MUZICA_USOR;
    static const std::string CALE_MUZICA_MEDIU;
    static const std::string CALE_MUZICA_GREU;
    static const std::string CALE_MUZICA_PIERDUT;
};

class Random {
private:
    static std::mt19937 generator;

public:
    static int getInt(int min, int max);
};