#pragma once
#include <iostream>
#include <string>
#include <random>
#include <utility>

template <typename T>
T clamp(T valoare, T minim, T maxim) {
    if (valoare < minim) return minim;
    if (valoare > maxim) return maxim;
    return valoare;
}

template <typename T>
class StatisticaSesiune {
    std::string nume;
    T valoare;
public:
    StatisticaSesiune(std::string n, T v) : nume(std::move(n)), valoare(v) {}
    [[nodiscard]] std::string genereazaText() const {
        return nume + ": " + std::to_string(valoare);
    }
};

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

class Logger {
    Logger() = default;
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    static void log(const std::string& mesaj) {
        std::cout << "[GAME LOG]: " << mesaj << std::endl;
    }
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};