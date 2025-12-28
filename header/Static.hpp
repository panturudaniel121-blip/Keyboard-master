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
    std::string eticheta;
    T valoare;
public:
    StatisticaSesiune(std::string  e, const T& v) : eticheta(std::move(e)), valoare(v) {}

    [[nodiscard]] std::string genereazaText() const {
        return eticheta + ": " + std::to_string(valoare);
    }
};

template <>
inline std::string StatisticaSesiune<std::string>::genereazaText() const {
    return eticheta + ": " + valoare;
}

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

class ManagerSesiune {
private:
    ManagerSesiune() : jocInceput(false) {} // Constructor privat
    bool jocInceput;
public:
    static ManagerSesiune& getInstance() {
        static ManagerSesiune instance;
        return instance;
    }

    // Functie non-statica (pentru a forta utilizarea getInstance)
    void marcheazaInceput() { jocInceput = true; }

    ManagerSesiune(const ManagerSesiune&) = delete;
    void operator=(const ManagerSesiune&) = delete;
};