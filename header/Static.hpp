#pragma once
#include <string>
#include <random>
struct Config {
    static const int LATIME_FEREASTRA = 700;
    static const int INALTIME_FEREASTRA = 800;
    static const std::string TITLU_FEREASTRA;
    static const std::string CALE_FONT;
    static const std::string CALE_SCORURI;
};

class Random {
private:
    static std::mt19937 generator;

public:
    static int getInt(int min, int max);
    static void init();
};